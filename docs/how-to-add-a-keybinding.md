# How to add a keybinding

This guide shows you how to bind a key combination to an editor action. As a working example, you bind ++ctrl+g++ to jump the cursor to the top of the file.

## Prerequisites

- A working build (`make` succeeds)
- Familiarity with C

## 1. Declare the handler

Add the prototype to `include/prototypes.h`:

```c
void editorGotoTop(void);
```

## 2. Implement the action

Add the function to `src/editor.c`:

```c
void editorGotoTop(void){
    E.cx = 0;
    E.cy = 0;
    E.rowOff = 0;
    E.colOff = 0;
}
```

The global editor state `E` (defined in `include/data.h`) holds the cursor position (`cx`, `cy`) and the scroll offsets (`rowOff`, `colOff`). Resetting all four moves the cursor and the viewport to the top-left corner.

## 3. Bind the key

In `src/input.c`, add a case to the `switch` inside `editorProcessKeypress()`:

```c
case CTRL_KEY('g'):
    editorGotoTop();
    break;
```

The `CTRL_KEY(k)` macro in `include/common.h` masks the character with `0x1f`, which mirrors how the terminal encodes Ctrl combinations.

## 4. Rebuild and test

```sh
make
./B-textEditor README.md
```

Scroll down with ++page-down++, then press ++ctrl+g++. The cursor returns to line 1, column 1.

!!! warning "Avoid reserved combinations"
    The terminal driver intercepts some Ctrl combinations before the editor sees them. ++ctrl+c++, ++ctrl+z++, and ++ctrl+d++ carry signal or EOF semantics. Check the existing cases in `editorProcessKeypress()` before you choose a key: ++ctrl+q++, ++ctrl+s++, ++ctrl+f++, ++ctrl+h++, and ++ctrl+l++ are already taken.
