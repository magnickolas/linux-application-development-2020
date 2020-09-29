#ifndef FILE_WITH_NAME_H_
#define FILE_WITH_NAME_H_
#include <stdio.h>
typedef struct FileWithName {
    FILE* fp;
    const char* name;
} FileWithName;
FileWithName get_file_with_name(const char*);
void close_file_with_name(FileWithName);
ssize_t get_line_chars_num(FileWithName);
ssize_t get_line_chars_num_backwards(FileWithName);
#endif //FILE_WITH_NAME_H_
