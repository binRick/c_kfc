#pragma once
#include <stdio.h>
////////////////////////////////////////////////////////
#include "kfc-cli/kfc-cli.h"
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
static int kfc_cli_load_palette(void);
static int kfc_cli_list_palettes(void);
static int kfc_cli_print_usage(void);
static int kfc_cli_print_palettes_table(void);
static int kfc_cli_print_palette_table(void);
static int kfc_cli_print_version(void);

////////////////////////////////////////////
enum kfc_mode_t {
  KFC_CLI_MODE_USAGE,
  KFC_CLI_MODE_LIST_PALETTES,
  KFC_CLI_MODE_LOAD_PALETTE,
  KFC_CLI_MODE_PRINT_PALETTES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_TABLE,
  KFC_CLI_MODE_PRINT_VERSION,
  KFC_CLI_MODES_QTY,
};
////////////////////////////////////////////////////////
