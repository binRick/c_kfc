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
////////////////////////////////////////////
enum kfc_mode_t {
  KFC_CLI_MODE_PRINT_USAGE,
  KFC_CLI_MODE_LIST_PALETTES,
  KFC_CLI_MODE_LOAD_PALETTE,
  KFC_CLI_MODE_PRINT_PALETTES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE,
  KFC_CLI_MODE_PRINT_PALETTE_TABLE,
  KFC_CLI_MODE_PRINT_VERSION,
  KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES,
  KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES,
  KFC_CLI_MODE_DETECT_TERMINAL_TYPE,
  KFC_CLI_MODE_TEST_KITTY_SOCKET,
  KFC_CLI_MODES_QTY,
};
////////////////////////////////////////////////////////
