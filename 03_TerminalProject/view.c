#include <ncurses.h>
#include <locale.h>
#include <sys/stat.h>

ssize_t get_line_length(FILE* fp) {
    ssize_t res = 0;
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        ++res;
    }
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
    ssize_t block_lines_start_poses[height];
    block_lines_start_poses[0] = 0;
    
    for (int i = 1; i < height; i++) {
        ssize_t len = get_line_length(fp);
        block_lines_start_poses[i] = block_lines_start_poses[i-1] + len + 1;
    }
    char line_buf[width+2];
        
    for (int i = 0; i < height; i++) {
        fseek(fp, block_lines_start_poses[i], SEEK_SET);
        fgets(line_buf, width, fp);
        printw("%s", line_buf);
    }
    refresh();
    getch();

    fclose(fp);
    endwin();

    return 0;
}
