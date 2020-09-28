#include <ncurses.h>
#include <locale.h>
#include <sys/stat.h>

ssize_t get_line_length(FILE* fp) {
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
    int lines_num = 0;

    for (int i = 1; i <= max_lines_num; i++) {
        ssize_t len = get_line_length(fp);
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
        // Status line
        printw("%s--%ld:%ld", fname, block_lines_offset, block_lines_start_offset);
        hline('-', width);
        move(1, 0);
        //
        for (int i = 0; i < lines_num; i++) {
            ssize_t pos_to_seek = block_lines_start_poses[i] + block_lines_start_offset;
            if (pos_to_seek >= block_lines_start_poses[i+1]) {
                pos_to_seek = block_lines_start_poses[i+1] - 1;
            }
            fseek(fp, pos_to_seek, SEEK_SET);
            fgets(line_buf, width, fp);
            printw("%s", line_buf);
        }
        refresh();
        int c = getch();
        if (c == KEY_UP) { // Forward shift by 1 line, get the top
            // TODO
        } else if (c == KEY_DOWN) { // Backward shift by 1 line, get the bottom
            // TODO
        } else if (c == KEY_LEFT) { // Decrease line beginning offset by 1
            if (--block_lines_start_offset < 0) {
                block_lines_start_offset = 0;
            }
        } else if (c == KEY_RIGHT) { // Increase line beginning offset by 1
            block_lines_start_offset++;
        } else if (c == 'q') {
            quit = true;
        }
    }

    fclose(fp);
    endwin();

    return 0;
}
