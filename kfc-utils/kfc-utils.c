#pragma once
/////////////////////////////////////
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
/////////////////////////////////////
#define DEBUG_PALETTES                    false
#define DEBUG_PALETTE_CODES               false
#define PALETTES_QTY_LIMIT                999
#ifndef __PALETTES_HASH__
#define PALETTES_HASH                     ""
#else
#define PALETTES_HASH                     __PALETTES_HASH__
#endif
#define FZF_UTILS_DEFAULT_HISTORY_FILE    "/tmp/fzf-utils-history.txt"
/////////////////////////////////////
#include "kfc-utils/kfc-utils-data.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
/////////////////////////////////////
#include "kitty/kitty.h"
#include "process/process.h"
/////////////////////////////////////
#include "bench/bench.h"
#include "bytes/bytes.h"
#include "c_ansi/ansi-utils/ansi-utils.h"
#include "c_fsio/include/fsio.h"
#include "c_fsio/include/fsio.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_stringfn/include/stringfn.h"
#include "c_vector/include/vector.h"
#include "debug-memory/debug_memory.h"
#include "djbhash/src/djbhash.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "libfort/src/fort.h"
#include "rgba/src/rgba.h"
#include "str-truncate.c/src/str-truncate.h"
#include "submodules/b64.c/b64.h"
#include "tempdir.c/tempdir.h"
#include "termpaint.h"
#include "termpaintx.h"
#include "timestamp/timestamp.h"
#include "which/src/which.h"
/////////////////////////////////////
static char *get_palette_item_sequence(const struct palette_property_t *pp);
char *get_palette_name_sequence(const char *PALETTE_NAME);
static bool is_valid_palette_item_name(const char *PALETTE_ITEM_NAME);
static char *get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME);
static void __kfc_utils_constructor(void) __attribute__((constructor));
static void __kfc_utils_destructor(void) __attribute__((destructor));
static char *get_cache_ymd();
static char *get_palette_history_file();

/////////////////////////////////////
static bool                KFC_UTILS_DEBUG_MODE = false;
static char                *PALETTES_CACHE_FILE = NULL;
static module(kfc_utils) * KFC;
static struct djbhash      palette_properties_h, valid_palette_property_names_h, invalid_palette_property_names_h;
static struct djbhash_node *hash_item;
extern const char          *EXECUTABLE_ABSOLUTE;
enum palette_cache_items_t { PALETTES_TABLE,
                             PALETTES_CACHE_QTY,
};
static struct kfc_utils_cache_files_t { char *name; char *path; }             PALETTE_CACHE_FILES[] = {
  [PALETTES_TABLE] = { .name = "palettes-table-cache", },
                     { 0 },
};
static struct terminal_type_names_t { const char *name, *env_key, *env_val; } terminal_type_names[] = {
  [TERMINAL_TYPE_KITTY]     = { .name = "kitty",     .env_key = "KITTY_PID",        .env_val = NULL,             },
  [TERMINAL_TYPE_ALACRITTY] = { .name = "alacritty", .env_key = "ALACRITTY_SOCKET", .env_val = NULL,             },
  [TERMINAL_TYPE_ITERM2]    = { .name = "iterm2",    .env_key = "TERM_PROGRAM",     .env_val = "iTerm.app",      },
  [TERMINAL_TYPE_TERMINAL]  = { .name = "terminal",  .env_key = "TERM_PROGRAM",     .env_val = "Apple_Terminal", },
                              { 0 },
};
static struct palette_name_translations_t                                     palette_name_translations[] = {
  { .src = "color0",      .dst = "color00", },
  { .src = "color1",      .dst = "color01", },
  { .src = "color2",      .dst = "color02", },
  { .src = "color3",      .dst = "color03", },
  { .src = "color4",      .dst = "color04", },
  { .src = "color5",      .dst = "color05", },
  { .src = "color6",      .dst = "color06", },
  { .src = "color7",      .dst = "color07", },
  { .src = "color8",      .dst = "color08", },
  { .src = "color9",      .dst = "color09", },
  { .src = "cursorcolor", .dst = "cursor",  },
  { 0 },
};
static struct palette_code_value_translations_t                               palette_code_value_translations[] = {
  { .name = "cursorstyle",   .src = "under",     .dst = "3 q", },
  { .name = "cursorstyle",   .src = "block",     .dst = "1 q", },
  { .name = "cursorstyle",   .src = "bar",       .dst = "5 q", },
  { .name = "cursorblink",   .src = "off",       .dst = "l",   },
  { .name = "cursorblink",   .src = "on",        .dst = "h",   },
  { .name = "altscreen",     .src = "off",       .dst = "l",   },
  { .name = "altscreen",     .src = "on",        .dst = "h",   },
  { .name = "screen",        .src = "off",       .dst = "l",   },
  { .name = "screen",        .src = "on",        .dst = "h",   },
  { .name = "cursorvisible", .src = "off",       .dst = "l",   },
  { .name = "cursorvisible", .src = "on",        .dst = "h",   },
  { .name = "reportfocus",   .src = "off",       .dst = "l",   },
  { .name = "reportfocus",   .src = "on",        .dst = "h",   },
  { .name = "mouse",         .src = "clickdrag", .dst = "2h",  },
  { .name = "mouse",         .src = "motion",    .dst = "3h",  },
  { .name = "mouse",         .src = "click",     .dst = "0h",  },
  { .name = "mouse",         .src = "off",       .dst = "0l",  },
  { .name = "reset",         .src = "yes",       .dst = "c",   },
  { .name = "reset",         .src = "no",        .dst = "",    },
  { 0 },
};
static struct palette_code_t                                                  palette_codes[] = {
  { .name = "color00",              .code = "]4;0;#",              },
  { .name = "color01",              .code = "]4;1;#",              },
  { .name = "color02",              .code = "]4;2;#",              },
  { .name = "color03",              .code = "]4;3;#",              },
  { .name = "color04",              .code = "]4;4;#",              },
  { .name = "color05",              .code = "]4;5;#",              },
  { .name = "color06",              .code = "]4;6;#",              },
  { .name = "color07",              .code = "]4;7;#",              },
  { .name = "color08",              .code = "]4;8;#",              },
  { .name = "color09",              .code = "]4;9;#",              },
  { .name = "color10",              .code = "]4;10;#",             },
  { .name = "color11",              .code = "]4;11;#",             },
  { .name = "color12",              .code = "]4;12;#",             },
  { .name = "color13",              .code = "]4;13;#",             },
  { .name = "color14",              .code = "]4;14;#",             },
  { .name = "color15",              .code = "]4;15;#",             },
  { .name = "color16",              .code = "]4;16;#",             },
  { .name = "color17",              .code = "]4;17;#",             },
  { .name = "color18",              .code = "]4;18;#",             },
  { .name = "color19",              .code = "]4;19;#",             },
  { .name = "color20",              .code = "]4;20;#",             },
  { .name = "color21",              .code = "]4;21;#",             },
  { .name = "color22",              .code = "]4;22;#",             },
  { .name = "color23",              .code = "]4;23;#",             },
  { .name = "color24",              .code = "]4;24;#",             },
  { .name = "color25",              .code = "]4;25;#",             },
  { .name = "foreground",           .code = "]10;#",               },
  { .name = "background",           .code = "]11;#",               },
  { .name = "selection_background", .code = "]17;#",               },
  { .name = "selection_foreground", .code = "]19;#",               },
  { .name = "title",                .code = "]2;",                 },
  { .name = "cursor",               .code = "]12;#",               },
  { .name = "cursorvisible",        .code = "[?25",                },
  { .name = "cursorstyle",          .code = "[",                   },
  { .name = "cursorblink",          .code = "[?12",                },
  { .name = "altscreen",            .code = "[?47",                },
  { .name = "screen",               .code = "[?5",                 },
  { .name = "icontitle",            .code = "]0;",                 },
  { .name = "icon",                 .code = "]1;",                 },
  { .name = "title",                .code = "]2;",                 },
  { .name = "reportfocus",          .code = "[?1004",              },
  { .name = "mouse",                .code = "[?100",               },
  { .name = "notification",         .code = "]99;i=1:d=1:p=body;", },
  { .name = "reset",                .code = "",                    },
  { 0 },
};

