#ifndef BLOCK_H_
#define BLOCK_H_
#include "./show.h"
#include "./file_with_name.h"

typedef struct BlockState BlockState;
BlockState* init_block_state(FileWithName, const WindowSize);
void free_block_state(BlockState*);
void output_content(BlockState*, const WindowSize);
void move_up(BlockState*);
void move_down(BlockState*);
void move_left(BlockState*);
void move_right(BlockState*);
void move_to_beginning(BlockState*);
void move_to_line_start(BlockState*);
#endif //BLOCK_H_
