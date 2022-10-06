#pragma once
#include "kfc-utils/kfc-utils.h"
/////////////////////////////////////
#ifndef INCBIN_PREFIX
#define INCBIN_PREFIX    inc_palette_
#endif
#ifndef INCBIN_STYLE
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#endif
#ifndef INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_SILENCE_BITCODE_WARNING
#endif
#include "incbin/incbin.h"
INCTXT(EmbeddedKfcUtilsPaletteTemplate, KFC_UTILS_PALETTE_TEMPLATE_FILE);
/////////////////////////////////////
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <math.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <unistd.h>
/////////////////////////////////////
#include "incbin/incbin.h"
#include "timestamp/timestamp.h"
/////////////////////////////////////
#define KFC_CLI_STORAGE_DIR                       "%s/.kfc"
#define KFC_CLI_PALETTES_HISTORY_PATH_TEMPLATE    "%s/%s"
#define DEBUG_TEMPLATE_RENDER                     false
#define DEBUG_PALETTES                            false
#define DEBUG_PALETTE_CODES                       false
#define PALETTES_QTY_LIMIT                        999
//#ifndef __PALETTES_HASH__
#define PALETTES_HASH                             "xxxxxxxxxxx"
//#else
//#define PALETTES_HASH                             __PALETTES_HASH__
//#endif
const char *FZF_UTILS_DEFAULT_HISTORY_FILE = "/tmp/.fzf-utils-history.txt";
const char          *HOMEDIR;
const char          *kfc_utils_storage_dir;
const char          *kfc_utils_palettes_history_file;
static const size_t PALETTES_QTY_LIMIT_LOAD   = 2000;
static const char   PALETTES_JINJA_TEMPLATE[] = "etc/kfc-utils-palettes.c.j2";
static const char   PALETTES_LOAD_DIR[]       = "palettes/load";
char                *KFC_UTILS_STORAGE_DIR;
struct kfc_utils_storage_t {
  char *homedir;
  char *dir;
  char *dir_template;
  bool is_valid;
} static kfc_utils_storage = {
  .homedir      = NULL,
  .dir          = NULL,
  .dir_template = KFC_CLI_STORAGE_DIR,
  .is_valid     = false,
};
INCTXT(palettes_template_c, "etc/kfc-utils-palettes.c.j2");
#define KFC_PALETTES_TEMPLATE_SIZE_VAR    "palettes_template_c_size"
#define KFC_PALETTES_TEMPLATE_DATA_VAR    "palettes_template_c_data"
/////////////////////////////////////
#include "kfc-utils/kfc-utils-data.h"
#include "kfc-utils/kfc-utils-module.h"
#include "string-utils/string-utils.h"
/////////////////////////////////////
#include "submodules/tinydir/tinydir.h"
/////////////////////////////////////
#include "ansi-rgb-utils/ansi-rgb-utils.h"
#include "process/process.h"
/////////////////////////////////////
#include "bench/bench.h"
#include "bytes/bytes.h"
#include "c_ansi/ansi-utils/ansi-utils.h"
#include "c_fsio/include/fsio.h"
#include "c_fsio/include/fsio.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_stringfn/include/stringfn.h"
#include "c_vector/vector/vector.h"
#include "parson/parson.h"
//#include "debug-memory/debug_memory.h"
#include "djbhash/src/djbhash.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "libfort/src/fort.h"
#include "rgba/src/rgba.h"
#include "roscha/include/hmap.h"
#include "roscha/include/object.h"
#include "roscha/include/parser.h"
#include "roscha/include/roscha.h"
#include "roscha/include/sds/sds.h"
#include "roscha/include/token.h"
#include "roscha/include/vector.h"
#include "str-truncate.c/src/str-truncate.h"
#include "submodules/b64.c/b64.h"
#include "tempdir.c/tempdir.h"
#include "termpaint.h"
#include "termpaintx.h"
#include "timestamp/timestamp.h"
#include "which/src/which.h"
/////////////////////////////////////
static char *kfc_utils_get_palette_item_sequence(const struct palette_property_t *pp);
char *kfc_utils_get_palette_name_sequence(const char *PALETTE_NAME);
static bool is_valid_palette_item_name(const char *PALETTE_ITEM_NAME);
static char *kfc_utils_get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME);
static void __kfc_utils_constructor(void) __attribute__((constructor));
static void __attribute((unused)) __kfc_utils_destructor(void) __attribute__((destructor));
static char *kfc_utils_get_cache_ymd();
static char *kfc_utils_get_palette_history_file();
static char *kfc_utils_render_palettes_template(struct Vector *__template_palettes_v);
static char *normalize_hex_color(const char *COLOR);
static bool kfc_utils_get_color_is_brightness_type(const float BRIGHTNESS, const int BACKGROUND_BRIGHTNESS_TYPE, const double BRIGHTNESS_THRESHOLD);
static void __kfc_utils_at_exit(void);
static char *kfc_utils_apply_sequence(char *SEQ, char *STR);