static void __attribute__((constructor)) __kfc_utils_constructor(){
  if (getenv("DEBUG_MODE") != NULL) {
    KFC_UTILS_DEBUG_MODE = true;
  }


  djbhash_init(&palette_properties_h);
  djbhash_init(&valid_palette_property_names_h);
  djbhash_init(&invalid_palette_property_names_h);
  KFC       = require(kfc_utils);
  KFC->mode = KFC_LOG_DEBUG;

  if (KFC_UTILS_DEBUG_MODE == true) {
    log_debug("<%d> [%s] palettes qty:%lu",
              getpid(),
              __FUNCTION__,
              KFC->get_palettes_qty()
              );
  }
  if (strcmp(PALETTES_HASH, "") != 0) {
    char *ymd = get_cache_ymd();
    for (size_t i = 0; i < PALETTES_CACHE_QTY; i++) {
      asprintf(&PALETTE_CACHE_FILES[i].path,
               "%s%s-%s-%s.txt",
               gettempdir(),
               PALETTE_CACHE_FILES[i].name,
               ymd,
               PALETTES_HASH
               );
      if (KFC_UTILS_DEBUG_MODE == true) {
        log_debug("palettes table hash %s|%s", PALETTES_HASH, PALETTE_CACHE_FILES[PALETTES_TABLE].path);
      }
    }
  }
}

static void __attribute__((destructor)) __kfc__utils_destructor(){
  clib_module_free(KFC);
  djbhash_destroy(&invalid_palette_property_names_h);
  djbhash_destroy(&valid_palette_property_names_h);
  djbhash_destroy(&palette_properties_h);
}


