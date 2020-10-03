#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "./block.h"
#include "./file_with_name.h"

struct BlockState {
    FileWithName f;
    size_t max_lines_num;
    size_t lines_num;
    fpos_t* lines_start_poses;
    fpos_t* block_lines_start_poses;
    size_t lines_start_poses_len;
    size_t block_lines_offset;
    size_t block_lines_start_offset;
    bool reached_eof;
    wchar_t* line_buf;
    int max_block_line_length;
};

BlockState* init_block_state(FileWithName f, const WindowSize ws) {
    BlockState* bs = malloc(sizeof(BlockState));
    bs->f = f;
    bs->max_lines_num = ws.height;
    if (bs->max_lines_num >= 2) {
        bs->max_lines_num -= 2;
    }
    bs->lines_num = 0;
    bs->lines_start_poses_len = bs->max_lines_num + 1;
    bs->lines_start_poses =
        malloc(bs->lines_start_poses_len * sizeof(*bs->lines_start_poses));
    bs->block_lines_start_poses = bs->lines_start_poses;
    bs->block_lines_offset = 0;
    bs->block_lines_start_offset = 0;
    bs->line_buf = malloc((ws.width + 2) * sizeof(*bs->line_buf));
    bs->reached_eof = false;
    fgetpos(bs->f.fp, &bs->block_lines_start_poses[0]);
    for (size_t i = 1; i <= bs->max_lines_num; i++) {
        ssize_t len = get_line_chars_num(bs->f);
        if (len == -1) {
          bs->reached_eof = true;
            bs->lines_start_poses =
                realloc(bs->lines_start_poses,
                        i * sizeof(*bs->lines_start_poses));
            bs->lines_start_poses_len = i;
            break;
        }
        fgetpos(bs->f.fp, &bs->block_lines_start_poses[i]);
        bs->lines_num++;
    }
    bs->max_block_line_length = 0;

    return bs;
}

static size_t fetch_n_lines_poses(BlockState* bs, size_t n) {
    if (bs->reached_eof || bs->lines_start_poses_len >= n + 1) {
        return bs->lines_start_poses_len - 1;
    }
    size_t new_len = bs->lines_start_poses_len * 1.5;
    if (new_len < n + 1) {
        new_len = n + 1;
    }
    bs->lines_start_poses = realloc(bs->lines_start_poses,
                                    new_len * sizeof(*bs->lines_start_poses));
    size_t old_len = bs->lines_start_poses_len;
    bs->lines_start_poses_len = new_len;
    for (size_t i = old_len; i < new_len; i++) {
        fsetpos(bs->f.fp, &bs->lines_start_poses[i-1]);
        ssize_t len = get_line_chars_num(bs->f);
        if (len == -1) {
            bs->reached_eof = true;
            bs->lines_start_poses =
                realloc(bs->lines_start_poses,
                        i * sizeof(*bs->lines_start_poses));
            bs->lines_start_poses_len = i;
            return i - 1;
        }
        fgetpos(bs->f.fp, &bs->lines_start_poses[i]);
    }
    return new_len - 1;
}

void free_block_state(BlockState* bs) {
    free(bs->lines_start_poses);
    free(bs->line_buf);
    free(bs);
}

void output_status_line(BlockState* bs, WINDOW* win) {
    // Status line
    wprintw(win,
            "%s %ld:%ld",
            bs->f.name, bs->block_lines_offset, bs->block_lines_start_offset);
}

void output_content(BlockState* bs, const WindowSize ws, WINDOW* win) {
    bs->max_block_line_length = 0;
    // Output file content block
    wchar_t* prefix_buf = malloc((bs->block_lines_start_offset+1) *
                                 sizeof(*prefix_buf));
    for (size_t i = 0; i < bs->lines_num; i++) {
        fsetpos(bs->f.fp, &bs->block_lines_start_poses[i]);
        bool should_read = true;
        if (bs->block_lines_start_offset > 0) {
            assert(fgetws(prefix_buf, bs->block_lines_start_offset + 1, bs->f.fp) != NULL);
            if (wcslen(prefix_buf) < bs->block_lines_start_offset ||
                    prefix_buf[bs->block_lines_start_offset - 1] == '\n') {
                should_read = false;
            }
        }
        if (!should_read) {
            mvwaddwstr(win, i+1, 1, L"\n");
        } else {
            assert(fgetws(bs->line_buf, ws.width-2, bs->f.fp) != NULL);
            int len = wcslen(bs->line_buf);
            if (len > bs->max_block_line_length) {
                bs->max_block_line_length = len;
            }

            mvwaddwstr(win, i+1, 1, bs->line_buf);
        }
    }
}

void move_up(BlockState* bs) {
    if (bs->block_lines_offset > 0) {
        bs->block_lines_offset--;
        bs->block_lines_start_poses = bs->lines_start_poses +
                                      bs->block_lines_offset;
        if (bs->lines_num < bs->max_lines_num) {
            bs->lines_num++;
        }
    }
}

void move_down(BlockState* bs) {
    if (bs->lines_num > 0) {
        bs->block_lines_offset++;
        size_t last_num =
            fetch_n_lines_poses(bs, bs->block_lines_offset + bs->lines_num);
        bs->block_lines_start_poses = bs->lines_start_poses +
                                      bs->block_lines_offset;
        if (bs->lines_num > last_num - bs->block_lines_offset) {
            bs->lines_num = last_num - bs->block_lines_offset;
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
    bs->block_lines_start_poses = bs->lines_start_poses;
    size_t last_num = fetch_n_lines_poses(bs, bs->lines_start_poses_len - 1);
    bs->block_lines_offset = 0;
    bs->block_lines_start_offset = 0;
    bs->lines_num = last_num - bs->block_lines_offset;
    if (bs->lines_num > bs->max_lines_num) {
        bs->lines_num = bs->max_lines_num;
    }
}

void move_to_line_start(BlockState* bs) {
    bs->block_lines_start_offset = 0;
}

void move_page_up(BlockState* bs) {
    for (size_t i = 0; i < bs->max_lines_num; i++) {
        move_up(bs);
    }
}

void move_page_down(BlockState* bs) {
    for (size_t i = 0; i < bs->max_lines_num; i++) {
        move_down(bs);
    }
}
