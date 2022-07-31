#pragma once
#define KFC_LOG_DEFAULT    KFC_LOG_INFO


////////////////////////////////////
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <libproc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
////////////////////////////////////
#include "cargs/include/cargs.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
////////////////////////////////////
#include "kfc-cli/kfc-cli-exports.h"
////////////////////////////////////
size_t get_palettes_qty(void);
int kfc_cli_list_palettes(void);

////////////////////////////////////
#define KFC_UTILS_COMMON_COMMON_PROPERTIES(NAME) \
  define(NAME, CLIB_MODULE);                     \
  enum kfc_utils_mode mode;

#define KFC_UTILS_COMMON_COMMON_MODULE(NAME) \
    .mode = KFC_LOG_DEFAULT,

#define KFC_UTILS_COMMON_COMMON_FUNCTIONS(NAME)     \
  int NAME ## _module_init(module(NAME) * exports); \
  void NAME ## _module_deinit(module(NAME) * exports);

#define KFC_UTILS_COMMON_COMMON_EXPORTS(NAME) \
    .mode = KFC_LOG_DEFAULT,                  \
  .init   = NAME ## _module_init,             \
  .deinit = NAME ## _module_deinit,

//////////////////////////////////////////////////
static struct ctx_t               kfc_utils_ctx = {
  .palette_name         = NULL,
  .palette_property     = NULL,
  .palette_value        = NULL,
  .random_palette_index = -1,
  .debug_mode           = false,
  .max_brightness       = DEFAULT_MAX_BRIGHTNESS,
  .mode                 = KFC_CLI_MODE_LIST_PALETTES,
};
static struct kfc_mode_handlers_t cli_modes[KFC_CLI_MODES_QTY] = { 0 };

//////////////////////////////////////////////////////////////////////////
module(kfc_utils_maps) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_maps)

  struct djbhash      palette_properties_h, valid_palette_property_names_h, invalid_palette_property_names_h;
  struct djbhash_node *hash_item;
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_maps)
exports(kfc_utils_maps) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_maps)
    .palette_properties_h           = NULL,
  .valid_palette_property_names_h   = NULL,
  .invalid_palette_property_names_h = NULL,
  .hash_item                        = NULL,
};