char *get_palette_properties_table(const char *PALETTE_NAME){
  struct palette_property_t *palette_properties_v = get_palette_name_properties_v(PALETTE_NAME);

  if (vector_size(palette_properties_v) < 1) {
    return("No properties found");
  }
  ft_table_t *table = ft_create_table();

  ft_set_border_style(table, FT_SOLID_ROUND_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);

  ft_write_ln(table,
              "Property",
              "Value",
              "Valid Prop",
              "Translated Prop",
              "Value is Translated",
              "Prefix",
              "Code",
              "Translated Value",
              "Suffix",
              "Sequence"
              );

  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    struct palette_property_t *pp = vector_get(palette_properties_v, i);
    ft_printf_ln(table,
                 "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s",
                 pp->name,
                 pp->value,
                 pp->is_valid_name ? "Yes" : "No",
                 pp->translated_name,
                 pp->is_translated ? "Yes" : "No",
                 pp->escaped_prefix,
                 pp->escaped_code,
                 pp->escaped_translated_value,
                 pp->escaped_suffix,
                 pp->escaped_sequence
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_MAGENTA);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, pp->is_valid_name ? FT_COLOR_GREEN : FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_BLUE);
    ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, pp->is_translated ? FT_COLOR_GREEN : FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 5, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
    ft_set_cell_prop(table, i + 1, 6, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_RED);
    ft_set_cell_prop(table, i + 1, 7, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 7, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 8, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 8, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 9, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
    ft_set_cell_prop(table, i + 1, 9, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
    ft_set_cell_prop(table, i + 1, 10, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 10, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    FREE_PALETTE_PROPERTIES(pp);
  }
  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  char *seq_summary, *seq_title;

  asprintf(&seq_title,
           AC_RESETALL "Palette"
           AC_RESETALL " "
           AC_RESETALL AC_CYAN AC_BOLD "%s"
           AC_RESETALL " ",
           PALETTE_NAME
           );
  struct StringBuffer *sb = stringbuffer_new();

  stringbuffer_append_string(sb, seq_title);
  stringbuffer_append_string(sb, "\n");
  stringbuffer_append_string(sb, table_s);
  char *seq = get_palette_name_sequence(PALETTE_NAME);

  asprintf(&seq_summary,
           AC_RESETALL AC_YELLOW AC_BOLD "%s"
           AC_RESETALL " Sequence from "
           AC_RESETALL AC_BLUE AC_REVERSED "%lu"
           AC_RESETALL " properties: "
           AC_RESETALL AC_YELLOW AC_ITALIC AC_REVERSED "%s" AC_RESETALL,
           bytes_to_string(strlen(seq)),
           vector_size(palette_properties_v),
           strdup_escaped(seq)
           );
  stringbuffer_append_string(sb, seq_summary);
  stringbuffer_append_string(sb, "\n");
  char *table_seq_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  vector_release(palette_properties_v);
  return(table_seq_s);
} /* get_palette_properties_table */


struct Vector *get_palette_names_by_brightness_type(int BACKGROUND_BRIGHTNESS_TYPE, float BRIGHTNESS_THRESHOLD){
  struct Vector             *v               = vector_new();
  struct Vector             *palette_names_v = get_palette_names_v();
  struct Vector             *palette_properties_v;
  struct palette_property_t *pp;

  for (size_t ii = 0; ii < vector_size(palette_names_v); ii++) {
    char *pname = (char *)vector_get(palette_names_v, ii);
    palette_properties_v = get_palette_name_properties_v(pname);
    bool found = false;
    for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
      pp = vector_get(palette_properties_v, i);
      if (found == false && strcmp("background", pp->name) == 0) {
        switch (BACKGROUND_BRIGHTNESS_TYPE) {
        case BACKGROUND_BRIGHTNESS_DARK:
          if (palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, BRIGHTNESS_THRESHOLD)) {
            vector_push(v, strdup(pname));
            found = true;
          }
          break;
        case BACKGROUND_BRIGHTNESS_BRIGHT:
          if (palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, BRIGHTNESS_THRESHOLD)) {
            vector_push(v, strdup(pname));
            found = true;
          }
          break;
        default:
          log_error("Invalid brightness type");
          break;
        }
      }
      FREE_PALETTE_PROPERTIES(pp);
    }
  }
  return(v);
}


float get_color_brightness(const char *COLOR){
  char   *hex = NULL;
  double hsluv[3];

  if (false == stringfn_starts_with(COLOR, "#")) {
    asprintf(&hex, "#%s", COLOR);
  }else{
    asprintf(&hex, "%s", COLOR);
  }

  short   ok;
  int32_t val   = rgba_from_string(hex, &ok);
  rgba_t  _rgba = rgba_new(val);

  rgb2hsluv(_rgba.r, _rgba.g, _rgba.b, &hsluv[0], &hsluv[2], &hsluv[2]);
  return(hsluv[2]);
}


bool palette_background_is_brightness_type(char *BACKGROUND_COLOR, int BACKGROUND_BRIGHTNESS_TYPE, double BRIGHTNESS_THRESHOLD){
  char   *hex = NULL;
  double hsluv[3];

  if (false == stringfn_starts_with(BACKGROUND_COLOR, "#")) {
    asprintf(&hex, "#%s", BACKGROUND_COLOR);
  }else{
    asprintf(&hex, "%s", BACKGROUND_COLOR);
  }

  short   ok;
  int32_t val   = rgba_from_string(hex, &ok);
  rgba_t  _rgba = rgba_new(val);

  rgb2hsluv(_rgba.r, _rgba.g, _rgba.b, &hsluv[0], &hsluv[2], &hsluv[2]);
  if (false) {
    log_info("[rgb2hsluv] <type:%d> %s [r:%f|g:%f|b:%f-]-> hue: %f, saturation: %f, lightness: %f|threshold:%f|",
             BACKGROUND_BRIGHTNESS_TYPE,
             hex,
             _rgba.r, _rgba.g, _rgba.b,
             hsluv[0], hsluv[2], hsluv[2], BRIGHTNESS_THRESHOLD);
  }
  switch (BACKGROUND_BRIGHTNESS_TYPE) {
  case BACKGROUND_BRIGHTNESS_DARK:
    return((hsluv[2] <= BRIGHTNESS_THRESHOLD) ? true : false);

    break;
  case BACKGROUND_BRIGHTNESS_BRIGHT:
    return((hsluv[2] >= BRIGHTNESS_THRESHOLD) ? true : false);

    break;
  default:
    log_error("Invalid brightness type");
    return(false);

    break;
  }
  return(false);
} /* palette_background_is_brightness_type */


