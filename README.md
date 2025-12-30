# B-textEditor

B-textEditor is a lightweight, modular, terminal-based text editor written in C, inspired by the Kilo editor.
It showcases low-level terminal handling, dynamic memory management, and efficient screen rendering using POSIX APIs.


## 📁 Project Structure

```
├── include
│   ├── common.h
│   ├── data.h
│   └── prototypes.h
├── src
│   ├── data.c
│   ├── editor.c
│   ├── file_io.c
│   ├── find.c
│   ├── input.c
│   ├── main.c
│   ├── output.c
│   ├── row.c
│   └── terminal.c
├── Makefile
└── README.md

```
## 🛠️ Build Instructions

The project uses a Makefile to automate compilation and manage object files.

### Build the editor

Run the following command from the project root:


__make__


This will:
- Compile all source files
- Create an obj/ directory for object files
- Generate the B-textEditor executable

### Clean build files

Remove compiled files and reset the build state:


__make clean__


---

## 🚀 Usage

### Running the Editor

To open the editor with a file:

./B-textEditor filename.txt


If the file does not exist, it will be created when saved.

---

## ⌨️ Controls

- Arrow Keys — Move the cursor
- Page Up / Page Down — Scroll by one screen
- Home / End — Jump to the start or end of the current line
- Enter — Insert a new line
- Backspace — Delete characters and merge lines
- Ctrl + Q — Quit the editor immediately

---

## 🔧 Technical Overview

### Raw Terminal Mode

The editor disables standard terminal features such as:
- Canonical input mode (ICANON)
- Character echoing (ECHO)

This allows input to be processed byte-by-byte for real-time interaction.

### Rendering Pipeline

- Uses an internal append buffer (abuf)
- All screen updates are batched into a single write() call
- Minimizes flickering and improves rendering performance

### Memory Management

- Text is stored as a dynamic array of erow structures
- Memory is resized using realloc() as lines are inserted, deleted, or modified

### Tab Handling

- Tabs are expanded virtually using a render index (rx)
- Ensures correct alignment on an 8-column grid

---

## 📌 Inspiration

This project is heavily inspired by the Kilo text editor, adapted into a modular, multi-file C codebase for better scalability and learning purposes.


