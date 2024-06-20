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
  if (gb->gap_end == gb->gap_start + 1) {  // si en décalant le gap_start de 1 on atteint le gap end, il faut refaire un gap
    gb->buffer = realloc(gb->buffer, (sizeof(char) * gb->size + DEFAULT_GAP) + 1);
    if (gb->buffer == NULL) {
      fprintf(stderr, "allocation error in gap_buffer_insert\n");
      exit(EXIT_FAILURE);
    }
    memmove(gb->buffer + gb->cursor_pos + DEFAULT_GAP, gb->buffer + gb->cursor_pos, gb->size - gb->cursor_pos);
    gb->size += DEFAULT_GAP;
    gb->gap_start = gb->cursor_pos;
    gb->gap_end = gb->gap_start + DEFAULT_GAP;
  }
  gb->buffer[gb->cursor_pos] = ch;
  gb->cursor_pos += 1;
  gb->gap_start += 1;
}

void gap_buffer_backspace(struct GapBuffer* gb) {
  if (gb->cursor_pos > 0) {
    gb->gap_start -= 1;
    gb->cursor_pos -= 1;
  }
}

void gap_buffer_move_cursor(struct GapBuffer* gb, int direction_vector[2] /* {x, y}, *should* be either -1, 0, or 1 */) {
int target_cursor_position = gb->cursor_pos;

if (direction_vector[0] != 0) {
  target_cursor_position += direction_vector[0];
  if (target_cursor_position >= 0 && target_cursor_position <= gb->size) {
    if (direction_vector[0] < 0) {
      gb->buffer[gb->gap_end] = gb->buffer[gb->cursor_pos - 1];
    } else {
      gb->buffer[gb->gap_start] = gb->buffer[gb->cursor_pos];
    }
    gb->gap_start += direction_vector[0];
    gb->gap_end += direction_vector[0];
    gb->cursor_pos = target_cursor_position;
  }
}

  if (direction_vector[1] != 0) {
    int current_line_start = gb->cursor_pos;
    int current_line_end = gb->cursor_pos;
    int target_line_start, target_line_end;

    while (current_line_start > 0 && gb->buffer[current_line_start - 1] != '\n') {
      current_line_start--;
    }
    while (current_line_end < gb->size && gb->buffer[current_line_end] != '\n') {
      current_line_end++;
    }

    if (direction_vector[1] < 0) {  // Move up
      target_line_end = current_line_start - 1;
      target_line_start = target_line_end;
      while (target_line_start > 0 && gb->buffer[target_line_start - 1] != '\n') {
        target_line_start--;
      }
    } else {  // Move down
      target_line_start = current_line_end + 1;
      target_line_end = target_line_start;
      while (target_line_end < gb->size && gb->buffer[target_line_end] != '\n') {
        target_line_end++;
      }
    }

    int current_column = gb->cursor_pos - current_line_start;
    int target_line_length = target_line_end - target_line_start;
    target_cursor_position = target_line_start + (current_column < target_line_length ? current_column : target_line_length);

    if (target_cursor_position < gb->gap_start) {
      memmove(gb->buffer + target_cursor_position + (gb->gap_end - gb->gap_start),
      gb->buffer + target_cursor_position,
      gb->gap_start - target_cursor_position);
    } else if (target_cursor_position > gb->gap_end) {
      memmove(gb->buffer + gb->gap_start,
      gb->buffer + gb->gap_end,
      target_cursor_position - gb->gap_end);
    }

    int gap_size = gb->gap_end - gb->gap_start;
    gb->gap_start = target_cursor_position;
    gb->gap_end = gb->gap_start + gap_size;
    gb->cursor_pos = target_cursor_position;
  }
}

void gap_buffer_free(struct GapBuffer* gb) {
  free(gb->buffer);
  free(gb);
}

