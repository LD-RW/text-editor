#include "common.h"
#include "data.h"
#include "prototypes.h"

/**
 * @brief Performs an emergency shutdown and reports system-level failures.
 * @details Before exiting, it restores terminal visibility by clearing the screen 
 * and homing the cursor. It then uses perror() to translate the global 'errno' 
 * into a human-readable string.
 * @param s The name of the function/context where the failure occurred.
 */
void die(const char *s){
    /* Clear screen and reset cursor to ensure the error message is readable. */
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1);
}

/**
 * @brief Restores the terminal to its original "Canonical" state.
 * @details Uses the TCSAFLUSH constant to ensure all output is drained to the 
 * screen and all unread input is discarded before applying the original attributes.
 */
void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) die("tcsetattr");
}

/**
 * @brief Configures the terminal for "Raw Mode" to allow byte-by-byte input handling.
 * @details This function disables standard terminal features like line-buffering, 
 * local echoing, and signal interception. It transitions the terminal from a 
 * "human-friendly" interface to a "program-friendly" data stream.
 */
void enableRawMode(){
    /* Capture the original state to allow for restoration on exit. */
    if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");

    /* Register the cleanup function to be called automatically by the OS. */
    atexit(disableRawMode);

    struct termios raw = E.orig_termios;

    /** * 1. INPUT FLAGS (c_iflag): Handling incoming bytes.
     * BRKINT: Prevents a 'Break' condition from sending a SIGINT.
     * ICRNL:  Disables translation of Carriage Return (13) to Newline (10).
     * INPCK:  Disables legacy parity checking; essential for 8-bit characters.
     * ISTRIP: Prevents stripping the 8th bit of each input byte.
     * IXON:   Disables software flow control (Ctrl-S / Ctrl-Q).
     */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /** * 2. OUTPUT FLAGS (c_oflag): Handling outgoing bytes.
     * OPOST: Disables all "Post-processing" (like translating \n to \r\n).
     * This gives the editor total control over cursor placement.
     */
    raw.c_oflag &= ~(OPOST);

    /** * 3. CONTROL FLAGS (c_cflag): Hardware-level settings.
     * CS8: Sets the character size to 8 bits per byte.
     */
    raw.c_cflag |= (CS8);

    /** * 4. LOCAL FLAGS (c_lflag): Terminal "User Experience" settings.
     * ECHO:   Disables automatic printing of typed characters.
     * ICANON: Disables line-buffering (Canonical Mode); reads byte-by-byte.
     * IEXTEN: Disables implementation-defined input processing (Ctrl-V).
     * ISIG:   Disables signal keys like Ctrl-C (SIGINT) and Ctrl-Z (SIGTSTP).
     */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    /** * 5. CONTROL CHARACTERS (c_cc): Timing and Polling.
     * VMIN:  Set to 0 so read() returns immediately if no data is available.
     * VTIME: Set to 1 (100ms) to create a non-blocking "polling" interval.
     * Together, these prevent the program from "freezing" while waiting for input,
     * allowing the main loop to handle background tasks (like UI refreshes).
     */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    /* Apply the raw attributes to STDIN. */
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
/**
 * @brief Low-level input parser that translates raw terminal bytes into editor keys.
 * @details This function handles the non-blocking read polling (VTIME) and performs
 * real-time parsing of ANSI escape sequences. It ensures that multi-byte inputs
 * (like arrow keys or Page Up/Down) are returned as a single, unique integer constant.
 * * @return int The ASCII value of a keypress, or a constant from the editorKey enum.
 */int editorReadKey(){
    int nread;
    char c;
    /* Busy-wait/Poll for a single byte of input. */
    while((nread = read(STDIN_FILENO, &c, 1)) != 1){
        if(nread == -1 && errno != EAGAIN) die("read");
    }
    /* If we receive an Escape character, check for a following ANSI sequence. */
    if (c == '\x1b') {
        char seq[3];

        /* Attempt to read the next two bytes of the sequence. */
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if(seq[1] >= '0' && seq[1] <= '9'){
                /* Format: <Esc>[<Digit>~ */
                if(read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if(seq[2] == '~'){
                    switch(seq[1]){
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY; // doesn't do anything for now
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            }
            else{
                switch (seq[1]) {
                    /* Format: <Esc>[<Letter> */
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        }
        /* If sequence doesn't match known patterns, treat it as a standalone Esc. */
        return '\x1b';
  } else {
        /* Return the standard ASCII character. */
        return c;
    }
}

/**
 * @brief Retrieves the current cursor coordinates by querying the terminal emulator.
 * @details This function performs a "Synchronous Query." It sends an ANSI Device Status 
 * Report (DSR) sequence to STDOUT and then parses the terminal's immediate response 
 * from STDIN. This is the most reliable method to determine screen dimensions 
 * across different terminal environments.
 * * @param rows Pointer to an integer where the reported vertical position will be stored.
 * @param cols Pointer to an integer where the reported horizontal position will be stored.
 * @return int Returns 0 on success; -1 if the query fails or the response is malformed.
 */
int getCursorPosition(int *rows, int *cols){
    char buf[32];
    unsigned int i = 0;

    /** * STEP 1: SEND DEVICE STATUS REPORT (DSR)
     * We write the escape sequence "\x1b[6n" to the terminal.
     * The '6' argument specifically requests a 'Cursor Position Report'.
     * We don't use ioctl for compatability with old systems
     */
    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    /** * STEP 2: READ THE RESPONSE BUFFER
     * The terminal "replies" by injecting a sequence into our STDIN buffer,
     * formatted as: ^[[<rows>;<cols>R (e.g., ^[[24;80R).
     * We read byte-by-byte until we hit the 'R' terminator.
     */
    while(i < sizeof(buf) - 1){
        if(read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if(buf[i] == 'R') break;
        i++;
    }

    /* Ensure the buffer is a valid C-string for sscanf processing. */
    buf[i] = '\0';

    /** * STEP 3: VALIDATE AND PARSE
     * First, we verify the sequence starts with the standard Escape Control 
     * Sequence (\x1b and '[').
     */
    if(buf[0] != '\x1b' || buf[1] != '[') return -1;

    /** * STEP 4: EXTRACTION VIA SSCANF
     * We pass &buf[2] to sscanf to skip the prefix and jump straight to the numbers.
     * sscanf acts as a pattern matcher: it expects an integer (%d), a literal 
     * semicolon (;), and another integer (%d). It returns the count of 
     * successfully assigned variables (expected: 2).
     */
    if(sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

    return 0;
}

/**
 * @brief Determines the dimensions of the terminal window.
 * @details This function attempts to find the screen width and height using two 
 * methods. "Plan A" uses a direct ioctl() system call to the kernel. If that 
 * fails, "Plan B" moves the cursor to the bottom-right extremity and queries 
 * its position as a fallback.
 * * @param rows Pointer to an integer to store the number of terminal rows.
 * @param cols Pointer to an integer to store the number of terminal columns.
 * @return int Returns 0 on success; -1 on a total failure to determine size.
 */
int getWindowSize(int *rows, int *cols){
    struct winsize ws;

    /** * STRATEGY 1: IOCTL (Modern & Efficient)
     * TIOCGWINSZ asks the OS kernel directly for the terminal's dimensions.
     * This is preferred because it is instant and doesn't require terminal I/O.
     */
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0){

        /** * STRATEGY 2: BOUNDARY PROBING (Fallback)
         * We move the cursor 999 units Right (C) and Down (B). 
         * The cursor is physically capped by the terminal's actual limits.
         */
        if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;

        /* Query the cursor position to find the coordinates of the corner. */
        return getCursorPosition(rows, cols);
    }
    
    else{
        /* Assign results from the successful ioctl call. */
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }

}