char *get_palettes_table() {
  char *cache_file = PALETTE_CACHE_FILES[PALETTES_TABLE].path;

  if (cache_file != NULL && strcmp(PALETTES_HASH, "") != 0) {
    if (fsio_file_exists(cache_file)) {
      size_t cache_file_size = fsio_file_size(cache_file);
      if (cache_file_size > 0) {
        char *cache_file_content = fsio_read_text_file(cache_file);
        if (cache_file_content != NULL) {
          if (KFC_UTILS_DEBUG_MODE) {
            log_info("<%d> [%s] returning %s cached table from cache file: %s",
                     getpid(), __FUNCTION__,
                     bytes_to_string(cache_file_size),
                     cache_file);
          }
          return(cache_file_content);
        }
      }
    }
  }
  unsigned long started_ts = timestamp();
  ft_table_t    *table     = ft_create_table();

  ft_set_border_style(table, FT_SOLID_ROUND_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);

  ft_write_ln(table,
              "Palette",
              "Size",
              "Source",
              "# Props",
              "BG Brightness"
              );

  for (size_t i = 0; i < vector_size(KFC->palettes_v) && i < PALETTES_QTY_LIMIT; i++) {
    struct inc_palette_t *p = vector_get(require(kfc_utils)->palettes_v, i);
    struct Vector        *pp = get_palette_name_properties_v(p->name);
    float                brightness = 0;
    bool                 is_dark = false, is_very_dark = false, is_bright = false, is_very_bright = false;
    for (size_t i = 0; i < vector_size(pp); i++) {
      struct palette_property_t *_pp = vector_get(pp, i);
      if (strcmp(_pp->name, "background") == 0) {
        brightness     = get_color_brightness(_pp->translated_value);
        is_dark        = palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, 20);
        is_very_dark   = palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, 5);
        is_bright      = palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, 80);
        is_very_bright = palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, 95);
      }
      FREE_PALETTE_PROPERTIES(_pp);
    }

    struct StringFNStrings sp = stringfn_split(p->file, '/');
    ft_printf_ln(table,
                 "%s|%s|%s|%lu|%5.2f  %s",
                 p->name,
                 bytes_to_string(p->size),
                 str_truncate(stringfn_join(sp.strings, "/", 1, sp.count - 2), 25),
                 vector_size(pp),
                 brightness,
                 is_very_dark ? "Very Dark"
                   : is_dark ? "Dark"
                   : is_very_bright ? "Very Bright"
                   : "Bright"
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLACK);
    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_BG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_INVERTED);
    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);

    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, p->size > 400 ? FT_COLOR_RED
                                                                : p->size < 290 ? FT_COLOR_YELLOW
                                                                : FT_COLOR_GREEN);

    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_DIM);
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_MAGENTA);

    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, vector_size(pp) > 19 ? FT_COLOR_RED
                                                                : vector_size(pp) < 19 ? FT_COLOR_YELLOW
                                                                : FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, is_very_dark ? FT_COLOR_BLUE
                                                                : is_dark ? FT_COLOR_YELLOW
                                                                : is_very_bright ? FT_COLOR_RED
                                                                : FT_COLOR_MAGENTA);


    stringfn_release_strings_struct(sp);
    vector_release(pp);
  }

  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  unsigned long ended_ts = timestamp();

  log_debug("palettes table dur:%lu", ended_ts - started_ts);
  if (cache_file != NULL && strcmp(PALETTES_HASH, "") != 0) {
    fsio_write_text_file(cache_file, table_s);
  }
  return(table_s);
} /* get_palettes_table */


char *get_palette_name_data(const char *PALETTE_NAME){
  struct inc_palette_t *p = get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    log_error("No properties for palette %s found", PALETTE_NAME);
    return(NULL);
  }
  return(p->data);
}
struct Vector *get_palette_name_properties_v(const char *PALETTE_NAME){
  struct Vector        *v = vector_new();
  struct inc_palette_t *p = get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    log_error("No properties for palette %s found", PALETTE_NAME);
    return(v);
  }
  struct StringFNStrings lines = stringfn_split_lines_and_trim(p->data);

  for (size_t i = 0; i < lines.count; i++) {
    struct StringFNStrings items = stringfn_split(lines.strings[i], '=');
    if (items.count < 2) {
      continue;
    }
    if (strlen(items.strings[0]) == 0) {
      continue;
    }
    struct palette_property_t *pp = malloc(sizeof(struct palette_property_t));
    pp->name             = strdup(items.strings[0]);
    pp->value            = strdup(items.strings[1]);
    pp->escaped_value    = strdup_escaped(pp->value);
    pp->is_valid_name    = is_valid_palette_item_name(pp->name);
    pp->is_translated    = palette_item_name_is_translated(pp->name);
    pp->translated_name  = (pp->is_valid_name == true) ? strdup(pp->name) : get_translated_palette_property_name(pp->name);
    pp->translated_value = (pp->is_translated == true) ? translate_palette_item_value(pp->translated_name, pp->value) : strdup(pp->value);
    pp->code             = get_palette_item_code(pp->translated_name);
    pp->sequence         = get_palette_item_sequence(pp);
    pp->suffix           = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_SUFFIX) : "";
    pp->prefix           = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_PREFIX) : "";

    if (strlen(pp->translated_value) == 0) {
      pp->sequence = "";
    }
    {
      pp->escaped_code             = pp->code ? strdup_escaped(pp->code) : "";
      pp->escaped_sequence         = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(pp->sequence) : "";
      pp->escaped_prefix           = strdup_escaped(pp->prefix);
      pp->escaped_suffix           = strdup_escaped(pp->suffix);
      pp->escaped_translated_value = strdup_escaped(pp->translated_value);
    }

    vector_push(v, pp);
    stringfn_release_strings_struct(items);
  }
  return(v);
} /* get_palette_name_properties_v */


size_t get_palettes_data_bytes(){
  size_t               s    = 0;
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    s += (size_t)strlen(tmp->data);
  }
  return(s);
}

struct Vector *get_palette_names_v(){
  struct Vector        *v   = vector_new();
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    vector_push(v, (char *)tmp->name);
  }
  return(v);
}

struct Vector *get_palettes_v(){
  struct Vector        *v   = vector_new();
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL && i < PALETTES_QTY_LIMIT; tmp++, i++) {
    vector_push(v, (void *)tmp);
  }
  return(v);
}


