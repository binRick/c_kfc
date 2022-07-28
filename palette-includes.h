#pragma once
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX inc_palette_
#include "submodules/incbin/incbin.h"
#include <stdio.h>
void __setup_palettes(void)__attribute__((constructor));
struct inc_palette_t {
  int size;
  const char *name;
  const char *file;
  const char *data;
};

