# B-textEditor

B-textEditor is a modular, terminal-based text editor written in C99, inspired by the [Kilo editor](https://github.com/antirez/kilo). It uses raw terminal mode and POSIX APIs to process input byte-by-byte and render the screen in a single `write()` call.

## Where to start

- **New to the editor?** Follow the [tutorial](tutorial.md). It takes you from a fresh clone to editing, searching, and saving a file.
- **Want to extend the editor?** Read [how to add a keybinding](how-to-add-a-keybinding.md).
- **Looking up a key or a source module?** See the [reference](reference.md).

## Requirements

- A POSIX system (Linux or macOS)
- `gcc` and `make`

