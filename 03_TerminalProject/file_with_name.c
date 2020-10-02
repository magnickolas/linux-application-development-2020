#include "./file_with_name.h"
#include <wchar.h>

FileWithName get_file_with_name(const char* fname) {
    FileWithName f;
    f.name = fname;
    f.fp = fopen(f.name, "r");
    fwide(f.fp, 1);
    return f;
}

void close_file_with_name(FileWithName f) {
    fclose(f.fp);
}

ssize_t get_line_chars_num(FileWithName f) {
    ssize_t res = 0;
    wint_t c;
    while ((c = fgetwc(f.fp)) != WEOF && c != '\n') {
        res++;
    }
    if (c == WEOF && res == 0) {
        return -1;
    }
    res += (c == '\n');
    return res;
}