int kfc_utils_module_test(void) {
  KFC->mode = KFC_LOG_DEBUG;
  printf("palettes vector qty:%lu\n", vector_size(KFC->palettes_v));
  printf("palette names vector qty:%lu\n", vector_size(KFC->palette_names_v));
  printf("palettes data bytes:%s\n", bytes_to_string(KFC->palettes_data_bytes));
  printf("palettes table:\n%s\n", KFC->get_palettes_table());
  for (size_t i = 0; i < 2; i++) {
    struct inc_palette_t *p = KFC->get_palette_t_by_index(i);
    printf("palette #%lu> %s\n%s\n", i, p->name, get_palette_properties_table(p->name));
  }
  printf("palette #%s\n%s\n", "vscode", KFC->get_palette_properties_table("vscode"));

  size_t i     = KFC->random_palette_index();
  char   *name = KFC->get_palette_name_by_index(i);
  printf("random palette index:%lu> |%s|%s|%s|\n",
         i,
         name,
         (KFC->get_palette_t_by_index(i))->name,
         KFC->get_palette_t_by_name(name)->name
         );
  if (name) {
    free(name);
  }

  clib_module_free(KFC);
  return(0);
}


/////////////////////////////////////////////////////////////////////


static char *get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME){
  struct palette_name_translations_t *tmp = palette_name_translations;

  for (size_t i = 0; tmp->src != NULL && tmp->dst != NULL && tmp != NULL; tmp++, i++) {
    if (strcmp(PALETTE_PROPERTY_NAME, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }
  return(strdup(PALETTE_PROPERTY_NAME));
}


bool palette_item_name_is_translated(const char *ITEM_NAME){
  struct palette_code_value_translations_t *tmp = palette_code_value_translations;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0) {
      return(true);
    }
  }
  return(false);
}


char *translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE){
  if (false == palette_item_name_is_translated(ITEM_NAME)) {
    return(strdup(ITEM_VALUE));
  }

  struct palette_code_value_translations_t *tmp = palette_code_value_translations;
  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0 && strcmp(ITEM_VALUE, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }

  return(strdup(ITEM_VALUE));
}


size_t random_palette_index(){
  srand(time(NULL) + getpid());
  return((size_t)(rand() % (PALETTES_QTY)));
}


char *get_palette_name_by_index(const int INDEX){
  return(palette_t_list[INDEX].name);
}

struct inc_palette_t *get_palette_t_by_index(const size_t INDEX){
  return(&(palette_t_list[INDEX]));
}

struct inc_palette_t *get_palette_t_by_name(const char *PALETTE_NAME){
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    if (strcmp(PALETTE_NAME, tmp->name) == 0) {
      return(tmp);
    }
  }
  return(NULL);
}


static bool is_valid_palette_item_name(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(true);
    }
  }

  return(false);
}


char *get_palette_name_property_value(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME){
  struct Vector *palette_properties = get_palette_name_properties_v(PALETTE_NAME);

  for (size_t i = 0; i < vector_size(palette_properties); i++) {
    struct palette_property_t *pp = vector_get(palette_properties, i);
    if (strcmp(PALETTE_PROPERTY_NAME, pp->name) == 0) {
      char *s = strdup(pp->value);
      vector_release(palette_properties);
      free(pp->name);
      free(pp->translated_name);
      free(pp->value);
      free(pp->translated_value);
      free(pp);
      return(s);
    }
    FREE_PALETTE_PROPERTIES(pp);
  }
  vector_release(palette_properties);
  return(NULL);
}


static char *get_palette_item_sequence(const struct palette_property_t *pp){
  struct StringBuffer *palette_codes_b = stringbuffer_new();

  stringbuffer_append_string(palette_codes_b, CODE_PREFIX);
  stringbuffer_append_string(palette_codes_b, pp->code);
  stringbuffer_append_string(palette_codes_b, pp->translated_value);
  stringbuffer_append_string(palette_codes_b, CODE_SUFFIX);
  char *palette_code = stringbuffer_to_string(palette_codes_b);

  stringbuffer_release(palette_codes_b);
  return(palette_code);
}


char *get_palette_item_code(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(tmp->code);
    }
  }

  return(NULL);
}


char *get_palette_name_sequence(const char *PALETTE_NAME){
  struct StringBuffer *sb = stringbuffer_new();
  struct Vector       *pp = get_palette_name_properties_v(PALETTE_NAME);

  for (size_t i = 0; i < vector_size(pp); i++) {
    struct palette_property_t *p = vector_get(pp, i);
    stringbuffer_append_string(sb, CODE_PREFIX);
    stringbuffer_append_string(sb, p->code);
    stringbuffer_append_string(sb, p->translated_value);
    stringbuffer_append_string(sb, CODE_SUFFIX);
    FREE_PALETTE_PROPERTIES(p);
  }
  stringbuffer_append_string(sb, CODES_SUFFIX);
  char *seq_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  return(seq_s);
}


size_t load_palette_name(const char *PALETTE_NAME){
  char   *seq = get_palette_name_sequence(PALETTE_NAME);
  size_t qty  = fprintf(stdout, "%s", seq);

  fflush(stdout);
  char *f = get_palette_history_file();

  if (f) {
    fsio_append_text_file(f, PALETTE_NAME);
    fsio_append_text_file(f, "\n");
  }
  return(qty);
} /* load_palette_name */


void null_callback(void *ctx, termpaint_event *event) {
  (void)ctx; (void)event;
}


