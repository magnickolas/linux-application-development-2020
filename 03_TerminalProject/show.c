#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <sys/stat.h>
#include "./block.h"
#include "./file_with_name.h"
#include "./msgs.h"
#include "./show.h"

#define KEY_ESC 27

WindowSize wget_window_size(WINDOW* win) {
    WindowSize res;
    getmaxyx(win, res.height, res.width);
    return res;
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        print_missing_filename_message(argv[0]);
        return 1;
    } else if (argc > 2) {
        print_too_many_arguments_message(argv[0]);
        return 1;
    } else if (argv[1][0] == '-') {
        if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            print_usage_message(argv[0]);
            return 0;
        } else {
            print_invalid_option_message(argv[0], argv[1] + 1);
            return 1;
        }
    }
    const char* fname = argv[1];
    struct stat st;
    if (stat(fname, &st) != 0) {
        print_no_such_file_message(argv[1]);
        return 1;
    }
    if (S_ISDIR(st.st_mode)) {
        print_is_a_directory_message(argv[1]);
        return 1;
    }

    FileWithName f = get_file_with_name(fname);

    initscr();
    set_escdelay(0);
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);

    WindowSize ws = wget_window_size(stdscr);
    WINDOW* win = newwin(ws.height-1, ws.width, 1, 0);
    WINDOW* win_status = newwin(1, ws.width, 0, 0);
    WindowSize ws_win = wget_window_size(win);
    BlockState* bs = init_block_state(f, ws_win);
    refresh();

    bool quit = false;

    while (!quit) {
        werase(win);
        werase(win_status);
        output_status_line(bs, win_status);
        output_content(bs, ws_win, win);
        box(win, 0, 0);
        wrefresh(win);
        wrefresh(win_status);

        int c = getch();
        if (c == KEY_UP || c == 'k') {
            move_up(bs);
        } else if (c == KEY_DOWN || c == 'j' || c == ' ') {
            move_down(bs);
        } else if (c == KEY_LEFT || c == 'h') {
            move_left(bs);
        } else if (c == KEY_RIGHT || c == 'l') {
            move_right(bs);
        } else if (c == KEY_HOME || c == 'g') {
            move_to_beginning(bs);
        } else if (c == '0') {
            move_to_line_start(bs);
        } else if (c == KEY_PPAGE) {
            move_page_up(bs);
        } else if (c == KEY_NPAGE) {
            move_page_down(bs);
        } else if (c == 'q' || c == KEY_ESC) {
            quit = true;
        }
    }

    free_block_state(bs);
    close_file_with_name(f);
    endwin();

    return 0;
}
