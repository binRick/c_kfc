#pragma once
////////////////////////////////////
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
////////////////////////////////////

enum kfc_utils_mode {
  LOGGER_NONE,
  LOGGER_INFO,
  LOGGER_ERROR,
  LOGGER_DEBUG,
};

// Module Type Interface
module(kfc_utils) {
  define(kfc_utils, CLIB_MODULE);
  //////////////////////////////////////////////////////////////////////////////////////
  //     Config
  enum kfc_utils_mode mode;
  //////////////////////////////////////////////////////////////////////////////////////
  //     Properties
  size_t               palettes_qty, palettes_data_bytes;
  struct Vector        *palettes_v, *palette_names_v;
  struct inc_palette_t **palettes;
  //////////////////////////////////////////////////////////////////////////////////////
  //     Utility Functions
  //////////////////////////////////////////////////////////////////////////////////////
  struct inc_palette_t *(*get_palette_t_by_index)(const size_t PALETTE_INDEX);
  struct inc_palette_t *(*get_palette_t_by_name)(char *PALETTE_NAME);
  char                 *(*get_palettes_table)();
  char                 *(*get_palette_properties_table)(const char *PALETTE_NAME);
  char                 *(*get_palette_name_by_index)(const size_t PALETTE_INDEX);
  size_t               (*random_palette_index)();
  //////////////////////////////////////////////////////////////////////////////////////
};

int  kfc_utils_init(module(kfc_utils) * exports);
void kfc_utils_deinit(module(kfc_utils) * exports);

// Default Module Exports
exports(kfc_utils) {
  .palettes_qty                 = 0,
  .palettes_data_bytes          = 0,
  .palettes                     = NULL,
  .palettes_v                   = NULL,
  .palette_names_v              = NULL,
  .mode                         = LOGGER_NONE,
  .init                         = kfc_utils_init,
  .deinit                       = kfc_utils_deinit,
  .get_palettes_table           = get_palettes_table,
  .random_palette_index         = random_palette_index,
  .get_palette_properties_table = get_palette_properties_table,
  .get_palette_t_by_name        = get_palette_t_by_name,
  .get_palette_t_by_index       = get_palette_t_by_index,
  .get_palette_name_by_index    = get_palette_name_by_index,
};