char *kfc_utils_detect_terminal_type(){
  if (getenv("ALACRITTY_SOCKET") != NULL) {
    return("Alacritty");
  }else if (getenv("ALACRITTY_LOG") != NULL) {
    return("Alacritty");
  }else if (getenv("KITTY_PID") != NULL) {
    return("Kitty");
  }else if (getenv("TERM_PROGRAM") != NULL) {
    char *tp = getenv("TERM_PROGRAM");
    if (strcmp(tp, "Apple_Terminal") == 0) {
      return("Terminal");
    }
  }
  char                  *t           = "UNKNOWN";
  termpaint_integration *integration = termpaintx_full_integration("+kbdsigint +kbdsigtstp");
  if (!integration) {
    puts("Could not init termpaint");
    return(NULL);
  }

  termpaint_terminal *terminal = termpaint_terminal_new(integration);
  termpaint_terminal_set_log_mask(terminal, TERMPAINT_LOG_AUTO_DETECT_TRACE | TERMPAINT_LOG_TRACE_RAW_INPUT);
  termpaint_terminal_set_event_cb(terminal, null_callback, NULL);
  termpaintx_full_integration_set_terminal(integration, terminal);
  termpaint_terminal_auto_detect(terminal);
  termpaintx_full_integration_wait_for_ready_with_message(integration, 100,
                                                          "Terminal auto detection is taking unusually long, press space to abort.");
  char buff[1000];
  char *self_reported_name_and_version = NULL;
  termpaint_terminal_auto_detect_result_text(terminal, buff, sizeof(buff));
  if (termpaint_terminal_self_reported_name_and_version(terminal)) {
    t = strdup_escaped(termpaint_terminal_self_reported_name_and_version(terminal));
  }
  termpaint_terminal_free_with_restore(terminal);
  return(t);
}

struct Vector *get_invalid_palette_property_names(){
  struct Vector *v = vector_new();
  struct Vector *p = get_unique_palette_property_names();

  djbhash_reset_iterator(&invalid_palette_property_names_h);
  djbhash_reset_iterator(&valid_palette_property_names_h);
  for (size_t i = 0; i < vector_size(p); i++) {
    char *n = vector_get(p, i);
    if (is_valid_palette_item_name(n) == false) {
      djbhash_set(&invalid_palette_property_names_h, n, n, DJBHASH_STRING);
      vector_push(v, strdup(n));
    }else{
      djbhash_set(&valid_palette_property_names_h, n, n, DJBHASH_STRING);
    }
    free(n);
  }
  return(v);
}

struct Vector *get_unique_palette_property_names(){
  struct Vector             *v               = vector_new();
  struct Vector             *palette_names_v = get_palette_names_v();
  struct palette_property_t *palette_properties_v;

  djbhash_reset_iterator(&palette_properties_h);
  for (size_t i = 0; i < vector_size(palette_names_v); i++) {
    palette_properties_v = get_palette_name_properties_v((char *)vector_get(palette_names_v, i));
    for (size_t ii = 0; ii < vector_size(palette_properties_v); ii++) {
      struct palette_property_t *pp = vector_get(palette_properties_v, ii);
      if ((hash_item = djbhash_find(&palette_properties_h, pp->translated_name)) != NULL) {
      }else{
        djbhash_set(&palette_properties_h, pp->translated_name, "", DJBHASH_STRING);
        vector_push(v, strdup(pp->translated_name));
      }
      FREE_PALETTE_PROPERTIES(pp);
    }
  }
  return(v);
}


bool kfc_utils_test_kitty_socket(){
  struct Vector *kitty_listen_ons = get_kitty_listen_ons();

  for (size_t i = 0; i < vector_size(kitty_listen_ons); i++) {
    char              *LO  = vector_get(kitty_listen_ons, i);
    kitty_listen_on_t *KLO = parse_kitten_listen_on(LO);
    fprintf(stdout, "connecting to> %s:%d\n", KLO->host, KLO->port);
    char              *BACKGROUND_COLOR = kitty_get_color("background", KLO->host, KLO->port);

    //char              *kitty_query_terminal = kitty_tcp_cmd((const char *)KLO->host, KLO->port, KITTY_QUERY_TERMINAL_CMD);
    fprintf(stdout,
            AC_RESETALL AC_BLUE "Listen on #%lu/%lu: %s\n" AC_RESETALL,
            i + 1, vector_size(kitty_listen_ons),
            (char *)vector_get(kitty_listen_ons, i)
            );
    fprintf(stdout, "bg color:%s\n", BACKGROUND_COLOR);
    //fprintf(stdout,"query terminal: %s\n", kitty_query_terminal);
    /*
     * char *kitty_ls_colors = kitty_tcp_cmd((const char *)KLO->host, KLO->port, __KITTY_GET_COLORS_CMD__);
     * printf("ls colors: %s\n",kitty_ls_colors);
     */
  }
  vector_release(get_kitty_listen_ons);
  return(true);
}


static char *get_cache_ymd(){
  struct timeval tv;
  time_t         nowtime;
  struct tm      *nowtm;
  char           tmbuf[64], *buf;

  gettimeofday(&tv, NULL);
  nowtime = tv.tv_sec;
  nowtm   = localtime(&nowtime);
  strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d", nowtm);
  asprintf(&buf, "%s", tmbuf);
  return(buf);
}


char *kfc_utils_get_exec_path(void){
  char pathbuf[PATH_MAX];

  proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
  if (fsio_file_exists(pathbuf) == false) {
    log_error("pid path failed! exec path not exist: %s", pathbuf);
    return(NULL);
  }
  char *buf;

  asprintf(&buf, "%s", pathbuf);
  return(buf);
}


void kfc_utils_setup_fzf_exec(struct fzf_exec_t *fe){
  char *env_path = (char *)which_path("env", getenv("PATH"));

  if (fsio_file_exists(env_path)) {
    char pathbuf[PATH_MAX];
    int  ret1 = proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
    if (fsio_file_exists(pathbuf)) {
      asprintf(&fe->preview_cmd, "%s -i '%s' -T -p {}",
               env_path,
               pathbuf
               );
    }
  }
  fe->preview_type    = "bottom";
  fe->preview_size    = 80;
  fe->debug_mode      = false;
  fe->select_multiple = false;
  fe->height          = 100;
  char *f = get_palette_history_file();

  fe->fzf_history_file = f ? f : "/dev/null";
}


