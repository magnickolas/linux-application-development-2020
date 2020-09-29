#include "./file_with_name.h"

FileWithName get_file_with_name(const char* fname) {
    FileWithName f;
    f.name = fname;
    f.fp = fopen(f.name, "r");
    return f;
}

void close_file_with_name(FileWithName f) {
    fclose(f.fp);
}

ssize_t get_line_chars_num(FileWithName f) {
    ssize_t res = 0;
    int c;
    while ((c = fgetc(f.fp)) != EOF && c != '\n') {
        res++;
    }
    if (c == EOF && res == 0) {
        return -1;
    }
    res += (c == '\n');
    return res;
}

ssize_t get_line_chars_num_backwards(FileWithName f) {
    ssize_t res = 0;
    int c;
    if (ftell(f.fp) == 0) {
        return 2 - (fgetc(f.fp) == '\n');
    }
    while (ftell(f.fp) > 0 && (c = fgetc(f.fp)) != '\n') {
        res++;
        fseek(f.fp, -2, SEEK_CUR);
    }
    if (ftell(f.fp) == 0) {
        res += 2;
    }
    res += (c == '\n');
    return res;
}

