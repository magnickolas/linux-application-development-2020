#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <sys/stat.h>

#define KEY_ESC 27

ssize_t get_line_chars_num(FILE* fp) {
    ssize_t res = 0;
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        res++;
    }
    if (c == EOF && res == 0) {
        return -1;
    }
    res += (c == '\n');
    return res;
}

ssize_t get_line_chars_num_backwards(FILE* fp) {
    ssize_t res = 0;
    int c;
    if (ftell(fp) == 0) {
        return 2 - (fgetc(fp) == '\n');
    }
    while (ftell(fp) > 0 && (c = fgetc(fp)) != '\n') {
        res++;
        fseek(fp, -2, SEEK_CUR);
    }
    if (ftell(fp) == 0) {
        res += 2;
    }
    res += (c == '\n');
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

    FILE* fp = fopen(fname, "r");
    
    initscr();
    set_escdelay(0);
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);

    int height, width;
    getmaxyx(stdscr, height, width);

    ssize_t from_pos = 0, to_pos = 0;
    int max_lines_num = height - 1;
    ssize_t block_lines_start_poses[max_lines_num + 1];
    block_lines_start_poses[0] = 0;
    ssize_t block_lines_start_offset = 0;
    ssize_t block_lines_offset = 0;
    ssize_t lines_num = 0;

    for (ssize_t i = 1; i <= max_lines_num; i++) {
        ssize_t len = get_line_chars_num(fp);
        if (len == -1) {
            break;
        }
        block_lines_start_poses[i] = block_lines_start_poses[i-1] + len;
        lines_num++;
    }

    char line_buf[width+2];
    bool quit = false;

    while (!quit) {
        clear();
        ssize_t max_block_line_length = 0;
        // Status line
        printw("%s--%ld:%ld", fname, block_lines_offset, block_lines_start_offset);
        hline('-', width);
        move(1, 0);
        // Output file content block
        for (ssize_t i = 0; i < lines_num; i++) {
            ssize_t pos_to_seek = block_lines_start_poses[i] + block_lines_start_offset;
            if (pos_to_seek >= block_lines_start_poses[i+1]) {
                pos_to_seek = block_lines_start_poses[i+1] - 1;
            }
            fseek(fp, pos_to_seek, SEEK_SET);
            fgets(line_buf, width, fp);
            ssize_t len = strlen(line_buf);
            if (len > max_block_line_length) {
                max_block_line_length = len;
            }
            printw("%s", line_buf);
        }
        refresh();

        int c = getch();
        if (c == KEY_UP || c == 'k') {
            // Forward shift by 1 line, get the top
            if (block_lines_offset > 0) {
                block_lines_offset--;
                if (lines_num < max_lines_num) {
                    lines_num++;
                }
                for (ssize_t i = lines_num; i > 0; i--) {
                    block_lines_start_poses[i] = block_lines_start_poses[i-1];
                }
                assert(block_lines_start_poses[0] >= 1);
                ssize_t backward_offset = 2 - (block_lines_start_poses[0] == 1);
                fseek(fp, block_lines_start_poses[0] - backward_offset, SEEK_SET);
                ssize_t len = get_line_chars_num_backwards(fp);
                block_lines_start_poses[0] -= len;
            }
        } else if (c == KEY_DOWN || c == 'j' || c == ' ') {
            // Backward shift by 1 line, get the bottom
            if (lines_num > 0) {
                block_lines_offset++;
                for (ssize_t i = 0; i < lines_num; i++) {
                    block_lines_start_poses[i] = block_lines_start_poses[i+1];
                }
                fseek(fp, block_lines_start_poses[lines_num], SEEK_SET);
                ssize_t len = get_line_chars_num(fp);
                if (len != -1) {
                    block_lines_start_poses[lines_num] += len;
                } else {
                    if (lines_num > 0) {
                        lines_num--;
                    }
                }
            }
        } else if (c == KEY_LEFT || c == 'h') {
            // Decrease line beginning offset by 1
            if (block_lines_start_offset > 0) {
                block_lines_start_offset--;
            }
        } else if (c == KEY_RIGHT || c == 'l') {
            // Increase line beginning offset by 1
            if (max_block_line_length > 1) {
                block_lines_start_offset++;
            }
        } else if (c == 'g') {
            fseek(fp, 0, SEEK_SET);
            lines_num = 0;
            block_lines_start_poses[0] = 0;
            for (ssize_t i = 1; i <= max_lines_num; i++) {
                ssize_t len = get_line_chars_num(fp);
                if (len == -1) {
                    break;
                }
                block_lines_start_poses[i] = block_lines_start_poses[i-1] + len;
                lines_num++;
            }
            block_lines_offset = 0;
            block_lines_start_offset = 0;
        } else if (c == 'q' || c == KEY_ESC) {
            quit = true;
        }
    }

    fclose(fp);
    endwin();

    return 0;
}
