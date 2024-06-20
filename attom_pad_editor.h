struct AttomPadEditor {
  struct AttomPad* pad;
  WINDOW* editor_pad;
  WINDOW* info_box_container;
  WINDOW* info_box;
  int vertical_shift;
  int horizontal_shift;
}

struct AttomPadEditor* editor_create();
struct AttomPadEditor* editor_load_file(char* path);
void editor_refresh(struct AttomPadEditor* e);
void editor_free(struct AttomPadEditor* e);
