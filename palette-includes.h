#pragma once
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    inc_palette_
///////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////
#include "submodules/incbin/incbin.h"
///////////////////////////////////////////////////////////////////
#define ANSI_ESC_CODE    "\x1b"
#define CODE_PREFIX      ANSI_ESC_CODE ""
#define CODE_SUFFIX      ANSI_ESC_CODE "\\"
#define CODES_SUFFIX     ANSI_ESC_CODE "[21D"
///////////////////////////////////////////////////////////////////
struct palette_code_value_translations_t {
  char *name;
  char *src;
  char *dst;
};
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
int random_palette_index();
char *get_palette_name_t_by_index(const int INDEX);
bool palette_item_name_is_translated(const char *ITEM_NAME);
char *translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE);

///////////////////////////////////////////////////////////////////