char *kfc_utils_select_apply_palette(void){
  char              *selected_palette = NULL;
  struct Vector     *v                = vector_new();
  struct Vector     *palette_names_v  = get_palette_names_v();
  struct fzf_exec_t *fe               = exec_fzf_setup();

  assert(fe != NULL);
  {
    kfc_utils_setup_fzf_exec(fe);
    char *env_path = (char *)which_path("env", getenv("PATH"));
    if (fsio_file_exists(env_path)) {
      char pathbuf[PATH_MAX];
      int  ret1 = proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
      if (fsio_file_size(pathbuf)) {
        asprintf(&fe->preview_cmd, "%s -i %s -p {} >/dev/tty 2>/dev/null",
                 env_path,
                 pathbuf
                 );
      }
      fe->preview_type = "bottom";
      fe->preview_size = 0;
      fe->height       = 50;
      fe->debug_mode   = false;
      fe->debug_mode   = true;
    }
    fe->header          = AC_RESETALL AC_UNDERLINE AC_BOLD AC_GREEN "Select Palettes" AC_RESETALL;
    fe->select_multiple = false;
    for (size_t i = 0; i < vector_size(palette_names_v); i++) {
      vector_push(fe->input_options, (char *)vector_get(palette_names_v, i));
    }
  }
  kfc_utils_color_report();
  fprintf(stdout, "%s", AC_HIDE_CURSOR);
  int res = exec_fzf(fe);

  assert(res == 0);
  if (vector_size(fe->selected_options) == 1) {
    selected_palette = (char *)vector_get(fe->selected_options, 0);
  }
  exec_fzf_release(fe);
  fprintf(stdout, "%s", AC_SHOW_CURSOR);
  return(selected_palette);
} /* kfc_utils_select_apply_palette */


