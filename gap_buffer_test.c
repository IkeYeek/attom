#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "gap_buffer.h"

// Helper function to compare gap buffer content with a string
bool gap_buffer_content_equals(struct GapBuffer* gb, const char* str) {
    int str_len = strlen(str);
    int buffer_index = 0;
    int str_index = 0;

    while (buffer_index < gb->size && str_index < str_len) {
        if (buffer_index == gb->gap_start) {
            buffer_index = gb->gap_end;
            continue;
        }
        if (gb->buffer[buffer_index] != str[str_index]) {
            return false;
        }
        buffer_index++;
        str_index++;
    }

    return str_index == str_len && 
           (buffer_index == gb->size || buffer_index == gb->gap_start);
}

void test_gap_buffer_create() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);
    assert(gb->size == DEFAULT_BUFFER_CAP + DEFAULT_GAP);
    assert(gb->gap_start == 0);
    assert(gb->gap_end == DEFAULT_GAP);
    assert(gb->cursor_pos == 0);
    gap_buffer_free(gb);
}

void test_gap_buffer_insert() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);

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
    assert(gap_buffer_content_equals(gb, "ahello, world!"));
    assert(gb->cursor_pos == 14);
    assert(gb->gap_start == 14);

    // Test inserting characters until the buffer needs to be reallocated
    int initial_size = gb->size;
    for (int i = 0; i < DEFAULT_GAP + 5; i++) {
        gap_buffer_insert(gb, 'x');
    }
    assert(gb->size > initial_size);

    gap_buffer_free(gb);
}

void test_gap_buffer_backspace() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);

    // Insert some characters
    char* test_str = "hello";
    for (int i = 0; test_str[i] != '\0'; i++) {
        gap_buffer_insert(gb, test_str[i]);
    }

    // Test backspace
    gap_buffer_backspace(gb);
    assert(gb->cursor_pos == 4);
    assert(gb->gap_start == 4);
    assert(gap_buffer_content_equals(gb, "hell"));

    // Test backspace at the beginning of the buffer
    while (gb->cursor_pos > 0) {
        gap_buffer_backspace(gb);
    }
    gap_buffer_backspace(gb);  // Should do nothing
    assert(gb->cursor_pos == 0);
    assert(gb->gap_start == 0);

    gap_buffer_free(gb);
}

void test_gap_buffer_move_cursor_horizontal() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);

    // Insert some characters
    char* test_str = "hello";
    for (int i = 0; test_str[i] != '\0'; i++) {
        gap_buffer_insert(gb, test_str[i]);
    }

    // Test moving cursor left
    int move_left[] = {-1, 0};
    gap_buffer_move_cursor(gb, move_left);
    assert(gb->cursor_pos == 4);
    assert(gb->gap_start == 4);

    // Test moving cursor right
    int move_right[] = {1, 0};
    gap_buffer_move_cursor(gb, move_right);
    assert(gb->cursor_pos == 5);
    assert(gb->gap_start == 5);


    gap_buffer_free(gb);
}

void test_gap_buffer_move_cursor_vertical() {
    struct GapBuffer* gb = gap_buffer_create();
    assert(gb != NULL);

    // Insert multiple lines
    char* test_str = "first line\nsecond line\nthird line";
    for (int i = 0; test_str[i] != '\0'; i++) {
        gap_buffer_insert(gb, test_str[i]);
    }

    // Move cursor to the middle of the second line
    for (int i = 0; i < 17; i++) {
        int move_left[] = {-1, 0};
        gap_buffer_move_cursor(gb, move_left);
    }

    gap_buffer_free(gb);
}

int main() {
    test_gap_buffer_create();
    test_gap_buffer_insert();
    test_gap_buffer_backspace();
    test_gap_buffer_move_cursor_horizontal();
    test_gap_buffer_move_cursor_vertical();
    printf("All tests passed\n");
    return EXIT_SUCCESS;
}
