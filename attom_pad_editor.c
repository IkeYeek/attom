#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

#include "gap_buffer.h"
#include "attom_pad.h"
#include "attom_pad_editor.h"

struct AttomPadEditor* editor_create(struct AttomPad* ap) {
  initscr();
  raw();
  noecho();

  struct AttomPadEditor* ape = calloc(1, sizeof(struct AttomPadEditor));
  if (ape == NULL) {
    fprintf(stderr, "allocation error in editor_create\n");
    exit(EXIT_FAILURE);
  }
  ape->pad = ap;
  ape->editor_pad = newpad(LINES, COLS - 3);
  ape->info_box_container = newwin(3, COLS, 0, 0);
  ape->info_box = subwin(ape->info_box_container, 1, COLS - 2, 1, 1);
  ape->vertical_shift = 0;
  ape->horizontal_shift = 0;
}

void editor_loop(struct AttomPadEditor* e) {
  keypad(e->editor_pad, TRUE);
  bool running = true;
  int ch;
 
  while(running) {
    ch = wgetch(e->editor_pad);

    switch (ch) {
      case KEY_F(1):
        running = false;
      break;
      case KEY_BACKSPACE:
        gap_buffer_backspace(e->pad->buffer);
      break;
      default:
        gap_buffer_insert(e->pad->buffer, ch);
      break;
    }

    editor_refresh(e);
  }
}

struct AttomPadEditor* editor_load_file(char* path);
void editor_refresh(struct AttomPadEditor* e) {
  wclear(e->editor_pad);
  for (int i = 0; i < e->pad->buffer->gap_start; i += 1) {
    waddch(e->editor_pad, e->pad->buffer->buffer[i]);
  }
  for (int i = e->pad->buffer->gap_end; i < e->pad->buffer->size; i += 1) {
    int c = e->pad->buffer->buffer[i];
    if (c != '\0')
      waddch(e->editor_pad, e->pad->buffer->buffer[i]);
    else 
      break;
  }
  prefresh(e->editor_pad, 0, 0, 0, 0, LINES, COLS);
}
void editor_free(struct AttomPadEditor* e);