char *kfc_utils_select_palette(void){
  char              *selected_palette = NULL;
  struct Vector     *v                = vector_new();
  struct Vector     *palette_names_v  = get_palette_names_v();
  struct fzf_exec_t *fe               = exec_fzf_setup();

  assert(fe != NULL);
  {
    kfc_utils_setup_fzf_exec(fe);
    fe->select_multiple = false;
    {
      fe->query_s = "mono";
      fe->query_s = "";
      fe->header  = AC_RESETALL AC_UNDERLINE AC_BOLD AC_GREEN "Select Palette" AC_RESETALL;
    }
    {
      fe->height       = 100;
      fe->preview_size = 60;
    }
    {
      fe->right_padding  = 3;
      fe->bottom_padding = 2;
      fe->top_padding    = 2;
      fe->left_padding   = 3;
    }
    {
      fe->right_margin  = 3;
      fe->bottom_margin = 2;
      fe->top_margin    = 2;
      fe->left_margin   = 3;
    }
    fe->preview_type = "top";
    fe->debug_mode   = true;
    fe->debug_mode   = false;
    char                *env_path       = (char *)which_path("env", getenv("PATH"));
    struct StringBuffer *fzf_header_sb  = stringbuffer_new();
    struct StringBuffer *fzf_header_sb0 = stringbuffer_new();
    struct StringBuffer *fzf_header_sb1 = stringbuffer_new();
    stringbuffer_append_string(fzf_header_sb, AC_RESETALL AC_ITALIC AC_RED "|n:history-next" AC_RESETALL);
    stringbuffer_append_string(fzf_header_sb, AC_RESETALL AC_ITALIC AC_GREEN "|p:history-prev" AC_RESETALL);
    if (fsio_file_exists(env_path)) {
      char pathbuf[PATH_MAX];
      int  ret1 = proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_ITALIC AC_CYAN "|m:palette-data" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "m";
        kb2->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -p {} -e 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "m";
        kb3->type = "+change-preview";
        asprintf(&kb3->cmd, "%s", kb2->cmd);
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "m";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_ITALIC AC_CYAN "|j:colors" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "j";
        kb2->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -t 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "j";
        kb3->type = "+change-preview";
        asprintf(&kb3->cmd, "%s", kb2->cmd);
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "j";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_ITALIC AC_CYAN "|t:table" AC_RESETALL);
        struct fzf_keybind_t *kb0 = malloc(sizeof(struct fzf_keybind_t));
        kb0->key  = "t";
        kb0->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb0->cmd, "%s -i %s -p {} -T 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb0);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "t";
        kb3->type = "+change-preview";
        asprintf(&kb3->cmd, "%s", kb0->cmd);
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "t";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_ITALIC AC_WHITE "|w:kitty" AC_RESETALL);
        struct fzf_keybind_t *kb = malloc(sizeof(struct fzf_keybind_t));
        kb->key  = "w";
        kb->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb->cmd, "%s -i %s -p {} -K 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "w";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, "|r:report");
        struct fzf_keybind_t *kb = malloc(sizeof(struct fzf_keybind_t));
        kb->key  = "r";
        kb->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb->cmd, "%s -i %s -R 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb);

        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "r";
        kb3->type = "+change-preview";
        asprintf(&kb3->cmd, "%s", kb->cmd);
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "r";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_GREEN AC_BOLD "|h:apply+report" AC_RESETALL);
        struct fzf_keybind_t *kb1 = malloc(sizeof(struct fzf_keybind_t));
        kb1->key  = "h";
        kb1->type = "+change-preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb1->cmd, "%s -i %s -p {} -L >/dev/tty 2>/dev/null; %s -i %s -R",
                   env_path, pathbuf,
                   env_path, pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb1);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "h";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Auto Load Palette + Color Table Mode> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "h";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_GREEN AC_BOLD "|a:apply" AC_RESETALL);
        struct fzf_keybind_t *kb1 = malloc(sizeof(struct fzf_keybind_t));
        kb1->key  = "a";
        kb1->type = "+change-preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb1->cmd, "%s -i %s -p {} -L >/dev/tty 2>/dev/null; %s -i %s -T -p {}",
                   env_path, pathbuf,
                   env_path, pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb1);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "a";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Auto Load Palette + Table Mode> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "a";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_BLUE "|k:dark" AC_RESETALL);
        struct fzf_keybind_t *kb = malloc(sizeof(struct fzf_keybind_t));
        kb->key  = "k";
        kb->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb->cmd, "%s -i %s -p {} -k 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "k";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Dark Palettes> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "k";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "wrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_BLUE "|s:sequence" AC_RESETALL);
        struct fzf_keybind_t *kb = malloc(sizeof(struct fzf_keybind_t));
        kb->key  = "s";
        kb->type = "preview";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb->cmd, "%s -i %s -p {} -E 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "s";
        kb3->type = "+change-preview";
        asprintf(&kb3->cmd, "%s", kb->cmd);
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "s";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "wrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_RED "|/:toggle-preview" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "/";
        kb2->type = "change-preview-window";
        asprintf(&kb2->cmd, "down,border-top,50%%|up,border-bottom,50%%|left,border-right,70%%|right,border-left,70%%");
        vector_push(fe->fzf_keybinds_v, kb2);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_RED "|l:all" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "l";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -l 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "l";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "All Palettes> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "l";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_YELLOW "|n:load-history" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "n";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "cat '%s' 2>/dev/null",
                   get_palette_history_file()
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "n";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Palette Historys> ");
        vector_push(fe->fzf_keybinds_v, kb3);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_BOLD AC_YELLOW "|g:very dark" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "g";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -k -B 2 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "g";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Very Dark Palettes> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "g";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_BOLD AC_RED "|i:invalid" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "i";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -I 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "i";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Invalid Properties> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "i";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb0, AC_RESETALL AC_RED "|u:unique" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "u";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -U 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);

        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "u";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Unique Properties> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "u";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_GREEN "|y:very bright" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "y";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -b -B 98 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "y";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Very Bright Palettes> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "y";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
      {
        stringbuffer_append_string(fzf_header_sb1, AC_RESETALL AC_YELLOW "|b:bright" AC_RESETALL);
        struct fzf_keybind_t *kb2 = malloc(sizeof(struct fzf_keybind_t));
        kb2->key  = "b";
        kb2->type = "reload";
        if (fsio_file_exists(pathbuf)) {
          asprintf(&kb2->cmd, "%s -i %s -b 2>/dev/null",
                   env_path,
                   pathbuf
                   );
        }
        vector_push(fe->fzf_keybinds_v, kb2);
        struct fzf_keybind_t *kb3 = malloc(sizeof(struct fzf_keybind_t));
        kb3->key  = "b";
        kb3->type = "+change-prompt";
        asprintf(&kb3->cmd, "Bright Palettes> ");
        vector_push(fe->fzf_keybinds_v, kb3);
        struct fzf_keybind_t *kb4 = malloc(sizeof(struct fzf_keybind_t));
        kb4->key  = "b";
        kb4->type = "+change-preview-window";
        asprintf(&kb4->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb4);
      }
    }
    vector_push(fe->fzf_header_lines_v, stringbuffer_to_string(fzf_header_sb));
    vector_push(fe->fzf_header_lines_v, stringbuffer_to_string(fzf_header_sb0));
    vector_push(fe->fzf_header_lines_v, stringbuffer_to_string(fzf_header_sb1));

    for (size_t i = 0; i < vector_size(palette_names_v); i++) {
      vector_push(fe->input_options, (char *)vector_get(palette_names_v, i));
    }
  }

  int res = exec_fzf(fe);

  assert(res == 0);
  if (vector_size(fe->selected_options) == 1) {
    selected_palette = (char *)vector_get(fe->selected_options, 0);
  }
  exec_fzf_release(fe);
  return(selected_palette);
} /* kfc_utils_select_palette */

struct Vector *kfc_utils_select_palettes(void){
  struct Vector     *v               = vector_new();
  struct Vector     *palette_names_v = get_palette_names_v();
  struct fzf_exec_t *fe              = exec_fzf_setup();

  assert(fe != NULL);
  {
    kfc_utils_setup_fzf_exec(fe);
    fe->header          = AC_RESETALL AC_UNDERLINE AC_BOLD AC_GREEN "Select Palettes" AC_RESETALL;
    fe->select_multiple = true;
    fe->height          = 100;
    fe->preview_size    = 40;
    fe->preview_type    = "top";
    for (size_t i = 0; i < vector_size(palette_names_v); i++) {
      vector_push(fe->input_options, (char *)vector_get(palette_names_v, i));
    }
  }

  int res = exec_fzf(fe);

  assert(res == 0);

  for (size_t i = 0; i < vector_size(fe->selected_options); i++) {
    vector_push(v, (char *)vector_get(fe->selected_options, i));
  }
  exec_fzf_release(fe);
  return(v);
}


int kfc_utils_color_report(void){
  fprintf(stdout, "\ec%s",
          b64_decode(COLOR_REPORT_B64, strlen(COLOR_REPORT_B64))
          );
  return(EXIT_SUCCESS);
}


static char *get_palette_history_file(){
  if (!fsio_file_exists(FZF_UTILS_DEFAULT_HISTORY_FILE)) {
    return(NULL);
  }
  return(FZF_UTILS_DEFAULT_HISTORY_FILE);
}


char *get_palette_history(){
  char *f = get_palette_history_file();

  if (!fsio_file_exists(f)) {
    return(NULL);
  }
  char *s = stringfn_mut_trim(fsio_read_text_file(f));

  return(s);
}


char *get_ansi_reset_sequence(){
  return("\ec");
}
