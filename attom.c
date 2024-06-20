#include <stdlib.h>
#include <stdio.h>

#include "gap_buffer.h"
#include "attom_pad.h"
#include "attom_pad_editor.h"

int main() {
  struct GapBuffer* gb = gap_buffer_create();
  struct AttomPad* ap = attom_pad_create(gb);
  struct AttomPadEditor* ape = editor_create(ap);
  editor_loop(ape);
  endwin();
  return EXIT_SUCCESS;
}
