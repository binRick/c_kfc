#pragma once
#include <stdbool.h>
#include <stdio.h>
////////////////////////////////////////////////////////
enum kfc_mode_t {
  KFC_CLI_MODE_PRINT_USAGE,
  KFC_CLI_MODE_LIST_PALETTES,
  KFC_CLI_MODE_LOAD_PALETTE,
  KFC_CLI_MODE_PRINT_PALETTES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE,
  KFC_CLI_MODE_PRINT_VERSION,
  KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES,
  KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES,
  KFC_CLI_MODE_DETECT_TERMINAL_TYPE,
  KFC_CLI_MODE_TEST_KITTY_SOCKET,
  KFC_CLI_MODE_TEST_COLORS,
  KFC_CLI_MODE_BRIGHT_BACKGROUNDS,
  KFC_CLI_MODE_DARK_BACKGROUNDS,
  KFC_CLI_MODE_SELECT_PALETTE,
  KFC_CLI_MODE_SELECT_PALETTES,
  KFC_CLI_MODE_SELECT_APPLY_PALETTE,
  KFC_CLI_MODE_COLOR_REPORT,
  KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE,
  KFC_CLI_MODE_PRINT_PALETTE_DATA,
  KFC_CLI_MODE_RESET_TERMINAL,
  KFC_CLI_MODE_PRINT_PALETTE_HISTORY,
  KFC_CLI_MODES_QTY,
};
struct ctx_t {
  char            *palette_name, *random_palette_name, *palette_property, *palette_value;
  size_t          random_palette_index;
  bool            debug_mode;
  float           max_brightness;
  enum kfc_mode_t mode;
} ctx_t;
struct kfc_mode_handlers_t {
  int (*handler)(void);
};
struct kfc_utils_cache_files_t {
  char *name;
  char *path;
};
static const double DEFAULT_MAX_BRIGHTNESS = 40.0;
int parse_args(int argc, char *argv[]);
int kfc_cli_list_palettes(void);
int kfc_cli_load_palette(void);
int kfc_cli_print_usage(void);
int kfc_cli_print_palettes_table(void);
int kfc_cli_print_palette_table(void);
int kfc_cli_print_version(void);
int kfc_cli_print_palette_properties_table(void);
int kfc_cli_print_invalid_palette_properties(void);
int kfc_cli_print_unique_palette_properties(void);
int kfc_cli_detect_terminal_type(void);
int kfc_cli_test_kitty_socket(void);
int kfc_cli_test_colors(void);
int kfc_cli_bright_backgrounds(void);
int kfc_cli_dark_backgrounds(void);
int kfc_cli_select_palettes(void);
int kfc_cli_select_apply_palette(void);
int kfc_cli_select_palette(void);
int kfc_cli_color_report(void);
int kfc_cli_print_escaped_sequence(void);
int kfc_cli_print_palette_data(void);
int kfc_cli_reset_terminal(void);
int kfc_cli_print_palette_history(void);

////////////////////////////////////////////////////////
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "submodules/meson_deps/submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/meson_deps/submodules/c_fsio/include/fsio.h"
#include "submodules/meson_deps/submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/meson_deps/submodules/c_stringfn/include/stringfn.h"
#include "submodules/meson_deps/submodules/c_stringfn/include/stringfn.h"
#include "submodules/meson_deps/submodules/c_timer/include/c_timer.h"
#include "submodules/meson_deps/submodules/c_vector/include/vector.h"
#include "submodules/meson_deps/submodules/tiny-regex-c/re.h"
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
