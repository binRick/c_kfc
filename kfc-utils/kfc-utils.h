#pragma once
#include <ctype.h>
#include <libproc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
//////////////////////////////////////////
#pragma once
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    inc_palette_
///////////////////////////////////////////////////////////////////
#include "submodules/incbin/incbin.h"
///////////////////////////////////////////////////////////////////
#define ANSI_ESC_CODE    "\x1b"
#define CODE_PREFIX      ANSI_ESC_CODE ""
#define CODE_SUFFIX      ANSI_ESC_CODE "\\"
#define CODES_SUFFIX     ANSI_ESC_CODE "[21D"
///////////////////////////////////////////////////////////////////
struct palette_name_translations_t {
  char *src;
  char *dst;
};
struct palette_code_value_translations_t {
  char *name;
  char *src;
  char *dst;
};
struct palette_property_t {
  char *name;
  char *translated_name;
  char *code;
  bool is_valid_name;
  char *prefix;
  char *suffix;
  char *escaped_prefix;
  char *escaped_suffix;
  char *escaped_code;
  char *value;
  char *escaped_value;
  bool is_translated;
  char *translated_value;
  char *escaped_translated_value;
  char *sequence;
  char *escaped_sequence;
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
extern struct inc_palette_t palette_t_list[];
const size_t                PALETTES_QTY;
///////////////////////////////////////////////////////////////////
int kfc_utils_module_test(void);
void __setup_palettes(void) __attribute__((constructor));
void __debug_palettes(void);
void __debug_palette_codes(void);
struct inc_palette_t *get_palette_t_by_name(const char *PALETTE_NAME);
struct inc_palette_t *get_palette_t_by_index(const size_t INDEX);
struct Vector *get_palettes_v();
struct Vector *get_palette_names_v();
struct Vector *get_palette_name_properties_v(const char *PALETTE_NAME);
char *get_palette_properties_table(const char *PALETTE_NAME);
char *get_palette_item_code(const char *PALETTE_ITEM_NAME);
char *get_palettes_table();
size_t get_palettes_data_bytes();
bool load_palette_name(const char *PALETTE_NAME);
size_t random_palette_index();
char *get_palette_name_by_index(const int INDEX);
bool palette_item_name_is_translated(const char *ITEM_NAME);
char *translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE);

///////////////////////////////////////////////////////////////////
#define PALETTE    "\
\033[48;5;0m  \033[0m\
\033[48;5;1m  \033[0m\
\033[48;5;2m  \033[0m\
\033[48;5;3m  \033[0m\
\033[48;5;4m  \033[0m\
\033[48;5;5m  \033[0m\
\033[48;5;6m  \033[0m\
\033[48;5;7m  \033[0m\n\
\033[48;5;8m  \033[0m\
\033[48;5;9m  \033[0m\
\033[48;5;10m  \033[0m\
\033[48;5;11m  \033[0m\
\033[48;5;12m  \033[0m\
\033[48;5;13m  \033[0m\
\033[48;5;14m  \033[0m\
\033[48;5;15m  \033[0m\
\n"
