B-textEditor/
├── include/
│   ├── common.h        # Standard headers, macros, constants
│   ├── data.h          # Core structs and global state declarations
│   └── prototypes.h   # Function prototypes
├── src/
│   ├── data.c          # Global editor state initialization
│   ├── editor.c        # Core editor logic and control flow
│   ├── file_io.c       # File loading and saving
│   ├── input.c         # Keypress handling
│   ├── main.c          # Program entry point and main loop
│   ├── output.c        # Screen rendering and refresh logic
│   ├── row.c           # Line/row manipulation utilities
│   └── terminal.c      # Raw mode and low-level terminal control
├── Makefile            # Build configuration
└── README.md           # Project documentation
