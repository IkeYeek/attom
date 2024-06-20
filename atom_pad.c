#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "gap_buffer.h"
#include "atom_pad.h"

struct AttomPad* attom_pad_create() {
  struct AttomPad* ap = calloc(1, sizeof(struct AttomPad));
  if (ap == NULL) {
    fprintf(stderr, "allocation error in attom_pad_create\n");
    exit(EXIT_FAILURE);
  }
  ap->buffer = gap_buffer_create();
  ap->file_name = NULL;
  ap->file_saved = false;
  return ap;
}

struct AttomPad* attom_pad_load_file(char* path) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    fprintf(stderr, "couldn't find file at path \"%s\"\n", path);
    exit(EXIT_FAILURE);
  }
  struct AttomPad* ap = attom_pad_create();
  FILE *f = fopen("textfile.txt", "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

  char *read_buffer = malloc(fsize + 1);
  fread(read_buffer, fsize, 1, f);
  fclose(f);

  read_buffer[fsize] = 0;
  free(ap->buffer);
  ap->buffer = calloc(fsize + DEFAULT_GAP + 1, sizeof(char));
  if (ap->buffer == NULL) {
    fprintf(stderr, "allocation error in attom_pad_load_file\n");
    exit(EXIT_FAILURE);
  }
  memcpy(ap->buffer + DEFAULT_GAP, read_buffer, fsize);
  free(read_buffer);
  return ap;
}

struct AttomPad* attom_pad_save(struct AttomPad* ap) {
  FILE* f = fopen(ap->file_name, "w+");
  if (f == NULL) {
    fprintf(stderr, "couldn't open file at path \"%s\"\n", ap->file_name);
    exit(EXIT_FAILURE);
  }
  char* clean_buffer = calloc(ap->buffer->size - ap->buffer->gap_end - ap->buffer->gap_start, sizeof(char));
  int shift = 0;
  if (ap->buffer->gap_start > 0) {
    memcpy(clean_buffer, ap->buffer, ap->buffer->gap_start);
    shift = ap->buffer->gap_start;
  }
  memcpy(clean_buffer + shift, ap->buffer + ap->buffer->gap_end, ap->buffer->size - ap->buffer->gap_end - ap->buffer->gap_start - shift);
  fwrite(clean_buffer, sizeof(char), ap->ap->buffer->size - ap->buffer->gap_end - ap->buffer->gap_start, f);
  free(clean_buffer);
  fclose(f);
}

struct AttomPad* attom_pad_save_as(struct AttomPad* ap, char* path) {
  ap->file_name = path;
  attom_pad_save(ap);
}

void attom_pad_free(struct AttomPad* ap) {
  gap_buffer_free(ap->buffer);
  free(ap->file_name);
  free(ap);
}

