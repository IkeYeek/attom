#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "gap_buffer.h"


bool buffers_equals(char* buff1, char* buff2, int n) {
  for (int i = 0; i < n; i += 1) {
    if (buff1[i] != buff2[i]) return false;
  }
  return true;
}

void test_gap_buffer_insert() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);

    // Test inserting an empty string
    char* empty_str = "";
    for (int i = 0; empty_str[i] != '\0'; i++) {
        gap_buffer_insert(gb, empty_str[i]);
    }
    assert(gb->cursor_pos == 0);
    assert(gb->gap_start == 0);
    assert(gb->gap_end == DEFAULT_GAP);

    // Test inserting a single character
    gap_buffer_insert(gb, 'a');
    assert(gb->buffer[0] == 'a');
    assert(gb->cursor_pos == 1);
    assert(gb->gap_start == 1);
    assert(gb->gap_end == DEFAULT_GAP);

    // Test inserting multiple characters
    char* test_str = "hello, world!";
    for (int i = 0; test_str[i] != '\0'; i++) {
        gap_buffer_insert(gb, test_str[i]);
    }
    assert(strcmp(gb->buffer, "ahello, world!") == 0);
    assert(gb->cursor_pos == 14);
    assert(gb->gap_start == 14);

    // Test inserting characters until the buffer needs to be reallocated
    char max_char = CHAR_MAX;
    for (int i = 0; i < DEFAULT_GAP + 5; i++) {
        gap_buffer_insert(gb, max_char);
    }
    assert(gb->size > DEFAULT_BUFFER_CAP + DEFAULT_GAP);

    // Test inserting invalid characters (outside the range of char)
    int invalid_char = 256;
    gap_buffer_insert(gb, invalid_char);
    assert(gb->buffer[gb->cursor_pos - 1] == (char)invalid_char);

    gap_buffer_free(gb);
}

int main() {
  test_gap_buffer_insert();
  printf("All tests passed\n");
  return EXIT_SUCCESS;
}
