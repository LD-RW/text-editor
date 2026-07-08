# Tutorial: your first editing session

In this tutorial you build the editor from source, open a file, edit it, search it, and save it. It takes about five minutes.

## 1. Build the editor

Clone the repository and run `make` from the project root:

```sh
git clone https://github.com/LD-RW/text-editor.git
cd text-editor
make
```

`make` compiles every file in `src/` into `obj/` and links the `B-textEditor` executable. The build uses `-Wall -Wextra -pedantic -std=c99`, so it completes without warnings.

## 2. Open a file

```sh
./B-textEditor notes.txt
```

If `notes.txt` does not exist yet, the editor creates it the first time you save. The status bar at the bottom shows the filename, the line count, and a `(modified)` marker once you change the buffer.

## 3. Type some text

Type a few lines. Press ++enter++ to insert a new line and ++backspace++ to delete characters. When you delete at the start of a line, the editor merges it with the line above.

## 4. Save your work

Press ++ctrl+s++. The status bar confirms the number of bytes written to disk and the `(modified)` marker disappears.

## 5. Search the file

Press ++ctrl+f++ and type a word that appears in your text. The search is incremental: the cursor jumps to the first match while you type.

- Press ++arrow-down++ or ++arrow-right++ to jump to the next match.
- Press ++arrow-up++ or ++arrow-left++ to jump to the previous match.
- Press ++enter++ to stay at the current match, or ++esc++ to return to where you started.

## 6. Quit

Press ++ctrl+q++. If the buffer has unsaved changes, the editor warns you and asks you to press ++ctrl+q++ two more times. This guard prevents accidental data loss.

## What you learned

You built the editor, edited a file, saved it with ++ctrl+s++, searched it incrementally with ++ctrl+f++, and quit with the three-press safety guard. The [reference](reference.md) lists every keybinding.
