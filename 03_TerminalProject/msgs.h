#ifndef MSGS_H_
#define MSGS_H_
#include <stdio.h>

void print_missing_filename_message(const char* name) {
    fprintf(stderr, "%s: Missing filename\n", name);
}

void print_too_many_arguments_message(const char* name) {
    fprintf(stderr, "%s: Too many arguments\n", name);
}

void print_usage_message(const char* name) {
    printf(
        "Usage: %s [FILE]\n"
        "Show the file's content, scrolling is supported\n"
        "\n"
        "Key bindings:\n"
        "  UpArrow or 'k'               scroll one line up\n"
        "  DownArrow or 'j' or SPACE    scroll one line down\n"
        "  LeftArrow or 'h'             scroll one line left\n"
        "  RightArrow or 'l'            scroll one line right\n"
        "  Home or 'g'                  move to the beginning\n"
        "  '0'                          move to the first column\n"
        "  PgUp                         scroll one page up\n"
        "  PgDn                         scroll one page down\n"
        "  ESC or 'q'                   quit\n",
        name
    );
}

void print_invalid_option_message(const char* name, const char* option) {
    fprintf(
        stderr,
        "%s: invalid option -- '%s'\n"
        "Try '%s --help' for more information.\n",
        name, option, name
    );
}

void print_no_such_file_message(const char* fname) {
    fprintf(stderr, "%s: No such file or directory\n", fname);
}

void print_is_a_directory_message(const char* fname) {
    fprintf(stderr, "%s: Is a directory\n", fname);
}
#endif //MSGS_H_
