#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H
#define DEFAULT_BUFFER_CAP 16
#define DEFAULT_GAP 8 
/*
 * buffer:
 * abcdefghi|[__________]klmnopqrstuvwxyz
 *          ^
 *         cursor
 *           ^          ^
 *      gap_start   gap_end
 *[_____________________________________] <- size
 * text size = size - (gap_end - gap_start)
 *
 * when inserting a character, we just increment the gap_start
 * when deleting a character, we just decrement the gap_start
 * when moving
 */
struct GapBuffer {
  char* buffer;
  int size;
  int gap_start;
  int gap_end;
  int cursor_pos;
};

struct GapBuffer* gap_buffer_create();
void gap_buffer_insert(struct GapBuffer* gb, int ch);
void gap_buffer_backspace(struct GapBuffer* gb);
void gap_buffer_move_cursor(struct GapBuffer* gb, int direction_vector[2]);
void gap_buffer_free(struct GapBuffer* gb);
#endif