/////////////////////////////////////
static bool                KFC_UTILS_DEBUG_MODE = false;
static module(kfc_utils) * KFC;
static struct djbhash      palette_properties_h, valid_palette_property_names_h, invalid_palette_property_names_h;
static struct djbhash_node *hash_item;
static bool                __kfc_utils_exited         = false;
static bool                __kfc_utils_palette_loaded = false;
enum palette_cache_items_t { PALETTES_TABLE,
                             PALETTES_CACHE_QTY,
};
static struct kfc_utils_cache_files_t { char *name; char *path; } PALETTE_CACHE_FILES[] = {
  [PALETTES_TABLE] = { .name = "palettes-table-cache", },
                     { 0 },
};
static struct palette_name_translations_t                         palette_name_translations[] = {
  { .src = "color0",          .dst = "color00",         },
  { .src = "color1",          .dst = "color01",         },
  { .src = "color2",          .dst = "color02",         },
  { .src = "color3",          .dst = "color03",         },
  { .src = "color4",          .dst = "color04",         },
  { .src = "color5",          .dst = "color05",         },
  { .src = "color6",          .dst = "color06",         },
  { .src = "color7",          .dst = "color07",         },
  { .src = "color8",          .dst = "color08",         },
  { .src = "color9",          .dst = "color09",         },
  { .src = "cursorcolor",     .dst = "cursor",          },
  { .src = "load-palette",    .dst = "restore-palette", },
  { .src = "restore_palette", .dst = "restore-palette", },
  { .src = "save_palette",    .dst = "save-palette",    },
  { 0 },
};
static char                                                       *palette_color_properties[] = {
  "background",
  "foreground",
  "color00",
  "color01",
  "color02",
  "color03",
  "color04",
  "color05",
  "color06",
  "color07",
  "color08",
  "color09",
  "color10",
  "color11",
  "color12",
  "color13",
  "color14",
  "color15",
  "color16",
  "color17",
  "color18",
  "color19",
  "color20",
  "color21",
  "color22",
  "color23",
  "color24",
  "color25",
  "cursor",
  "selection_background",
  "selection_foreground",
  0,
};
static struct palette_code_value_translations_t                   palette_code_value_translations[] = {
  { .name = "cursorstyle",     .src = "under",     .dst = "3 q", },
  { .name = "cursorstyle",     .src = "block",     .dst = "1 q", },
  { .name = "cursorstyle",     .src = "bar",       .dst = "5 q", },
  { .name = "cursorblink",     .src = "off",       .dst = "l",   },
  { .name = "cursorblink",     .src = "on",        .dst = "h",   },
  { .name = "altscreen",       .src = "off",       .dst = "l",   },
  { .name = "altscreen",       .src = "on",        .dst = "h",   },
  { .name = "screen",          .src = "off",       .dst = "l",   },
  { .name = "screen",          .src = "on",        .dst = "h",   },
  { .name = "cursorvisible",   .src = "off",       .dst = "l",   },
  { .name = "cursorvisible",   .src = "on",        .dst = "h",   },
  { .name = "reportfocus",     .src = "off",       .dst = "l",   },
  { .name = "reportfocus",     .src = "on",        .dst = "h",   },
  { .name = "mouse",           .src = "clickdrag", .dst = "2h",  },
  { .name = "mouse",           .src = "motion",    .dst = "3h",  },
  { .name = "mouse",           .src = "click",     .dst = "0h",  },
  { .name = "mouse",           .src = "off",       .dst = "0l",  },
  { .name = "reset",           .src = "yes",       .dst = "c",   },
  { .name = "reset",           .src = "no",        .dst = "",    },
  { .name = "restore-palette", .src = "yes",       .dst = "101", },
  { .name = "save-palette",    .src = "yes",       .dst = "001", },
  { .name = "restore-palette", .src = "True",      .dst = "101", },
  { .name = "save-palette",    .src = "True",      .dst = "001", },
  { .name = "restore-palette", .src = "true",      .dst = "101", },
  { .name = "save-palette",    .src = "true",      .dst = "001", },
  { .name = "restore-palette", .src = "1",         .dst = "101", },
  { .name = "save-palette",    .src = "1",         .dst = "001", },
  { .name = "restore-palette", .src = "on",        .dst = "101", },
  { .name = "save-palette",    .src = "on",        .dst = "001", },
  { 0 },
};
static struct palette_code_t                                      palette_codes[] = {
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
  { .name = "save-palette",         .code = "]30",                 },
  { .name = "restore-palette",      .code = "]30",                 },
  { 0 },
};

static void __kfc_utils_at_exit(){
  if (__kfc_utils_exited == true) {
    return(0);
  }
  __kfc_utils_exited = true;
  if (__kfc_utils_palette_loaded == false) {
    fprintf(stdout, AC_SHOW_CURSOR);
  }
  exit(0);
}

static void __attribute__((constructor)) __kfc_utils_constructor(){
  signal(SIGINT, __kfc_utils_at_exit);
  signal(SIGTERM, __kfc_utils_at_exit);
  signal(SIGQUIT, __kfc_utils_at_exit);
  atexit(__kfc_utils_at_exit);
  struct passwd *pw = getpwuid(getuid());
  HOMEDIR = pw->pw_dir;
  if (fsio_dir_exists(HOMEDIR) == true) {
    kfc_utils_storage.homedir = HOMEDIR;
  }else{
    kfc_utils_storage.homedir = getenv("HOME");
  }
  if (kfc_utils_storage.dir_template != NULL) {
    asprintf(&kfc_utils_storage_dir, kfc_utils_storage.dir_template, kfc_utils_storage.homedir);
    if (false == fsio_dir_exists(KFC_UTILS_STORAGE_DIR)) {
      fsio_mkdirs(KFC_UTILS_STORAGE_DIR, 0700);
    }
    asprintf(&kfc_utils_palettes_history_file, KFC_CLI_PALETTES_HISTORY_PATH_TEMPLATE,
             kfc_utils_storage_dir, "palettes-history.log"
             );
  }

  if (getenv("DEBUG_MODE") != NULL) {
    KFC_UTILS_DEBUG_MODE = true;
  }

  if (fsio_dir_exists(kfc_utils_storage.homedir) == true) {
    asprintf(&kfc_utils_storage.dir, "%s/%s/%s", kfc_utils_storage.homedir, ".config", "kfc");
    if (fsio_dir_exists(kfc_utils_storage.dir) == false) {
      fsio_mkdirs(kfc_utils_storage.dir, FSIO_MODE_ALL);
    }
  }

  djbhash_init(&palette_properties_h);
  djbhash_init(&valid_palette_property_names_h);
  djbhash_init(&invalid_palette_property_names_h);
  KFC       = require(kfc_utils);
  KFC->mode = (KFC_UTILS_DEBUG_MODE == true) ? KFC_LOG_DEBUG : KFC_LOG_DEFAULT;

  if (KFC_UTILS_DEBUG_MODE == true) {
    log_debug("<%d> [%s] palettes qty:%lu",
              getpid(),
              __FUNCTION__,
              KFC->get_palettes_qty()
              );
  }
  if (false) {
    if (strcmp(PALETTES_HASH, "") != 0) {
      char *ymd = kfc_utils_get_cache_ymd();
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
  kfc_utils_storage.is_valid = (fsio_dir_exists(kfc_utils_storage.dir) == true) ? true : false;
  if (KFC_UTILS_DEBUG_MODE == true) {
    log_debug("<%d> "
              "\nkfc_utils_storage.dir: %s"
              "\nkfc_utils_storage.is_valid: %s"
              "",
              getpid(),
              kfc_utils_storage.dir,
              (kfc_utils_storage.is_valid == true) ? "Yes" : "No"
              );
  }
} /* __kfc_utils_constructor */

static void __attribute__((destructor)) __kfc__utils_destructor(){
  clib_module_free(KFC);
  djbhash_destroy(&invalid_palette_property_names_h);
  djbhash_destroy(&valid_palette_property_names_h);
  djbhash_destroy(&palette_properties_h);
  fprintf(stdout, AC_ALT_SCREEN_OFF);
}

char *kfc_utils_get_palette_colors_table(const char *PALETTE_NAME){
  struct StringBuffer       *sb                   = stringbuffer_new();
  struct palette_property_t *palette_properties_v = kfc_utils_get_palette_name_properties_v(PALETTE_NAME);

  if (vector_size(palette_properties_v) < 1) {
    return("No properties found");
  }

  ft_table_t *table = ft_create_table();

  ft_set_border_style(table, FT_FRAME_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_BLUE);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);

  ft_write_ln(table,
              "Property",
              "Value",
              "Color",
              "Red", "Green", "Blue",
              "Brightness",
              "Bright",
              "Dark",
              "V. Bright",
              "V. Dark",
              "Sequence"
              );
  size_t max_width = 0;

  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    struct palette_property_t *pp = vector_get(palette_properties_v, i);
    ft_u8printf_ln(table,
                   "%s|%s"
                   "|%s"
                   "|%s|%s|%s"
                   "|%s|%s|%s"
                   "|%s|%s|%s",
                   pp->name,
                   (false == pp->is_color) ? pp->translated_value : normalize_hex_color(pp->translated_value),

                   (true == pp->is_color) ? "Yes" : "No",
                   (false == pp->is_color) ? "" : pp->color_t->red_s,
                   (false == pp->is_color) ? "" : pp->color_t->green_s,
                   (false == pp->is_color) ? "" : pp->color_t->blue_s,
                   (false == pp->is_color) ? ""
                    : pp->color_t->brightness_s,
                   (false == pp->is_color) ? ""
                    : (pp->color_t->is_bright == true) ? "Yes" : "No",
                   (false == pp->is_color) ? ""
                    : (pp->color_t->is_dark == true) ? "Yes" : "No",
                   (false == pp->is_color) ? ""
                    : (pp->color_t->is_very_bright == true) ? "Yes" : "No",
                   (false == pp->is_color) ? ""
                    : (pp->color_t->is_very_dark == true) ? "Yes" : "No",
                   (false == pp->is_color) ? "" : pp->color_t->escaped_sequence
                   );
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, (true == pp->is_color) ? FT_COLOR_GREEN : FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_RED);
    ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_GREEN);
    ft_set_cell_prop(table, i + 1, 5, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_BLUE);
    ft_set_cell_prop(table, i + 1, 6, FT_CPROP_CONT_FG_COLOR, (false == pp->is_color) ? FT_COLOR_DEFAULT : FT_COLOR_LIGHT_YELLOW);
    ft_set_cell_prop(table, i + 1, 7, FT_CPROP_CONT_FG_COLOR, (false == pp->is_color) ? FT_COLOR_DEFAULT
            : (pp->color_t->is_bright) ? FT_COLOR_LIGHT_GREEN : FT_COLOR_LIGHT_RED
                     );
    ft_set_cell_prop(table, i + 1, 8, FT_CPROP_CONT_FG_COLOR, (false == pp->is_color) ? FT_COLOR_DEFAULT
            : (pp->color_t->is_dark) ? FT_COLOR_LIGHT_GREEN : FT_COLOR_LIGHT_RED
                     );
    ft_set_cell_prop(table, i + 1, 9, FT_CPROP_CONT_FG_COLOR, (false == pp->is_color) ? FT_COLOR_DEFAULT
            : (pp->color_t->is_very_bright) ? FT_COLOR_LIGHT_GREEN : FT_COLOR_LIGHT_RED
                     );
    ft_set_cell_prop(table, i + 1, 10, FT_CPROP_CONT_FG_COLOR, (false == pp->is_color) ? FT_COLOR_DEFAULT
            : (pp->color_t->is_very_dark) ? FT_COLOR_LIGHT_GREEN : FT_COLOR_LIGHT_RED
                     );
    max_width = (strlen(pp->name) > max_width) ? strlen(pp->name) : max_width;
    FREE_PALETTE_PROPERTIES(pp);
  }
  char *table1_s = strdup(ft_to_u8string(table));

  ft_destroy_table(table);

  ft_table_t *table_title = ft_create_table();
  {
    ft_set_border_style(table_title, FT_FRAME_STYLE);
    ft_set_tbl_prop(table_title, FT_TPROP_LEFT_MARGIN, 0);
    ft_set_tbl_prop(table_title, FT_TPROP_RIGHT_MARGIN, 0);
  }
  {
    ft_printf_ln(table_title, "%s", PALETTE_NAME);
  }
  char *table_title_s = strdup(ft_to_u8string(table_title));
  {
    ft_destroy_table(table_title);
  }

  stringbuffer_append_string(sb, AC_RESETALL);
  stringbuffer_append_string(sb, AC_BLUE);
  stringbuffer_append_string(sb, table_title_s);
  stringbuffer_append_string(sb, AC_RESETALL);
  stringbuffer_append_string(sb, table1_s);
  char *ret_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);

  return(ret_s);
} /* kfc_utils_get_palette_colors_table */