//////////////////////////////////////////////////////////////////////////
module(kfc_utils_table) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_table)
  char *(*get_palette_properties_table)(const char *PALETTE_NAME);
  char *(*get_palettes_table)(void);
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_table)
exports(kfc_utils_table) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_table)
    .get_palette_properties_table = kfc_utils_get_palette_properties_table,
  .get_palettes_table             = kfc_utils_get_palettes_table,
};
//////////////////////////////////////////////////////////////////////////
module(kfc_utils_cache) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_cache)
  struct kfc_utils_cache_files_t CACHE_ITEMS[CACHE_ITEMS_QTY + 1];
  char                           *CACHE_FILES[CACHE_ITEMS_QTY + 1];
  char                           *(*get_cache_ymd)(void);
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_cache)
exports(kfc_utils_cache) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_cache)
    .get_cache_ymd         = kfc_utils_get_cache_ymd,
  .CACHE_FILES             = {
    [CACHE_PALETTES_TABLE] = NULL,
    [CACHE_ITEMS_QTY]      = NULL,
  },
  .CACHE_ITEMS             = {
    [CACHE_PALETTES_TABLE] = { .name = "palettes-table-cache", },
    [CACHE_ITEMS_QTY]      = { 0 },
  },
};
//////////////////////////////////////////////////
module(kfc_utils_modes) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_modes)
  struct kfc_mode_handlers_t modes[KFC_CLI_MODES_QTY + 1];
};
module(kfc_utils_table) * kfc_utils_table;
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_modes)
exports(kfc_utils_modes) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_modes)
    .modes                                          = {
    [KFC_CLI_MODE_PRINT_USAGE]                      = { .handler = kfc_cli_print_usage,                      },
    [KFC_CLI_MODE_LIST_PALETTES]                    = { .handler = kfc_cli_list_palettes,                    },
    [KFC_CLI_MODE_LOAD_PALETTE]                     = { .handler = kfc_cli_load_palette,                     },
    [KFC_CLI_MODE_PRINT_PALETTES_TABLE]             = { .handler = kfc_cli_print_palettes_table,             },
    [KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE]   = { .handler = kfc_cli_print_palette_properties_table,   },
    [KFC_CLI_MODE_PRINT_VERSION]                    = { .handler = kfc_cli_print_version,                    },
    [KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES]  = { .handler = kfc_cli_print_unique_palette_properties,  },
    [KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES] = { .handler = kfc_cli_print_invalid_palette_properties, },
    [KFC_CLI_MODE_DETECT_TERMINAL_TYPE]             = { .handler = kfc_cli_detect_terminal_type,             },
    [KFC_CLI_MODE_TEST_KITTY_SOCKET]                = { .handler = kfc_cli_test_kitty_socket,                },
    [KFC_CLI_MODE_TEST_COLORS]                      = { .handler = kfc_cli_test_colors,                      },
    [KFC_CLI_MODE_BRIGHT_BACKGROUNDS]               = { .handler = kfc_cli_bright_backgrounds,               },
    [KFC_CLI_MODE_DARK_BACKGROUNDS]                 = { .handler = kfc_cli_dark_backgrounds,                 },
    [KFC_CLI_MODE_SELECT_PALETTE]                   = { .handler = kfc_cli_select_palette,                   },
    [KFC_CLI_MODE_SELECT_APPLY_PALETTE]             = { .handler = kfc_cli_select_apply_palette,             },
    [KFC_CLI_MODE_SELECT_PALETTES]                  = { .handler = kfc_cli_select_palettes,                  },
    [KFC_CLI_MODE_COLOR_REPORT]                     = { .handler = kfc_cli_color_report,                     },
    [KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE]   = { .handler = kfc_cli_print_escaped_sequence,           },
    [KFC_CLI_MODE_PRINT_PALETTE_DATA]               = { .handler = kfc_cli_print_palette_data,               },
    [KFC_CLI_MODE_RESET_TERMINAL]                   = { .handler = kfc_cli_reset_terminal,                   },
    [KFC_CLI_MODE_PRINT_PALETTE_HISTORY]            = { .handler = kfc_cli_print_palette_history,            },
    [KFC_CLI_MODES_QTY]                             = { 0 },
  },
};
//////////////////////////////////////////////////

