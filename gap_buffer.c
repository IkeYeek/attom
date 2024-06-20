#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gap_buffer.h"

struct GapBuffer* gap_buffer_create() {
  struct GapBuffer* gb = calloc(1, sizeof(struct GapBuffer));
  if (gb == NULL) {
    fprintf(stderr, "allocation error in gap_buffer_create\n");
    exit(EXIT_FAILURE);
  }
  gb->buffer = calloc(DEFAULT_BUFFER_CAP + DEFAULT_GAP + 1, sizeof(char));
  if (gb->buffer == NULL) {
    fprintf(stderr, "allocation error in gap_buffer_create (2)\n");
    exit(EXIT_FAILURE);
  }
  gb->size = DEFAULT_BUFFER_CAP + DEFAULT_GAP;
  gb->gap_start = 0;
  gb->gap_end = DEFAULT_GAP;
  gb->cursor_pos = 0;
  return gb;
}

void gap_buffer_insert(struct GapBuffer* gb, int ch) {
  if (gb->gap_end == gb->gap_start + 1) {
    gb->buffer = realloc(gb->buffer, (sizeof(char) * gb->size + DEFAULT_GAP) + 1);
    memmove(gb->buffer + gb->cursor_pos + DEFAULT_GAP, gb->buffer + gb->cursor_pos, gb->size - gb->cursor_pos);
    gb->size += DEFAULT_GAP;
    gb->gap_start = gb->cursor_pos;
    gb->gap_end = gb->gap_start + DEFAULT_GAP;
  }
  gb->buffer[gb->cursor_pos] = ch;
  gb->cursor_pos += 1;
  gb->gap_start += 1;
}
void gap_buffer_backspace(struct GapBuffer* gb);
void gap_buffer_move_cursor(struct GapBuffer* gb, int direction_vector[2]);
void gap_buffer_free(struct GapBuffer* gb) {
  free(gb->buffer);
  free(gb);
}

