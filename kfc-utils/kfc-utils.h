#pragma once
#include <ctype.h>
#include <libproc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
///////////////////////////////////////////////////////////////////
#include "rgba/src/rgba.h"
//////////////////////////////////////////
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    inc_palette_
#include "submodules/incbin/incbin.h"
///////////////////////////////////////////////////////////////////
const char *kfc_utils_palettes_history_file;
#define ANSI_ESC_CODE                           "\x1b"
#define CODE_PREFIX                             ANSI_ESC_CODE ""
#define CODE_SUFFIX                             ANSI_ESC_CODE "\\"
#define CODES_SUFFIX                            ANSI_ESC_CODE "[21D"
#define KFC_UTILS_RENDERED_PALETTE_COPY_PATH    "kfc-utils/.kfc-utils-palettes.c"
#define KFC_UTILS_PALETTE_TEMPLATE_FILE         "etc/kfc-utils-palettes.c.j2"
#define debug_malloc                            "malloc"
#define debug_free                              "free"
#define debug_strdup                            "strdup"
#define FREE_PALETTE_PROPERTIES(pp)    do { \
    if (pp) {                               \
      if (pp->name) {                       \
        free(pp->name);                     \
      }                                     \
      if (pp->translated_name) {            \
        free(pp->translated_name);          \
      }                                     \
      if (pp->value) {                      \
        free(pp->value);                    \
      }                                     \
      if (pp->translated_value) {           \
        free(pp->translated_value);         \
      }                                     \
      free(pp);                             \
    }                                       \
} while (0)
#define INC_PALETTE_T()     \
  struct inc_palette_t {    \
    const size_t size;      \
    const size_t lines_qty; \
    const size_t keys_qty;  \
    char         *keys_csv; \
    const char   *name;     \
    const char   *file;     \
    const char   *data;     \
    const char   *dir;      \
  };
#define INC_PALETTE_T_DEFINED
#define ___S(s)                        #s
#define __S(s)                         ___S(s)
#define INC_PALETTE_T_STRING()         __S(INC_PALETTE_T())
///////////////////////////////////////////////////////////////////
struct palette_name_translations_t { char *src; char *dst; };
struct palette_code_value_translations_t { char *name; char *src; char *dst; };
struct palette_code_t { char *name; char *code; };
enum background_brightness_type_t {
  BACKGROUND_BRIGHTNESS_BRIGHT,
  BACKGROUND_BRIGHTNESS_DARK,
  BACKGROUND_BRIGHTNESS_QTY,
};
enum terminal_types_t {
  TERMINAL_TYPE_KITTY,
  TERMINAL_TYPE_ALACRITTY,
  TERMINAL_TYPE_ITERM2,
  TERMINAL_TYPE_TERMINAL,
  TERMINAL_TYPES_QTY,
};
struct rendered_template_result_t {
  size_t        qty;
  size_t        size;
  unsigned long started, dur;
  char          *path, *content, *template_path;
  size_t        palette_file_lines;
  size_t        palette_file_bytes;
  size_t        palette_file_properties;
};
struct palette_property_color_t {
  bool   is_bright; bool is_dark;
  bool   is_very_bright; bool is_very_dark;
  rgba_t rgba;
  float  brightness;
  char   *brightness_s;
  int    red; int green; int blue;
  char   *red_s; char *green_s; char *blue_s;
  char   *sequence;
  char   *escaped_sequence;
};
struct palette_property_t {
  char                            *name;
  char                            *translated_name;
  char                            *code;
  bool                            is_valid_name;
  char                            *prefix;
  char                            *suffix;
  char                            *escaped_prefix;
  char                            *escaped_suffix;
  char                            *escaped_code;
  char                            *value;
  char                            *escaped_value;
  bool                            is_translated;
  char                            *translated_value;
  char                            *escaped_translated_value;
  char                            *sequence;
  char                            *escaped_sequence;
  bool                            is_color;
  struct palette_property_color_t *color_t;
};
INC_PALETTE_T()
///////////////////////////////////////////////////////////////////
extern const struct inc_palette_t palette_t_list[];
extern const size_t               PALETTES_QTY;
///////////////////////////////////////////////////////////////////
///     general utilities
///////////////////////////////////////////////////////////////////
int kfc_utils_module_test(void);
void __debug_palettes(void);
void __debug_palette_codes(void);
char *kfc_utils_get_exec_path(void);

///////////////////////////////////////////////////////////////////
///     table utilities
///////////////////////////////////////////////////////////////////
char *kfc_utils_get_palettes_table();
char *kfc_utils_get_palette_properties_table(const char *PALETTE_NAME);

///////////////////////////////////////////////////////////////////
///     palette utilities
///////////////////////////////////////////////////////////////////
struct Vector *kfc_utils_get_palettes_v();
struct Vector *kfc_utils_get_palette_names_v();
struct inc_palette_t *kfc_utils_get_palette_t_by_name(const char *PALETTE_NAME);
struct inc_palette_t *kfc_utils_get_palette_t_by_index(const size_t INDEX);
size_t kfc_utils_random_dark_palette_index(float MAX_DARKNESS_VALUE);
char *kfc_utils_get_palette_name_property_value(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME);
size_t kfc_utils_get_palettes_data_bytes();
size_t kfc_utils_load_palette_name(const char *PALETTE_NAME);
size_t kfc_utils_random_palette_index();
char *kfc_utils_get_palette_name_by_index(const int INDEX);
struct Vector *kfc_utils_select_palettes(void);
char *kfc_utils_select_palette(void *CTX);
char *kfc_utils_select_apply_palette(void);
int kfc_utils_color_report(void);
char *kfc_utils_get_palette_name_sequence(const char *PALETTE_NAME);
char *kfc_utils_get_palette_name_data(const char *PALETTE_NAME);
char *kfc_utils_get_ansi_reset_sequence();
char *kfc_utils_get_palette_history();
struct rendered_template_result_t *kfc_utils_get_rendered_template(void);
char *kfc_utils_get_cwd(void);
///////////////////////////////////////////////////////////////////
///     palette property utilities
///////////////////////////////////////////////////////////////////
struct Vector *kfc_utils_get_palette_name_properties_v(const char *PALETTE_NAME);
struct Vector *kfc_utils_get_unique_palette_property_names();
struct Vector *kfc_utils_get_unique_palette_property_names();
struct Vector *kfc_utils_get_invalid_palette_property_names();
struct Vector *kfc_utils_get_palette_names_by_brightness_type(int BACKGROUND_BRIGHTNESS_TYPE, float BRIGHTNESS_THRESHOLD);
struct Vector *kfc_utils_get_palette_property_color_names_v();
char *kfc_utils_get_palette_colored_properties(const char *PALETTE_NAME);
char *kfc_utils_get_palette_item_code(const char *PALETTE_ITEM_NAME);
char *kfc_utils_get_palette_colors_table(const char *PALETTE_NAME);
bool kfc_utils_palette_item_name_is_translated(const char *ITEM_NAME);
char *kfc_utils_translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE);
bool kfc_utils_palette_background_is_brightness_type(char *BACKGROUND_COLOR, int BACKGROUND_BRIGHTNESS_TYPE, double BRIGHTNESS_THRESHOLD);
bool kfc_utils_palette_property_is_color(const char *PALETTE_PROPERTY_NAME);

///////////////////////////////////////////////////////////////////
///     terminal utilities
///////////////////////////////////////////////////////////////////
char *kfc_utils_detect_terminal_type();

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
///////////////////////////////////////////////////////////////////
