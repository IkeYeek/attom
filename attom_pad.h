#include <stdbool.h>

struct AttomPad {
  struct GapBuffer* buffer;
  char* file_name;
  bool file_saved;
};

struct AttomPad* attom_pad_create();
struct AttomPad* attom_pad_load_file(char* path);
struct AttomPad* attom_pad_save(struct AttomPad* ap, char* path);
struct AttomPad* attom_pad_save_as(struct AttomPad* ap, char* path);
void attom_pad_free(struct AttomPad* ap);
