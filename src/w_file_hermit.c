#include "w_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const byte _binary_doom1_wad_start[];
extern const byte _binary_doom1_wad_end[];

static void Hermit_CloseFile(wad_file_t *file) { free(file); }

static size_t Hermit_Read(wad_file_t *file, unsigned int offset, void *buffer,
                          size_t buffer_len) {
  if (offset >= file->length) {
    return 0;
  }

  size_t available = file->length - offset;
  size_t to_read = (buffer_len < available) ? buffer_len : available;

  memcpy(buffer, file->mapped + offset, to_read);

  return to_read;
}

static wad_file_class_t hermit_wad_file_class = {NULL, Hermit_CloseFile,
                                                 Hermit_Read};

wad_file_t *W_OpenFile(char *path) {
  // HermitOS environment provides a single statically linked WAD.
  // Requests for external IWADs or PWADs are unsupported and must fail.
  if (!strstr(path, "doom1.wad")) {
    return NULL;
  }

  printf("HermitVFS: Mapping embedded WAD (%s)\n", path);

  wad_file_t *file = malloc(sizeof(wad_file_t));
  if (!file) {
    return NULL;
  }

  file->file_class = &hermit_wad_file_class;
  file->mapped = (byte *)_binary_doom1_wad_start;
  file->length =
      (unsigned int)(_binary_doom1_wad_end - _binary_doom1_wad_start);

  return file;
}

void W_CloseFile(wad_file_t *wad) {
  if (wad && wad->file_class && wad->file_class->CloseFile) {
    wad->file_class->CloseFile(wad);
  }
}

size_t W_Read(wad_file_t *wad, unsigned int offset, void *buffer,
              size_t buffer_len) {
  if (wad && wad->file_class && wad->file_class->Read) {
    return wad->file_class->Read(wad, offset, buffer, buffer_len);
  }
  return 0;
}
