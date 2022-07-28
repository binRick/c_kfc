#pragma once
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    inc_palette_
///////////////////////////////////////////////////////////////////
#include <stdio.h>
///////////////////////////////////////////////////////////////////
#include "submodules/incbin/incbin.h"
///////////////////////////////////////////////////////////////////
#define CODE_PREFIX     "\e"
#define CODE_SUFFIX     "\e\\ "
#define CODES_SUFFIX    "\e[21D"
///////////////////////////////////////////////////////////////////
struct palette_code_t {
  char *name;
  char *code;
};
struct inc_palette_t {
  int        size;
  const char *name;
  const char *file;
  const char *data;
};
///////////////////////////////////////////////////////////////////
void __setup_palettes(void) __attribute__((constructor));
void __debug_palettes(void);
void __debug_palette_codes(void);
struct inc_palette_t *get_palette_t_by_name(const char *PALETTE_NAME);
bool load_palette_name(const char *PALETTE_NAME);

///////////////////////////////////////////////////////////////////
