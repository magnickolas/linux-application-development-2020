# Show

Show the file's content, scrolling is supported.

## Features
- Scroll up/down/left/right
- Scroll page up/down
- Go to the beginning
- Go to the first column
- Multibyte encoding
- Vim-like key bindings

## Key bindings
| Key                       | Action                   |
| ------------------------- | ------------------------ |
| UpArrow or 'k'            | scroll one line up       |
| DownArrow or 'j' or SPACE | scroll one line down     |
| LeftArrow or 'h'          | scroll one line left     |
| RightArrow or 'l'         | scroll one line right    |
| Home or 'g'               | move to the beginning    |
| '0'                       | move to the first column |
| PgUp                      | scroll one page up       |
| PgDn                      | scroll one page down     |
| ESC or 'q'                | quit                     |

## Building
Install pkg-config, libncursesw and gcc or clang compiler.

Then

```shell
make
./Show --help
```
