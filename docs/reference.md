# Reference

## Keybindings

| Key | Action |
| --- | --- |
| Arrow keys | Move the cursor |
| `Page Up` / `Page Down` | Scroll by one screen |
| `Home` / `End` | Jump to the start or end of the line |
| `Enter` | Insert a new line |
| `Backspace`, `Ctrl+H`, `Delete` | Delete a character |
| `Ctrl+S` | Save the file |
| `Ctrl+F` | Incremental search (arrows navigate matches, `Esc` cancels, `Enter` accepts) |
| `Ctrl+Q` | Quit; requires 3 presses when the buffer has unsaved changes |

## Configuration constants

Defined in `include/common.h`:

| Constant | Value | Meaning |
| --- | --- | --- |
| `TAB_STOP` | `8` | Tabs render on an 8-column grid via the render index (`rx`) |
| `B_QUIT_TIMES` | `3` | Presses of `Ctrl+Q` required to discard unsaved changes |
| `B_TEXTEDITOR_VERSION` | `0.0.1` | Version string shown in the welcome message |

## Source modules

| File | Responsibility |
| --- | --- |
| `src/main.c` | Entry point; initializes the editor and runs the input loop |
| `src/terminal.c` | Raw mode setup and teardown (`termios`), key reading, window size |
| `src/input.c` | Keypress dispatch and cursor movement |
| `src/output.c` | Screen rendering via the append buffer (`abuf`); single-`write()` refresh |
| `src/row.c` | Row operations: insert, delete, append, and the `cx`/`rx` conversion |
| `src/editor.c` | High-level editing operations on the buffer |
| `src/file_io.c` | Opening files into rows and serializing rows back to disk |
| `src/find.c` | Incremental search with directional navigation |
| `src/data.c` | Global editor state definition |

## Build targets

| Command | Effect |
| --- | --- |
| `make` | Compiles `src/*.c` into `obj/` and links `B-textEditor` |
| `make clean` | Removes `obj/` and the executable |

Compiler flags: `-Wall -Wextra -pedantic -std=c99 -Iinclude`.
