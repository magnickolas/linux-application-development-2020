#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <sys/stat.h>
#include "./show.h"
#include "./file_with_name.h"
#include "./block.h"

#define KEY_ESC 27

WindowSize get_window_size() {
    WindowSize res;
    getmaxyx(stdscr, res.height, res.width);
    return res;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Missing filename\n");
        return 1;
    }
    setlocale(LC_ALL, "");

    const char* fname = argv[1];
    struct stat st;
    if (stat(fname, &st) != 0) {
        fprintf(stderr, "%s: No such file or directory\n", fname);
        return 1;
    }
    if (S_ISDIR(st.st_mode)) {
        fprintf(stderr, "%s: Is a directory\n", fname);
        return 1;
    }

    FileWithName f = get_file_with_name(fname);

    initscr();
    set_escdelay(0);
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);

    WindowSize ws = get_window_size();
    BlockState* bs = init_block_state(f, ws);

    bool quit = false;

    while (!quit) {
        clear();
        output_content(bs, ws);
        refresh();

        int c = getch();
        if (c == KEY_UP || c == 'k') {
            move_up(bs);
        } else if (c == KEY_DOWN || c == 'j' || c == ' ') {
            move_down(bs);
        } else if (c == KEY_LEFT || c == 'h') {
            move_left(bs);
        } else if (c == KEY_RIGHT || c == 'l') {
            move_right(bs);
        } else if (c == 'g') {
            move_to_beginning(bs);
        } else if (c == '0') {
            move_to_line_start(bs);
        } else if (c == 'q' || c == KEY_ESC) {
            quit = true;
        }
    }

    free_block_state(bs);
    close_file_with_name(f);
    endwin();

    return 0;
}
