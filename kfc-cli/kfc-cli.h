#pragma once
////////////////////////////////////////////////////////
#include "c_ansi/ansi-codes/ansi-codes.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
////////////////////////////////////////////////////////
#define DEFAULT_MAX_BRIGHTNESS         "20.0"
#define DEFAULT_MAX_BRIGHTNESS_VERY    "5.0"
////////////////////////////////////////////////////////
enum kfc_mode_t {
  KFC_CLI_MODE_PRINT_USAGE,
  KFC_CLI_MODE_LIST_PALETTES,
  KFC_CLI_MODE_PRINT_LAST_LOADED_PALETTE_NAME,
  KFC_CLI_MODE_PRINT_PALETTE_BACKGROUND,
  KFC_CLI_MODE_LOAD_PALETTE,
  KFC_CLI_MODE_PRINT_PALETTES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_COLORS_TABLE,
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
  KFC_CLI_MODE_RENDER_UNJA_TEMPLATE,
  KFC_CLI_MODE_RENDER_BRIGHT_COLORS_DEMO,
  KFC_CLI_MODE_VT100UTILS_DEMO,
  KFC_CLI_MODE_PRINT_PALETTE_COLOR_PROPERTY_NAMES,
  KFC_CLI_MODE_PRINT_PALETTE_COLORED_PROPERTY_NAMES,
  KFC_CLI_MODE_BRIGHT_COLORS_VT100UTILS_DEMO,
  KFC_CLI_MODE_PRINT_COLOR_BOXES,
  KFC_CLI_MODES_QTY,
};
struct ctx_t {
  char            *palette_name, *random_palette_name, *palette_property, *palette_value;
  size_t          random_palette_index, palettes_limit_qty;
  bool            debug_mode;
  float           max_brightness;
  enum kfc_mode_t mode;
  struct Vector   *modes;
  module(kfc_utils) * kfc_utils;
} ctx_t;
////////////////////////////////////////////////////////
