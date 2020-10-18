#define _GNU_SOURCE
#include "pcre.h"
#include "wstring.h"
#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define KEY_ESC 033

typedef struct WindowSize {
    int height;
    int width;
} WindowSize;

WindowSize wget_window_size(WINDOW* win) {
    WindowSize res;
    getmaxyx(win, res.height, res.width);
    return res;
}

int main() {
    setlocale(LC_ALL, "");

    initscr();
    set_escdelay(0);
    noecho();
    cbreak();
    keypad(stdscr, true);

    WindowSize ws = wget_window_size(stdscr);
    int input_fields_width = ws.width / 2;
    int input_fields_height = 3;
    assert(input_fields_width >= 1);
    WINDOW* win_pat = newwin(input_fields_height, input_fields_width, 0, 0);
    WINDOW* win_sub = newwin(input_fields_height, input_fields_width, 0,
                             ws.width - input_fields_width);
    WINDOW* win_res = newwin(ws.height - input_fields_height, ws.width,
                             input_fields_height, 0);
    refresh();

    WINDOW* win_in_focus = win_pat;

    bool quit = false;

    Wstring* pat_wstr = new_wstring();
    Wstring* sub_wstr = new_wstring();
    char* res_str = malloc(sizeof(*res_str));
    assert(res_str != NULL);
    res_str[0] = '\0';

    while (!quit) {
        box(win_pat, 0, 0);
        box(win_sub, 0, 0);
        box(win_res, 0, 0);
        mvwaddwstr(win_pat, 1, 1, pat_wstr->buf);
        mvwaddwstr(win_sub, 1, 1, sub_wstr->buf);
        // PRINT RES
        size_t res_len = strlen(res_str);
        size_t printed_len = 0;
        int str_num = 0;
        while (printed_len < res_len) {
            size_t str_len = strcspn(res_str + printed_len, "\n\0");
            mvwaddnstr(win_res, str_num + 1, 1, res_str + printed_len, str_len);
            printed_len += str_len + 1;
            str_num++;
        }

        wrefresh(win_pat);
        wrefresh(win_sub);
        wrefresh(win_res);
        wnoutrefresh(win_in_focus);
        doupdate();

        wint_t c;
        get_wch(&c);
        werase(win_pat);
        werase(win_sub);
        werase(win_res);

        if (c == KEY_BTAB) {
            if (win_in_focus == win_pat) {
                win_in_focus = win_sub;
            } else {
                win_in_focus = win_pat;
            }
        } else if (c == KEY_ESC) {
            quit = true;
        } else if (c == KEY_BACKSPACE) {
            if (win_in_focus == win_pat) {
                pop_wstring(pat_wstr);
            } else {
                pop_wstring(sub_wstr);
            }
        } else if (c == '\n') {
            // Pass
        } else {
            if (win_in_focus == win_pat) {
                push_wstring(pat_wstr, c);
            } else {
                push_wstring(sub_wstr, c);
            }
        }

        if (pat_wstr->len + sub_wstr->len > 0) {
            // Get match groups
            char* pat_str =
                malloc((pat_wstr->len + 1) * sizeof(*pat_wstr->buf));
            char* sub_str =
                malloc((sub_wstr->len + 1) * sizeof(*sub_wstr->buf));
            sprintf(pat_str, "%ls", pat_wstr->buf);
            sprintf(sub_str, "%ls", sub_wstr->buf);
            free(res_str);
            res_str = get_match_groups(pat_str, sub_str);
            free(pat_str);
            free(sub_str);
        } else {
            res_str[0] = '\0';
        }
    }

    destroy_wstring(pat_wstr);
    destroy_wstring(sub_wstr);
    free(res_str);

    endwin();

    return 0;
}