module(kfc_utils_terminal) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_terminal)
  char                         *(*kfc_utils_detect_terminal_type)();
  const char                   *ANSI_CODES[ANSI_CODES_QTY + 1];
  struct terminal_type_names_t terminal_type_names[TERMINAL_TYPES_QTY + 1];
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_terminal)
exports(kfc_utils_terminal) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_terminal)
    .terminal_type_names      = {
    [TERMINAL_TYPE_KITTY]     = { .name = "kitty", .env_key     = "KITTY_PID", .env_val        = NULL,             },
    [TERMINAL_TYPE_ALACRITTY] = { .name = "alacritty", .env_key = "ALACRITTY_SOCKET", .env_val = NULL,             },
    [TERMINAL_TYPE_ITERM2]    = { .name = "iterm2", .env_key    = "TERM_PROGRAM", .env_val     = "iTerm.app",      },
    [TERMINAL_TYPE_TERMINAL]  = { .name = "terminal", .env_key  = "TERM_PROGRAM", .env_val     = "Apple_Terminal", },
    [TERMINAL_TYPES_QTY]      = { 0 },
  },
  .ANSI_CODES           = {
    [ANSI_CODE_ESC]     = "\x1b",
    [ANSI_CODE_PREFIX]  = "\\",
    [ANSI_CODES_SUFFIX] = "\[21D",
    [ANSI_CODES_QTY]    = NULL,
  },
};
enum cli_arg_options_t {
  PRINT_PALETTE_HISTORY,
  RESET_TERMINAL_STATE,
  PRINT_PALETTE_DATE,
  ESCAPTED_SEQUENCE,
  COLOR_REPORT,
  SELECT_APPLY_PALETTES,
  SELECT_PALETTES,
  SELECT_PALETTE,
  DARK_BACKGROUNDS,
  BRIGHT_BACKGROUNDS,
  MAX_BRIGHTNESS,
  TEST_COLORS,
  TEST_KITTY,
  TERMINAL_TEST,
  UNIQUE_PALETTE_ITEMS,
  INVALID_PALETTE_ITEMS,
  LOAD_PALETTE,
  LIST_PALETTES,
  KFC_CLI_DEBUG_MODE,
  SPECIFIED_PALETTE,
  SPECIFIED_PALETTE_VALUE,
  SPECIFIED_PALETTE_PROPERTY,
  PRINT_VERSION,
  PRINT_PALETTES_TABLE,
  PRINT_PALETTE_TABLE,
  LOAD_RANDOM_PALETTE,
  LIMIT_PALETTES_QTY,
  SHOW_HELP,
  CLI_ARG_OPTIONS_QTY,
};
////////////////////////////////////
module(kfc_utils_cli) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_cli)
  const char *EXECUTABLE_ABSOLUTE;
  int                (*parse_args)(const int argc, const char *argv[]);
  void               (*prepare_args)(cag_option_context *context, const cag_option *options, size_t option_count, int argc, char **argv);
  bool               (*get_args_option)(cag_option_context *context);
  const char         (*get_args_identifier)(cag_option_context *context);
  const char         *palette_property;
  cag_option_context args_context;
  struct ctx_t       *ctx;
  struct cag_option  arg_options[CLI_ARG_OPTIONS_QTY];
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_cli)
exports(kfc_utils_cli) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_cli)
    .EXECUTABLE_ABSOLUTE                                           = NULL,
  .args_context                                                    = NULL,
  .parse_args                                                      = parse_args,
  .prepare_args                                                    = cag_option_prepare,
  .get_args_option                                                 = cag_option_fetch,
  .get_args_identifier                                             = cag_option_get,
  .ctx                                                             = &kfc_utils_ctx,
  .arg_options                                                     = {
    [PRINT_PALETTE_HISTORY]                                        = { .identifier = 'G',                     .access_letters = "G",
                                                      .access_name = "print-palette-history", .value_name     = NULL,.description= "Print Palette History" },
    [RESET_TERMINAL_STATE]                                         = { .identifier = 'Q',              .access_letters = "Q",
                                                      .access_name = "reset-terminal", .value_name     = NULL,.description= "Reset Terminal State" },
    [PRINT_PALETTE_DATE]                                           = { .identifier = 'e',                  .access_letters = "e",
                                                      .access_name = "print-palette-data", .value_name     = NULL,.description= "Print Palettte Data" },
    [ESCAPTED_SEQUENCE]                                            = { .identifier = 'E',                .access_letters = "E",
                                                      .access_name = "escaped-sequence", .value_name     = NULL,.description= "Escaped Sequence" },
    [COLOR_REPORT]                                                 = { .identifier = 'R',            .access_letters = "R",
                                                      .access_name = "color-report", .value_name     = NULL,.description= "Color Report" },
    [SELECT_APPLY_PALETTES]                                        = { .identifier = 'A',                    .access_letters = "A",
                                                      .access_name = "select-apply-palette", .value_name     = NULL,.description= "Select & Apply Palettes" },
    [SELECT_PALETTES]                                              = { .identifier = 'S',               .access_letters = "S",
                                                      .access_name = "select-palettes", .value_name     = NULL,.description= "Select Palettes" },
    [SELECT_PALETTE]                                               = { .identifier = 's',              .access_letters = "s",
                                                      .access_name = "select-palette", .value_name     = NULL,.description= "Select Palette" },
    [DARK_BACKGROUNDS]                                             = { .identifier = 'k',                .access_letters = "k",
                                                      .access_name = "dark-backgrounds", .value_name     = NULL,.description= "Dark Backgrounds" },
    [BRIGHT_BACKGROUNDS]                                           = { .identifier = 'b',                  .access_letters = "b",
                                                      .access_name = "bright-backgrounds", .value_name     = NULL,.description= "Bright Backgrounds" },
    [MAX_BRIGHTNESS]                                               = { .identifier = 'B',              .access_letters = "B",
                                                      .access_name = "max-brightness", .value_name     = "MAX_BRIGHTNESS",.description= "Max Brightness (0.00 - 100)" },
    [TEST_COLORS]                                                  = { .identifier = 'C',           .access_letters = "C",
                                                      .access_name = "test-colors", .value_name     = NULL,.description= "Test Colors" },
    [TEST_KITTY]                                                   = { .identifier = 'K',                 .access_letters = "K",
                                                      .access_name = "test-kitty-socket", .value_name     = NULL,.description= "Test Kitty Socket" },
    [TERMINAL_TEST]                                                = { .identifier = 'D',               .access_letters = "D",
                                                      .access_name = "detect-terminal", .value_name     = NULL,.description= "Detect Terminal Type" },
    [UNIQUE_PALETTE_ITEMS]                                         = { .identifier = 'U',            .access_letters = "U",
                                                      .access_name = "unique-items", .value_name     = NULL,.description= "List Unique Palette Items" },
    [INVALID_PALETTE_ITEMS]                                        = { .identifier = 'I',             .access_letters = "I",
                                                      .access_name = "invalid-items", .value_name     = NULL,.description= "List Invalid Palette Items" },
    [LOAD_PALETTE]                                                 = { .identifier = 'L',            .access_letters = "L",
                                                      .access_name = "load-palette", .value_name     = NULL,.description= "Load Palette" },
    [LIST_PALETTES]                                                = { .identifier = 'l',             .access_letters = "l",
                                                      .access_name = "list-palettes", .value_name     = NULL,.description= "List Palettes" },
    [KFC_CLI_DEBUG_MODE]                                           = { .identifier = 'd',     .access_letters = "d",
                                                      .access_name = "debug", .value_name     = NULL,.description= "Debug Mode" },
    [SPECIFIED_PALETTE]                                            = { .identifier = 'p',       .access_letters = "p",
                                                      .access_name = "palette", .value_name     = "PALETTE_NAME",.description= "Specified Palette" },
    [SPECIFIED_PALETTE_VALUE]                                      = { .identifier = 'V',             .access_letters = "V",
                                                      .access_name = "palette-value", .value_name     = "PALETTE_VALUE",.description= "Specified Palette Value" },
    [SPECIFIED_PALETTE_PROPERTY]                                   = { .identifier = 'P',                .access_letters = "P",
                                                      .access_name = "palette-property", .value_name     = "PALETTE_PROPERTY",
                                                      .description = "Specified Palette Property" },
    [PRINT_VERSION]                                                = { .identifier = 'v',       .access_letters = "v",
                                                      .access_name = "version", .value_name     = NULL,.description= "Print Version" },
    [PRINT_PALETTES_TABLE]                                         = { .identifier = 't',              .access_letters = "t",
                                                      .access_name = "palettes-table", .value_name     = NULL,.description= "Print Palettes Table" },
    [PRINT_PALETTE_TABLE]                                          = { .identifier = 'T',             .access_letters = "T",
                                                      .access_name = "palette-table", .value_name     = NULL,.description= "Print Palette Table" },
    [LOAD_RANDOM_PALETTE]                                          = { .identifier = 'r',              .access_letters = "r",
                                                      .access_name = "random-palette", .value_name     = NULL,.description= "Load Random Palette" },
    [LIMIT_PALETTES_QTY]                                           = { .identifier = 'q',                  .access_letters = "q",
                                                      .access_name = "limit-palettes-qty", .value_name     = "PALETTES_QTY",.description= "Limit Palettes Qty" },
    [SHOW_HELP]                                                    = { .identifier = 'h',    .access_letters = "h",
                                                      .access_name = "help", .description    = "Shows the command help" },
  },
};
////////////////////////////////////
module(kfc_utils_kitty) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_kitty)
  bool (*kfc_utils_test_kitty_socket)();
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_kitty)
exports(kfc_utils_kitty) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_kitty)
};
////////////////////////////////////
module(kfc_utils_palette_property) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_palette_property)
  struct djbhash                                 palette_properties_h, valid_palette_property_names_h, invalid_palette_property_names_h;
  struct djbhash_node                            *hash_item;
  struct Vector                                  *(*get_palette_name_properties_v)(const char *PALETTE_NAME);
  struct Vector                                  *(*get_unique_palette_property_names)();
  struct Vector                                  *(*get_invalid_palette_property_names)();
  struct Vector                                  *(*get_palette_names_by_brightness_type)(int BACKGROUND_BRIGHTNESS_TYPE, float BRIGHTNESS_THRESHOLD);
  char                                           *(*get_palette_item_code)(const char *PALETTE_ITEM_NAME);
  bool                                           (*palette_item_name_is_translated)(const char *ITEM_NAME);
  char                                           *(*translate_palette_item_value)(const char *ITEM_NAME, const char *ITEM_VALUE);
  bool                                           (*palette_background_is_brightness_type)(char *BACKGROUND_COLOR, int BACKGROUND_BRIGHTNESS_TYPE, double BRIGHTNESS_THRESHOLD);
  char                                           *(*get_palette_item_sequence)(const struct palette_property_t *pp);
  char                                           *(*get_palette_name_sequence)(const char *PALETTE_NAME);
  bool                                           (*is_valid_palette_item_name)(const char *PALETTE_ITEM_NAME);
  char                                           *(*get_translated_palette_property_name)(const char *PALETTE_PROPERTY_NAME);
  char                                           *(*get_palette_history_file)(void);
  const struct palette_code_value_translations_t palette_code_value_translations[100];
  const struct palette_name_translations_t       palette_name_translations[100];
  const struct                palette_code_t     palette_codes[];
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_palette_property)
exports(kfc_utils_palette_property) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_palette_property)
    .palette_codes = {
    { .name        = "color00",              .code = "]4;0;#",              },
    { .name        = "color01",              .code = "]4;1;#",              },
    { .name        = "color02",              .code = "]4;2;#",              },
    { .name        = "color03",              .code = "]4;3;#",              },
    { .name        = "color04",              .code = "]4;4;#",              },
    { .name        = "color05",              .code = "]4;5;#",              },
    { .name        = "color06",              .code = "]4;6;#",              },
    { .name        = "color07",              .code = "]4;7;#",              },
    { .name        = "color08",              .code = "]4;8;#",              },
    { .name        = "color09",              .code = "]4;9;#",              },
    { .name        = "color10",              .code = "]4;10;#",             },
    { .name        = "color11",              .code = "]4;11;#",             },
    { .name        = "color12",              .code = "]4;12;#",             },
    { .name        = "color13",              .code = "]4;13;#",             },
    { .name        = "color14",              .code = "]4;14;#",             },
    { .name        = "color15",              .code = "]4;15;#",             },
    { .name        = "color16",              .code = "]4;16;#",             },
    { .name        = "color17",              .code = "]4;17;#",             },
    { .name        = "color18",              .code = "]4;18;#",             },
    { .name        = "color19",              .code = "]4;19;#",             },
    { .name        = "color20",              .code = "]4;20;#",             },
    { .name        = "color21",              .code = "]4;21;#",             },
    { .name        = "color22",              .code = "]4;22;#",             },
    { .name        = "color23",              .code = "]4;23;#",             },
    { .name        = "color24",              .code = "]4;24;#",             },
    { .name        = "color25",              .code = "]4;25;#",             },
    { .name        = "foreground",           .code = "]10;#",               },
    { .name        = "background",           .code = "]11;#",               },
    { .name        = "selection_background", .code = "]17;#",               },
    { .name        = "selection_foreground", .code = "]19;#",               },
    { .name        = "title",                .code = "]2;",                 },
    { .name        = "cursor",               .code = "]12;#",               },
    { .name        = "cursorvisible",        .code = "[?25",                },
    { .name        = "cursorstyle",          .code = "[",                   },
    { .name        = "cursorblink",          .code = "[?12",                },
    { .name        = "altscreen",            .code = "[?47",                },
    { .name        = "screen",               .code = "[?5",                 },
    { .name        = "icontitle",            .code = "]0;",                 },
    { .name        = "icon",                 .code = "]1;",                 },
    { .name        = "title",                .code = "]2;",                 },
    { .name        = "reportfocus",          .code = "[?1004",              },
    { .name        = "mouse",                .code = "[?100",               },
    { .name        = "notification",         .code = "]99;i=1:d=1:p=body;", },
    { .name        = "reset",                .code = "",                    },
    { 0 },
  },
  .palette_code_value_translations = {
    { .name                        = "cursorstyle",   .src = "under",     .dst = "3 q", },
    { .name                        = "cursorstyle",   .src = "block",     .dst = "1 q", },
    { .name                        = "cursorstyle",   .src = "bar",       .dst = "5 q", },
    { .name                        = "cursorblink",   .src = "off",       .dst = "l",   },
    { .name                        = "cursorblink",   .src = "on",        .dst = "h",   },
    { .name                        = "altscreen",     .src = "off",       .dst = "l",   },
    { .name                        = "altscreen",     .src = "on",        .dst = "h",   },
    { .name                        = "screen",        .src = "off",       .dst = "l",   },
    { .name                        = "screen",        .src = "on",        .dst = "h",   },
    { .name                        = "cursorvisible", .src = "off",       .dst = "l",   },
    { .name                        = "cursorvisible", .src = "on",        .dst = "h",   },
    { .name                        = "reportfocus",   .src = "off",       .dst = "l",   },
    { .name                        = "reportfocus",   .src = "on",        .dst = "h",   },
    { .name                        = "mouse",         .src = "clickdrag", .dst = "2h",  },
    { .name                        = "mouse",         .src = "motion",    .dst = "3h",  },
    { .name                        = "mouse",         .src = "click",     .dst = "0h",  },
    { .name                        = "mouse",         .src = "off",       .dst = "0l",  },
    { .name                        = "reset",         .src = "yes",       .dst = "c",   },
    { .name                        = "reset",         .src = "no",        .dst = "",    },
    { 0 },
  },
  .palette_name_translations = {
    { .src                   = "color0",      .dst = "color00", },
    { .src                   = "color1",      .dst = "color01", },
    { .src                   = "color2",      .dst = "color02", },
    { .src                   = "color3",      .dst = "color03", },
    { .src                   = "color4",      .dst = "color04", },
    { .src                   = "color5",      .dst = "color05", },
    { .src                   = "color6",      .dst = "color06", },
    { .src                   = "color7",      .dst = "color07", },
    { .src                   = "color8",      .dst = "color08", },
    { .src                   = "color9",      .dst = "color09", },
    { .src                   = "cursorcolor", .dst = "cursor",  },
    { 0 },
  },
};
////////////////////////////////////
////////////////////////////////////
module(kfc_utils_palette) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_palette)
  struct inc_palette_t *(*get_palette_t_by_index)(const size_t PALETTE_INDEX);
  struct inc_palette_t *(*get_palette_t_by_name)(char *PALETTE_NAME);
  char                 (*get_palette_name_property_value)(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME);
  size_t               (*load_palette_name)(const char *PALETTE_NAME);
  char                 *(*get_palette_name_by_index)(const size_t PALETTE_INDEX);
  char                 *(*select_palette)(void);
  char                 *(*select_apply_palette)(void);
  char                 *(*get_palette_name_sequence)(const char *PALETTE_NAME);
  char                 *(*get_palette_name_data)(const char *PALETTE_NAME);
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_palette)
exports(kfc_utils_palette) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_palette)
};
////////////////////////////////////
module(kfc_utils_palettes) {
  KFC_UTILS_COMMON_COMMON_PROPERTIES(kfc_utils_palettes)
  size_t palettes_qty, palettes_data_bytes;
  size_t                     palettes_limit_qty;
  const struct Vector        *palettes_v, *palette_names_v;
  const struct inc_palette_t **palettes;
  struct Vector              *(*get_palettes_v)(void);
  const size_t               (*get_palettes_qty)(void);
  struct Vector              *(*kfc_utils_select_palettes)(void);
  size_t                     (*get_random_palette_index)(void);
};
KFC_UTILS_COMMON_COMMON_FUNCTIONS(kfc_utils_palettes)
exports(kfc_utils_palettes) {
  KFC_UTILS_COMMON_COMMON_EXPORTS(kfc_utils_palettes)
    .get_random_palette_index = kfc_utils_get_random_palette_index,
  .palettes_qty               = 0,
  .palettes_limit_qty         = 999,
  .palettes_data_bytes        = 0,
  .palettes_v                 = NULL,
  .palette_names_v            = NULL,
  .get_palettes_qty           = get_palettes_qty,
  .get_palettes_v             = get_palettes_v,
};
