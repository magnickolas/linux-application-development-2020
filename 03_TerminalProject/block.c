#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./block.h"
#include "./file_with_name.h"

struct BlockState {
    FileWithName f;
    size_t max_lines_num;
    size_t lines_num;
    ssize_t* block_lines_start_poses;
    ssize_t block_lines_offset;
    ssize_t block_lines_start_offset;
    char* line_buf;
    int max_block_line_length;
};

BlockState* init_block_state(FileWithName f, const WindowSize ws) {
    BlockState* bs = malloc(sizeof(BlockState));
    bs->f = f;
    bs->max_lines_num = ws.height;
    if (bs->max_lines_num > 0) {
        bs->max_lines_num--;
    }
    bs->lines_num = 0;
    bs->block_lines_start_poses =
        malloc((bs->max_lines_num + 1) * sizeof(*bs->block_lines_start_poses));
    bs->block_lines_start_poses[0] = 0;
    bs->block_lines_offset = 0;
    bs->block_lines_start_offset = 0;
    bs->line_buf = malloc((ws.width + 2) * sizeof(*bs->line_buf));
    for (size_t i = 1; i <= bs->max_lines_num; i++) {
        ssize_t len = get_line_chars_num(bs->f);
        if (len == -1) {
            break;
        }
        bs->block_lines_start_poses[i] = bs->block_lines_start_poses[i-1] + len;
        bs->lines_num++;
    }
    bs->max_block_line_length = 0;

    return bs;
}

void free_block_state(BlockState* bs) {
    free(bs->block_lines_start_poses);
    free(bs->line_buf);
    free(bs);
}

void output_content(BlockState* bs, const WindowSize ws) {
    bs->max_block_line_length = 0;
    // Status line
    printw("%s--%ld:%ld", bs->f.name, bs->block_lines_offset, bs->block_lines_start_offset);
    hline('-', ws.width);
    move(1, 0);
    // Output file content block
    for (size_t i = 0; i < bs->lines_num; i++) {
        ssize_t pos_to_seek = bs->block_lines_start_poses[i] + bs->block_lines_start_offset;
        if (pos_to_seek >= bs->block_lines_start_poses[i+1]) {
            pos_to_seek = bs->block_lines_start_poses[i+1] - 1;
        }
        fseek(bs->f.fp, pos_to_seek, SEEK_SET);
        assert(fgets(bs->line_buf, ws.width, bs->f.fp) != NULL);
        int len = strlen(bs->line_buf);
        if (len > bs->max_block_line_length) {
            bs->max_block_line_length = len;
        }
        printw("%s", bs->line_buf);
    }
}

void move_up(BlockState* bs) {
    if (bs->block_lines_offset > 0) {
        bs->block_lines_offset--;
        if (bs->lines_num < bs->max_lines_num) {
            bs->lines_num++;
        }
        for (ssize_t i = bs->lines_num; i > 0; i--) {
            bs->block_lines_start_poses[i] = bs->block_lines_start_poses[i-1];
        }
        assert(bs->block_lines_start_poses[0] >= 1);
        ssize_t backward_offset = 2 - (bs->block_lines_start_poses[0] == 1);
        fseek(bs->f.fp, bs->block_lines_start_poses[0] - backward_offset, SEEK_SET);
        ssize_t len = get_line_chars_num_backwards(bs->f);
        bs->block_lines_start_poses[0] -= len;
    }
}

void move_down(BlockState* bs) {
    if (bs->lines_num > 0) {
        bs->block_lines_offset++;
        for (size_t i = 0; i < bs->lines_num; i++) {
            bs->block_lines_start_poses[i] = bs->block_lines_start_poses[i+1];
        }
        fseek(bs->f.fp, bs->block_lines_start_poses[bs->lines_num], SEEK_SET);
        ssize_t len = get_line_chars_num(bs->f);
        if (len != -1) {
            bs->block_lines_start_poses[bs->lines_num] += len;
        } else {
            if (bs->lines_num > 0) {
                bs->lines_num--;
            }
        }
    }
}

void move_left(BlockState* bs) {
    if (bs->block_lines_start_offset > 0) {
        bs->block_lines_start_offset--;
    }
}

void move_right(BlockState* bs) {
    if (bs->max_block_line_length > 1) {
        bs->block_lines_start_offset++;
    }
}

void move_to_beginning(BlockState* bs) {
    fseek(bs->f.fp, 0, SEEK_SET);
    bs->lines_num = 0;
    bs->block_lines_start_poses[0] = 0;
    for (size_t i = 1; i <= bs->max_lines_num; i++) {
        ssize_t len = get_line_chars_num(bs->f);
        if (len == -1) {
            break;
        }
        bs->block_lines_start_poses[i] = bs->block_lines_start_poses[i-1] + len;
        bs->lines_num++;
    }
    bs->block_lines_offset = 0;
    bs->block_lines_start_offset = 0;
}

void move_to_line_start(BlockState* bs) {
    bs->block_lines_start_offset = 0;
}