char *kfc_utils_get_palette_properties_table(const char *PALETTE_NAME){
  struct palette_property_t *palette_properties_v = kfc_utils_get_palette_name_properties_v(PALETTE_NAME);

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
  char *seq = kfc_utils_get_palette_name_sequence(PALETTE_NAME);

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

struct Vector *kfc_utils_get_palette_names_by_brightness_type(int BACKGROUND_BRIGHTNESS_TYPE, float BRIGHTNESS_THRESHOLD){
  struct Vector             *v               = vector_new();
  struct Vector             *palette_names_v = kfc_utils_get_palette_names_v();
  struct Vector             *palette_properties_v;
  struct palette_property_t *pp;

  for (size_t ii = 0; ii < vector_size(palette_names_v); ii++) {
    char *pname = (char *)vector_get(palette_names_v, ii);
    palette_properties_v = kfc_utils_get_palette_name_properties_v(pname);
    bool found = false;
    for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
      pp = vector_get(palette_properties_v, i);
      if (found == false && strcmp("background", pp->name) == 0) {
        switch (BACKGROUND_BRIGHTNESS_TYPE) {
        case BACKGROUND_BRIGHTNESS_DARK:
          if (kfc_utils_palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, BRIGHTNESS_THRESHOLD)) {
            vector_push(v, strdup(pname));
            found = true;
          }
          break;
        case BACKGROUND_BRIGHTNESS_BRIGHT:
          if (kfc_utils_palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, BRIGHTNESS_THRESHOLD)) {
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

static bool kfc_utils_get_color_is_brightness_type(const float BRIGHTNESS, const int BACKGROUND_BRIGHTNESS_TYPE, const double BRIGHTNESS_THRESHOLD){
  switch (BACKGROUND_BRIGHTNESS_TYPE) {
  case BACKGROUND_BRIGHTNESS_DARK:
    return((BRIGHTNESS <= BRIGHTNESS_THRESHOLD) ? true : false);

    break;
  case BACKGROUND_BRIGHTNESS_BRIGHT:
    return((BRIGHTNESS >= BRIGHTNESS_THRESHOLD) ? true : false);

    break;
  default:
    log_error("Invalid brightness type");
    return(false);

    break;
  }
  return(false);
}

rgba_t kfc_utils_get_color_rgba_t(const char *COLOR){
  char    *hex = normalize_hex_color(COLOR);
  short   ok;
  int32_t val = rgba_from_string(hex, &ok);

  return(rgba_new(val));
}

float kfc_utils_get_color_brightness(const char *COLOR){
  double hsluv[3];
  rgba_t _rgba = kfc_utils_get_color_rgba_t(COLOR);

  rgb2hsluv(_rgba.r, _rgba.g, _rgba.b, &hsluv[0], &hsluv[2], &hsluv[2]);
  return(hsluv[2]);
}

static char *normalize_hex_color(const char *COLOR){
  char *hex = NULL;

  if (false == stringfn_starts_with(COLOR, "#")) {
    asprintf(&hex, "#%s", COLOR);
  }else{
    asprintf(&hex, "%s", COLOR);
  }
  return(hex);
}

bool kfc_utils_palette_background_is_brightness_type(char *BACKGROUND_COLOR, int BACKGROUND_BRIGHTNESS_TYPE, double BRIGHTNESS_THRESHOLD){
  char    *hex = normalize_hex_color(BACKGROUND_COLOR);
  double  hsluv[3];

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

char *kfc_utils_get_palettes_table() {
  char *cache_file = PALETTE_CACHE_FILES[PALETTES_TABLE].path;

  if (false) {
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
  }

  ft_table_t *table = ft_create_table();

  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_border_style(table, FT_SOLID_ROUND_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
  ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
  ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
  ft_set_cell_prop(table, FT_ANY_ROW, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
  ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
  ft_set_cell_prop(table, FT_ANY_ROW, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
  for (int r = 0; r < 5; r++) {
    ft_set_cell_prop(table, 0, r, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, 0, r, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
    ft_set_cell_prop(table, 0, r, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
    ft_set_cell_prop(table, 0, r, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);
  }

  ft_write_ln(table,
              "Palette",
              "Size",
              "# Props",
              "Background Brightness"
              ""
              );
  ft_set_cell_span(table, 0, 3, 2);

  for (size_t i = 0; i < vector_size(KFC->palettes_v) && i < PALETTES_QTY_LIMIT; i++) {
    struct inc_palette_t *p = vector_get(require(kfc_utils)->palettes_v, i);
    struct Vector        *pp = kfc_utils_get_palette_name_properties_v(p->name);
    float                brightness = 0;
    bool                 is_dark = false, is_very_dark = false, is_bright = false, is_very_bright = false;
    for (size_t i = 0; i < vector_size(pp); i++) {
      struct palette_property_t *_pp = vector_get(pp, i);
      if (strcmp(_pp->name, "background") == 0) {
        brightness     = kfc_utils_get_color_brightness(_pp->translated_value);
        is_dark        = kfc_utils_palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, 20);
        is_very_dark   = kfc_utils_palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, 5);
        is_bright      = kfc_utils_palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, 80);
        is_very_bright = kfc_utils_palette_background_is_brightness_type(_pp->translated_value, BACKGROUND_BRIGHTNESS_BRIGHT, 95);
      }
      FREE_PALETTE_PROPERTIES(_pp);
    }

    struct StringFNStrings sp = stringfn_split(p->file, '/');
    ft_printf_ln(table,
                 "%s|%s|%lu|%5.2f|%s",
                 p->name,
                 bytes_to_string(p->size),
                 vector_size(pp),
                 brightness,
                 is_very_dark ? "Very Dark"
                   : is_dark ? "Dark"
                   : is_very_bright ? "Very Bright"
                   : "Bright"
                 );

    size_t qty = vector_size(pp);
    if (p->size > 600) {
      ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    }else if (p->size < 100) {
      ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
    }
    if (qty > 30) {
      ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    }else if (qty < 10) {
      ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
    }

    if (is_very_dark) {
      ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
      ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
    }else if (is_very_bright) {
      ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
      ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
    }else if (is_bright) {
      ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
      ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    }

    stringfn_release_strings_struct(sp);
    vector_release(pp);
  }

  char *table_s = ft_to_string(table);

  ft_destroy_table(table);

  if (false) {
    if (cache_file != NULL && strcmp(PALETTES_HASH, "") != 0) {
      fsio_write_text_file(cache_file, table_s);
    }
  }
  return(table_s);
} /* get_palettes_table */

char *kfc_utils_get_palette_name_data(const char *PALETTE_NAME){
  struct inc_palette_t *p = kfc_utils_get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    log_error("No properties for palette %s found", PALETTE_NAME);
    return(NULL);
  }
  return(p->data);
}

static char *kfc_utils_apply_sequence(char *SEQ, char *STR){
  char *s;

  asprintf(&s, AC_RESETALL "%s" "%s"  AC_RESETALL, SEQ, STR);
  return(s);
}

struct Vector *kfc_utils_get_palette_name_properties_v(const char *PALETTE_NAME){
  struct Vector        *v = vector_new();
  struct inc_palette_t *p = kfc_utils_get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    log_error("No properties for palette %s found", PALETTE_NAME);
    return(v);
  }
  struct StringFNStrings lines = stringfn_split_lines_and_trim(p->data);

  for (size_t i = 0; i < (size_t)lines.count; i++) {
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
    pp->is_translated    = kfc_utils_palette_item_name_is_translated(pp->name);
    pp->translated_name  = (pp->is_valid_name == true) ? strdup(pp->name) : kfc_utils_get_translated_palette_property_name(pp->name);
    pp->translated_value = (pp->is_translated == true) ? kfc_utils_translate_palette_item_value(pp->translated_name, pp->value) : strdup(pp->value);
    pp->code             = kfc_utils_get_palette_item_code(pp->translated_name);
    pp->sequence         = kfc_utils_get_palette_item_sequence(pp);
    pp->suffix           = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_SUFFIX) : "";
    pp->prefix           = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_PREFIX) : "";
    pp->is_color         = kfc_utils_palette_property_is_color(pp->translated_name);
    pp->color_t          = NULL;
    if (true == pp->is_color) {
      pp->color_t             = malloc(sizeof(struct palette_property_color_t));
      pp->color_t->brightness = kfc_utils_get_color_brightness(pp->translated_value);
      asprintf(&pp->color_t->brightness_s, "%d", (int)pp->color_t->brightness);
      pp->color_t->is_bright      = kfc_utils_get_color_is_brightness_type(pp->color_t->brightness, BACKGROUND_BRIGHTNESS_BRIGHT, atof(DEFAULT_MAX_BRIGHTNESS));
      pp->color_t->is_dark        = kfc_utils_get_color_is_brightness_type(pp->color_t->brightness, BACKGROUND_BRIGHTNESS_DARK, atof(DEFAULT_MAX_BRIGHTNESS));
      pp->color_t->is_very_bright = kfc_utils_get_color_is_brightness_type(pp->color_t->brightness, BACKGROUND_BRIGHTNESS_BRIGHT, atof(DEFAULT_MAX_BRIGHTNESS_VERY));
      pp->color_t->is_very_dark   = kfc_utils_get_color_is_brightness_type(pp->color_t->brightness, BACKGROUND_BRIGHTNESS_DARK, atof(DEFAULT_MAX_BRIGHTNESS_VERY));
      pp->color_t->rgba           = kfc_utils_get_color_rgba_t(pp->translated_value);
      pp->color_t->red            = (int)(255 * pp->color_t->rgba.r);
      pp->color_t->green          = (int)(255 * pp->color_t->rgba.g);
      pp->color_t->blue           = (int)(255 * pp->color_t->rgba.b);
      asprintf(&pp->color_t->red_s, "%d", pp->color_t->red);
      asprintf(&pp->color_t->green_s, "%d", pp->color_t->green);
      asprintf(&pp->color_t->blue_s, "%d", pp->color_t->blue);
      asprintf(&pp->color_t->sequence, "\x1b[38;2;%d;%d;%dm",
               pp->color_t->red,
               pp->color_t->green,
               pp->color_t->blue
               );
      pp->color_t->escaped_sequence = strdup_escaped(pp->color_t->sequence);
    }
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

size_t kfc_utils_get_palettes_data_bytes(){
  size_t               s    = 0;
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    s += (size_t)strlen(tmp->data);
  }
  return(s);
}

struct Vector *kfc_utils_get_palette_names_v(){
  struct Vector        *v   = vector_new();
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    vector_push(v, (char *)tmp->name);
  }
  return(v);
}

struct Vector *kfc_utils_get_palettes_v(){
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
    printf("palette #%lu> %s\n%s\n", i, p->name, kfc_utils_get_palette_properties_table(p->name));
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

static char *kfc_utils_get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME){
  struct palette_name_translations_t *tmp = palette_name_translations;

  for (size_t i = 0; tmp->src != NULL && tmp->dst != NULL && tmp != NULL; tmp++, i++) {
    if (strcmp(PALETTE_PROPERTY_NAME, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }
  return(strdup(PALETTE_PROPERTY_NAME));
}

bool kfc_utils_palette_item_name_is_translated(const char *ITEM_NAME){
  struct palette_code_value_translations_t *tmp = palette_code_value_translations;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0) {
      return(true);
    }
  }
  return(false);
}

char *kfc_utils_translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE){
  if (false == kfc_utils_palette_item_name_is_translated(ITEM_NAME)) {
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

size_t kfc_utils_random_palette_index(){
  srand(time(NULL) + getpid() + timestamp());
  return((rand() % (PALETTES_QTY)));
}

size_t kfc_utils_random_dark_palette_index(float MAX_DARKNESS_VALUE){
  int                       r                  = -1;
  bool                      background_is_dark = false;
  struct palette_property_t *pp;
  struct inc_palette_t      *p;
  struct Vector             *pp_v;

  while (background_is_dark == false) {
    r    = kfc_utils_random_palette_index();
    p    = vector_get(require(kfc_utils)->palettes_v, r);
    pp_v = kfc_utils_get_palette_name_properties_v(p->name);
    for (size_t i = 0; i < vector_size(pp_v); i++) {
      pp = vector_get(pp_v, i);
      if (strcmp(pp->translated_name, "background") == 0) {
        background_is_dark = kfc_utils_palette_background_is_brightness_type(
          pp->translated_value,
          BACKGROUND_BRIGHTNESS_DARK,
          MAX_DARKNESS_VALUE
          );
      }
    }
  }
  if (pp) {
    FREE_PALETTE_PROPERTIES(pp);
  }
  return((size_t)r);
}

char *kfc_utils_get_palette_name_by_index(const int INDEX){
  return(palette_t_list[INDEX].name);
}

struct inc_palette_t *kfc_utils_get_palette_t_by_index(const size_t INDEX){
  return(&(palette_t_list[INDEX]));
}

struct inc_palette_t *kfc_utils_get_palette_t_by_name(const char *PALETTE_NAME){
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

char *kfc_utils_get_palette_name_property_value(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME){
  struct Vector *palette_properties = kfc_utils_get_palette_name_properties_v(PALETTE_NAME);

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

static char *kfc_utils_get_palette_item_sequence(const struct palette_property_t *pp){
  struct StringBuffer *palette_codes_b = stringbuffer_new();

  stringbuffer_append_string(palette_codes_b, CODE_PREFIX);
  stringbuffer_append_string(palette_codes_b, pp->code);
  stringbuffer_append_string(palette_codes_b, pp->translated_value);
  stringbuffer_append_string(palette_codes_b, CODE_SUFFIX);
  char *palette_code = stringbuffer_to_string(palette_codes_b);

  stringbuffer_release(palette_codes_b);
  return(palette_code);
}

char *kfc_utils_get_palette_item_code(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(tmp->code);
    }
  }

  return(NULL);
}

char *kfc_utils_get_palette_name_sequence(const char *PALETTE_NAME){
  struct StringBuffer *sb = stringbuffer_new();
  struct Vector       *pp = kfc_utils_get_palette_name_properties_v(PALETTE_NAME);

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

size_t kfc_utils_load_palette_name(const char *PALETTE_NAME){
  char   *seq = kfc_utils_get_palette_name_sequence(PALETTE_NAME);
  size_t qty  = fprintf(stdout, "%s", seq);

  fflush(stdout);
  char *f = kfc_utils_get_palette_history_file();

  if (f) {
    fsio_append_text_file(f, PALETTE_NAME);
    fsio_append_text_file(f, "\n");
  }
  if (qty > 0) {
    __kfc_utils_palette_loaded = true;
    {
      char *palette_data = kfc_utils_get_palette_name_data(PALETTE_NAME);
      char *hist;
      char *cwd = kfc_utils_get_cwd();
      asprintf(&hist, "%lu:%s:%lu:%s:%s:%s\n",
               (size_t)timestamp(),
               PALETTE_NAME,
               qty,
               b64_encode(cwd, strlen(cwd)),
               b64_encode(seq, strlen(seq)),
               b64_encode(palette_data, strlen(palette_data))
               );

      fsio_append_text_file(kfc_utils_palettes_history_file, hist);
    }
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
  termpaint_terminal_auto_detect_result_text(terminal, buff, sizeof(buff));
  if (termpaint_terminal_self_reported_name_and_version(terminal)) {
    t = strdup_escaped(termpaint_terminal_self_reported_name_and_version(terminal));
  }
  termpaint_terminal_free_with_restore(terminal);
  return(t);
}

struct Vector *kfc_utils_get_invalid_palette_property_names(){
  struct Vector *v = vector_new();
  struct Vector *p = kfc_utils_get_unique_palette_property_names();

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

struct Vector *kfc_utils_get_unique_palette_property_names(){
  struct Vector             *v               = vector_new();
  struct Vector             *palette_names_v = kfc_utils_get_palette_names_v();
  struct palette_property_t *palette_properties_v;

  djbhash_reset_iterator(&palette_properties_h);
  for (size_t i = 0; i < vector_size(palette_names_v); i++) {
    palette_properties_v = kfc_utils_get_palette_name_properties_v((char *)vector_get(palette_names_v, i));
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

static char *kfc_utils_get_cache_ymd(){
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
  char *f = kfc_utils_get_palette_history_file();

  fe->fzf_history_file = f ? f : "/dev/null";
}

char *kfc_utils_select_apply_palette(void){
  char              *selected_palette = NULL;
  struct Vector     *palette_names_v  = kfc_utils_get_palette_names_v();
  struct fzf_exec_t *fe               = exec_fzf_setup();

  assert(fe != NULL);
  {
    kfc_utils_setup_fzf_exec(fe);
    char *env_path = (char *)which_path("env", getenv("PATH"));
    if (fsio_file_exists(env_path)) {
      char pathbuf[PATH_MAX];
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
      fe->debug_mode   = (getenv("DEBUG_MODE") != NULL) ? true : false;
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

char *kfc_utils_select_palette(void *CTX){
  char              *selected_palette = NULL;
  struct Vector     *palette_names_v  = kfc_utils_get_palette_names_v();
  struct fzf_exec_t *fe               = exec_fzf_setup();

  assert(fe != NULL);
  {
    kfc_utils_setup_fzf_exec(fe);
    if ((char *)(((struct ctx_t *)CTX)->palette_name) != NULL) {
      fe->query_s = (char *)(((struct ctx_t *)CTX)->palette_name);
    }else{
      fe->query_s = "";
    }
    fe->select_multiple = false;
    {
      fe->header = AC_RESETALL AC_UNDERLINE AC_BOLD AC_GREEN "Select Palette" AC_RESETALL;
    }
    {
      fe->height       = 100;
      fe->preview_size = 60;
    }
    {
      fe->right_padding  = 0;
      fe->bottom_padding = 0;
      fe->top_padding    = 0;
      fe->left_padding   = 0;
    }
    {
      fe->right_margin  = 0;
      fe->bottom_margin = 0;
      fe->top_margin    = 0;
      fe->left_margin   = 0;
    }
    fe->preview_type = "top";
    fe->debug_mode   = true;
    fe->debug_mode   = false;
    char                *env_path      = (char *)which_path("env", getenv("PATH"));
    struct StringBuffer *fzf_header_sb = stringbuffer_new();
    stringbuffer_append_string(fzf_header_sb, AC_RESETALL AC_ITALIC AC_RED "|n:history-next" AC_RESETALL);
    stringbuffer_append_string(fzf_header_sb, AC_RESETALL AC_ITALIC AC_GREEN "|p:history-prev" AC_RESETALL);
    struct fzf_keybind_t _kb[35] = { 0 };
    char                 pathbuf[PATH_MAX];
    {
      struct fzf_keybind_t *kb19 = &_kb[19];
      kb19->key  = "w";
      kb19->type = "execute-silent";
      asprintf(&kb19->cmd,
               "command curl"
               " --data key=w"
               " --data keymod=ctrl"
               " --data 'item={}'"
               " --data 'pid=%d'"
               " -vk"
               " http://localhost:8199/cmd/n",
               getpid()
               );
      vector_push(fe->fzf_keybinds_v, kb19);
      struct fzf_keybind_t *kb18 = &_kb[18];
      kb18->key  = "w";
      kb18->type = "+change-preview";
      asprintf(&kb18->cmd, "%s -i %s -p {} -E 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb18);
      struct fzf_keybind_t *kb25 = &_kb[25];
      kb25->key  = "w";
      kb25->type = "+change-prompt";
      asprintf(&kb25->cmd, "View Palette Sequence >  ");
      vector_push(fe->fzf_keybinds_v, kb25);

      struct fzf_keybind_t *kb20 = &_kb[20];
      kb20->key  = "w";
      kb20->type = "+change-preview-window";
      asprintf(&kb20->cmd, "wrap,nofollow");
      vector_push(fe->fzf_keybinds_v, kb20);
    }
    {
      struct fzf_keybind_t *kb17 = &_kb[17];
      kb17->key  = "e";
      kb17->type = "execute-silent";
      asprintf(&kb17->cmd,
               "command curl"
               " --data key=n"
               " --data keymod=ctrl"
               " --data 'item={}'"
               " --data 'pid=%d'"
               " -vk"
               " http://localhost:8199/cmd/n",
               getpid()
               );
      vector_push(fe->fzf_keybinds_v, kb17);
      struct fzf_keybind_t *kb24 = &_kb[24];
      kb24->key  = "e";
      kb24->type = "+change-prompt";
      asprintf(&kb24->cmd, "Dump Palette Data >  ");
      vector_push(fe->fzf_keybinds_v, kb24);
      struct fzf_keybind_t *kb16 = &_kb[16];
      kb16->key  = "e";
      kb16->type = "+change-preview";
      asprintf(&kb16->cmd, "%s -i %s -p {} -e 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb16);
    }
    {
      struct fzf_keybind_t *kb = &_kb[0];
      kb->key  = "u";
      kb->type = "execute-silent";
      asprintf(&kb->cmd,
               "command curl"
               " --data key=u"
               " --data keymod=ctrl"
               " --data 'key0=val0'"
               " --data 'key1=val1'"
               " --data 'key2=val2'"
               " --data 'key3=val3'"
               " --data 'item={}'"
               " --data 'pid=%d'"
               " -vk"
               " http://localhost:8199/cmd/u",
               getpid()
               );
      vector_push(fe->fzf_keybinds_v, kb);
    }
    {
      struct fzf_keybind_t *kb13 = &_kb[13];
      kb13->key  = "n";
      kb13->type = "execute-silent";
      asprintf(&kb13->cmd,
               "command curl"
               " --data key=n"
               " --data keymod=ctrl"
               " --data item={}"
               " -vk"
               " http://localhost:8199/cmd/n"
               );

      struct fzf_keybind_t *kb14 = &_kb[14];
      kb14->key  = "n";
      kb14->type = "+change-prompt";
      asprintf(&kb14->cmd, "Select a Bright Palette > ");
      vector_push(fe->fzf_keybinds_v, kb14);

      struct fzf_keybind_t *kb11 = &_kb[11];
      kb11->key  = "n";
      kb11->type = "+change-preview";
      asprintf(&kb11->cmd, "%s -i %s -p {} -T 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb11);

      struct fzf_keybind_t *kb21 = &_kb[21];
      kb21->key  = "n";
      kb21->type = "+change-preview-window";
      asprintf(&kb21->cmd, "nowrap,nofollow");
      vector_push(fe->fzf_keybinds_v, kb21);

      struct fzf_keybind_t *kb15 = &_kb[15];
      kb15->key  = "n";
      kb15->type = "+reload";
      asprintf(&kb15->cmd, "%s -i %s -b -B 99 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb15);
    }

    {
      struct fzf_keybind_t *kb1 = &_kb[1];
      kb1->key  = "t";
      kb1->type = "execute-silent";
      asprintf(&kb1->cmd,
               "command curl"
               " --data key=t"
               " --data keymod=ctrl"
               " --data item={}"
               " -vk"
               " http://localhost:8199/x"
               );

      struct fzf_keybind_t *kb12 = &_kb[12];
      kb12->key  = "t";
      kb12->type = "+change-prompt";
      asprintf(&kb12->cmd, "Select a Dark Palette > ");
      vector_push(fe->fzf_keybinds_v, kb12);

      struct fzf_keybind_t *kb11 = &_kb[11];
      kb11->key  = "t";
      kb11->type = "+change-preview";
      asprintf(&kb11->cmd, "%s -i %s -p {} -T 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb11);
      struct fzf_keybind_t *kb26 = &_kb[26];
      kb26->key  = "t";
      kb26->type = "+change-preview-window";
      asprintf(&kb26->cmd, "nowrap,nofollow");
      vector_push(fe->fzf_keybinds_v, kb26);

      struct fzf_keybind_t *kb10 = &_kb[10];
      kb10->key  = "t";
      kb10->type = "+reload";
      asprintf(&kb10->cmd, "%s -i %s -k -B 1 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb10);
    }

    {
      struct fzf_keybind_t *kb27 = &_kb[27];
      kb27->key  = "a";
      kb27->type = "execute-silent";
      asprintf(&kb27->cmd, "%s -i %s -p {} -L >/dev/tty 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb27);
    }

    {
      struct fzf_keybind_t *kb3 = &_kb[3];
      kb3->key  = "y";
      kb3->type = "+change-prompt";
      asprintf(&kb3->cmd, "New Prompt for y >  ");
      vector_push(fe->fzf_keybinds_v, kb3);

      struct fzf_keybind_t *kb8 = &_kb[8];
      kb8->key  = "y";
      kb8->type = "+change-preview";
      asprintf(&kb8->cmd, "%s -i %s -p {} -E 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb8);

      struct fzf_keybind_t *kb9 = &_kb[9];
      kb9->key  = "y";
      kb9->type = "+reload";
      asprintf(&kb9->cmd, "%s -i %s -l 2>/dev/null",
               env_path,
               pathbuf
               );
      vector_push(fe->fzf_keybinds_v, kb9);
    }
    {
      {
        struct fzf_keybind_t *kb4 = &_kb[4];
        kb4->key  = "i";
        kb4->type = "change-prompt";
        asprintf(&kb4->cmd, "New Prompt for i >  ");
        vector_push(fe->fzf_keybinds_v, kb4);
        struct fzf_keybind_t *kb5 = &_kb[5];
        kb5->key  = "i";
        kb5->type = "+change-preview";
        asprintf(&kb5->cmd, "%s -i %s -p {} -T 2>/dev/null",
                 env_path,
                 pathbuf
                 );
        vector_push(fe->fzf_keybinds_v, kb5);

        struct fzf_keybind_t *kb6 = &_kb[7];
        kb6->key  = "i";
        kb6->type = "+reload";
        asprintf(&kb6->cmd, "%s -i %s -l 2>/dev/null",
                 env_path,
                 pathbuf
                 );
        vector_push(fe->fzf_keybinds_v, kb6);

        struct fzf_keybind_t *kb22 = &_kb[22];
        kb22->key  = "i";
        kb22->type = "+change-preview-window";
        asprintf(&kb22->cmd, "nowrap,nofollow");
        vector_push(fe->fzf_keybinds_v, kb22);

        struct fzf_keybind_t *kb2 = &_kb[2];
        kb2->key  = "i";
        kb2->type = "+execute-silent";
        asprintf(&kb2->cmd,
                 "command curl"
                 " --data key=y"
                 " --data env_path='%s'"
                 " --data path_buf='%s'"
                 " --data keymod=ctrl"
                 " --data item={}"
                 " -vk"
                 " http://localhost:8199///cmd///y///",
                 env_path, pathbuf
                 );
        vector_push(fe->fzf_keybinds_v, kb2);
      }
    }
  }

  for (size_t i = 0; i < vector_size(palette_names_v); i++) {
    vector_push(fe->input_options, (char *)vector_get(palette_names_v, i));
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
  struct Vector     *palette_names_v = kfc_utils_get_palette_names_v();
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

static char *kfc_utils_get_palette_history_file(){
  if (!fsio_file_exists(FZF_UTILS_DEFAULT_HISTORY_FILE)) {
    return(NULL);
  }
  return(FZF_UTILS_DEFAULT_HISTORY_FILE);
}

char *kfc_utils_get_palette_history(){
  char *f = kfc_utils_get_palette_history_file();

  if (!fsio_file_exists(f)) {
    return(NULL);
  }
  char *s = stringfn_mut_trim(fsio_read_text_file(f));

  return(s);
}

char *kfc_utils_get_ansi_reset_sequence(){
  return("\ec");
}

#define template_fmt    ""                       \
  "file:%s|size:%lu|data_name:%s|dir:%s|path:%s" \
  "|data fillname:%s|"                           \
  "|# lines:%lu|"
#define template_info(tmp)      \
  log_info(template_fmt         \
           , tmp->file          \
           , tmp->bytes         \
           , tmp->data_name     \
           , tmp->dir           \
           , tmp->path          \
           , tmp->data_fullname \
           , tmp->lines_qty     \
           )

struct vector *kfc_utils_load_palettes(const char *PATH){
  struct Vector *pf = vector_new();
  tinydir_dir   dir;

  if (tinydir_open(&dir, (char *)PATH) == -1) {
    log_info("Loading EmbeddedKfcUtilsPaletteTemplate of %lu bytes", sizeof(inc_palette_EmbeddedKfcUtilsPaletteTemplate_size));
    log_info("%s", inc_palette_EmbeddedKfcUtilsPaletteTemplate_data);
    if (strlen(inc_palette_EmbeddedKfcUtilsPaletteTemplate_data) > 32) {
      //char *f;
      //  asprintf(&f,"%s%s.j2",gettempdir(),"embedded_palettes.j2");
      //    fsio_write_text_file(f,inc_palette_EmbeddedKfcUtilsPaletteTemplate_data);
//      vector_push(pf, f);
    }else{
      goto bail;
    }
  }

  while (dir.has_next) {
    tinydir_file file;
    if (tinydir_readfile(&dir, &file) == -1) {
      perror("Error getting file");
      goto bail;
    }

    if (!file.is_dir) {
      char *f = malloc(1024);
      sprintf(f, "%s/%s", PATH, file.name);
      vector_push(pf, f);
    }

    if (tinydir_next(&dir) == -1) {
      perror("Error getting next file");
      goto bail;
    }
  }

  tinydir_close(&dir);
  return(pf);

bail:
  tinydir_close(&dir);
} /* kfc_utils_load_palettes */

struct palette_template_item_t {
  char          *path, *data_name, *dir, *name, *file, *data_fullname, *bytes_s;
  size_t        bytes, lines_qty, *content;
  struct Vector *keys_v;
} palette_template_item_t;

static char *kfc_utils_render_palettes_template(struct Vector *__template_palettes_v){
  char *TEMPLATE_s, *TEMPLATE_s_rendered;

  roscha_init();
  struct roscha_env *env = roscha_env_new();

  env->vars = roscha_object_new(hmap_new());
  struct roscha_object *palette_t_list = roscha_object_new_vector(roscha_vector_new());

  for (size_t i = 0; i < vector_size(__template_palettes_v) && i < PALETTES_QTY_LIMIT_LOAD; i++) {
    struct palette_template_item_t *t  = vector_get(__template_palettes_v, i);
    struct roscha_object           *i0 = roscha_object_new(hmap_new());
    roscha_hmap_set_new(i0, "name", t->name);
    roscha_hmap_set_new(i0, "inc_name", stringfn_replace(t->name, '-', '_'));
    roscha_hmap_set_new(i0, "dir", t->dir);
    roscha_hmap_set_new(i0, "size", (int)t->bytes);
    roscha_hmap_set_new(i0, "dir", t->dir);
    roscha_vector_push_object(palette_t_list, i0);
    roscha_hmap_set(env->vars, "palette_t_list", palette_t_list);
    roscha_hmap_set_new(env->vars, "palettes_qty", (int)(size_t)vector_size(__template_palettes_v));
    roscha_hmap_set_new(env->vars, "LEFT_CURLY", "{");
    roscha_hmap_set_new(env->vars, "RIGHT_CURLY", "}");
    roscha_hmap_set_new(env->vars, "inc_palette_t_struct", INC_PALETTE_T_STRING());
    if (KFC_UTILS_DEBUG_MODE == true) {
      log_debug("added palette # %lu :: %s", i, t->name);
    }
  }

  if (fsio_file_exists(PALETTES_JINJA_TEMPLATE) == 0) {
    fprintf(stderr, AC_RESETALL AC_RED_BLACK "Template File does not exist. Loading from embeded text" AC_RESETALL "\n");
    asprintf(&TEMPLATE_s, "%s", inc_palette_EmbeddedKfcUtilsPaletteTemplate_data);
    log_debug(AC_RESETALL AC_RED_BLACK "template_s :\n%s\n", TEMPLATE_s);
  }else{
    if (false) {
      fprintf(stderr, AC_RESETALL AC_GREEN_BLACK "Template File does exist. Reading from it." AC_RESETALL "\n");
    }
    TEMPLATE_s = fsio_read_text_file(PALETTES_JINJA_TEMPLATE);
  }
  if (strlen(TEMPLATE_s) == 0) {
    TEMPLATE_s = strdup(inc_palette_EmbeddedKfcUtilsPaletteTemplate_data);
  }
  roscha_env_add_template(env, strdup("test"), TEMPLATE_s);
  sds RENDERED_ROSCHA_TEMPLATE = roscha_env_render(env, "test");

  asprintf(&TEMPLATE_s_rendered, "%s", RENDERED_ROSCHA_TEMPLATE);
  fsio_write_text_file(KFC_UTILS_RENDERED_PALETTE_COPY_PATH, TEMPLATE_s_rendered);
  roscha_deinit();
  return(TEMPLATE_s_rendered);
} /* kfc_utils_render_palettes_template */

struct rendered_template_result_t *kfc_utils_get_rendered_template(void){
  struct rendered_template_result_t *res = malloc(sizeof(struct rendered_template_result_t));

  res->palette_file_bytes      = 0;
  res->palette_file_lines      = 0;
  res->palette_file_properties = 0;

  res->started       = timestamp();
  res->path          = KFC_UTILS_RENDERED_PALETTE_COPY_PATH;
  res->template_path = KFC_UTILS_PALETTE_TEMPLATE_FILE;
  char          *P                     = PALETTES_LOAD_DIR;
  struct Vector *pfiles                = kfc_utils_load_palettes(P);
  struct Vector *__template_palettes_v = vector_new();
  struct Vector *palette_file_paths    = vector_new();

  for (size_t i = 0; i < vector_size(pfiles); i++) {
    vector_push(palette_file_paths, (char *)vector_get(pfiles, i));
  }

  for (size_t i = 0; i < vector_size(palette_file_paths) && i < PALETTES_QTY_LIMIT_LOAD; i++) {
    char *pf = (char *)vector_get(palette_file_paths, i);
    if (pf == NULL) {
      continue;
    }
    struct { char *path; } *p = malloc(PATH_MAX);
    p->path = strdup(pf);
    if (p == NULL || p->path == NULL || strlen(p->path) < 1) {
      break;
    }
    struct palette_template_item_t *tmp = malloc(sizeof(struct palette_template_item_t));
    tmp->path = strdup(p->path);
    if (!fsio_file_exists(tmp->path)) {
      continue;
    }
    tmp->bytes = fsio_file_size(tmp->path);
    if (tmp->bytes < 2) {
      continue;
    }
    tmp->keys_v  = vector_new();
    tmp->dir     = dirname(tmp->path);
    tmp->file    = basename(tmp->path);
    tmp->name    = strdup(tmp->file);
    tmp->content = fsio_read_text_file(tmp->path);
    struct StringFNStrings lines = stringfn_split_lines_and_trim(tmp->content);
    tmp->lines_qty = lines.count;
    tmp->data_name = stringfn_replace(tmp->file, '-', '_');
    asprintf(&tmp->data_fullname, "inc_palette_%s_data", tmp->data_name);
    for (size_t ii = 0; ii < (size_t)lines.count; ii++) {
      struct StringFNStrings _tss = stringfn_split(lines.strings[ii], '=');
      if (_tss.count > 1) {
        vector_push(tmp->keys_v, strdup(_tss.strings[0]));
        res->palette_file_properties += 1;
      }
    }

    stringfn_release_strings_struct(lines);
    vector_push(__template_palettes_v, tmp);
    res->palette_file_lines += tmp->lines_qty;
    res->palette_file_bytes += strlen(tmp->content);
    p++;
  }
  res->content = kfc_utils_render_palettes_template(__template_palettes_v);
  res->size    = strlen(res->content);
  res->dur     = timestamp() - res->started;
  res->qty     = vector_size(__template_palettes_v);

  return(res);
} /* kfc_utils_get_rendered_template */

bool kfc_utils_palette_property_is_color(const char *PALETTE_PROPERTY_NAME){
  bool          is_color = false;
  struct Vector *props   = kfc_utils_get_palette_property_color_names_v();

  for (size_t i = 0; i < vector_size(props); i++) {
    if (strcmp(PALETTE_PROPERTY_NAME, (char *)vector_get(props, i)) == 0) {
      is_color = true;
      goto done;
    }
  }
done:
  vector_release(props);
  return(is_color);
}

struct Vector *kfc_utils_get_palette_property_color_names_v(){
  struct Vector *v    = vector_new();
  char          **tmp = palette_color_properties;

  while (*tmp != NULL) {
    vector_push(v, *tmp);
    tmp++;
  }
  return(v);
}

char *kfc_utils_get_palette_colored_properties(const char *PALETTE_NAME){
  struct StringBuffer       *sb                   = stringbuffer_new();
  struct palette_property_t *palette_properties_v = kfc_utils_get_palette_name_properties_v(PALETTE_NAME);

  if (vector_size(palette_properties_v) < 1) {
    return("No properties found");
  }
  size_t spaces_qty = 1;

  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    struct palette_property_t *pp = vector_get(palette_properties_v, i);
    if (strlen(pp->translated_name) > spaces_qty) {
      spaces_qty = strlen(pp->translated_name);
    }
  }
  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    struct palette_property_t *pp = vector_get(palette_properties_v, i);
    if (pp->is_color == false) {
      continue;
    }
    stringbuffer_append_string(sb, pp->translated_name);
    for (size_t si = 0; si < (spaces_qty - strlen(pp->translated_name)) + 10; si++) {
      stringbuffer_append_string(sb, " ");
    }
    stringbuffer_append_string(sb,
                               kfc_utils_apply_sequence(
                                 pp->color_t->sequence,
                                 pp->translated_value
                                 )
                               );
    stringbuffer_append_string(sb, "                ");
    stringbuffer_append_string(sb, pp->color_t->escaped_sequence);
    stringbuffer_append_string(sb, "\n");
  }

  char *ret = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  stringfn_mut_trim(ret);
  return(ret);
} /* kfc_utils_get_palette_colored_properties */

char *kfc_utils_get_cwd(void){
  char *buf = malloc(PATH_MAX);

  if (NULL == getcwd(buf, PATH_MAX)) {
    perror("getcwd error");
    return(NULL);
  }
  char *buf1 = malloc(PATH_MAX);

  if (NULL == realpath(buf, buf1)) {
    perror("realpath error");
    return(NULL);
  }
  return(buf1);
}
