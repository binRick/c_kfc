#pragma once
#define KFC_LOG_DEFAULT    KFC_LOG_INFO
////////////////////////////////////
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
////////////////////////////////////
size_t kfc_utils_get_palettes_qty();

enum kfc_utils_mode {
  KFC_LOG_NONE,
  KFC_LOG_INFO,
  KFC_LOG_ERROR,
  KFC_LOG_DEBUG,
  KFC_LOG_QTY,
};

// Module Type Interface
module(kfc_utils) {
  define(kfc_utils, CLIB_MODULE);
  //////////////////////////////////////////////////////////////////////////////////////
  //     Config
  //////////////////////////////////////////////////////////////////////////////////////
  enum kfc_utils_mode mode;
  //////////////////////////////////////////////////////////////////////////////////////
  //     Properties
  //////////////////////////////////////////////////////////////////////////////////////
  size_t               palettes_qty, palettes_data_bytes, palettes_limit_qty;
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
  size_t               (*get_palettes_qty)();
  struct Vector        (*get_palettes_v)();
  //////////////////////////////////////////////////////////////////////////////////////
};

int  kfc_utils_module_init(module(kfc_utils) * exports);
void kfc_utils_module_deinit(module(kfc_utils) * exports);

// Default Module Exports
exports(kfc_utils) {
  .palettes_qty                 = 0,
  .palettes_limit_qty           = 999,
  .palettes_data_bytes          = 0,
  .palettes_v                   = NULL,
  .palette_names_v              = NULL,
  .mode                         = KFC_LOG_DEFAULT,
  .init                         = kfc_utils_module_init,
  .deinit                       = kfc_utils_module_deinit,
  .get_palettes_table           = kfc_utils_get_palettes_table,
  .random_palette_index         = kfc_utils_random_palette_index,
  .get_palette_properties_table = kfc_utils_get_palette_properties_table,
  .get_palette_t_by_name        = kfc_utils_get_palette_t_by_name,
  .get_palette_t_by_index       = kfc_utils_get_palette_t_by_index,
  .get_palette_name_by_index    = kfc_utils_get_palette_name_by_index,
  .get_palettes_qty             = kfc_utils_get_palettes_qty,
  .get_palettes_v               = kfc_utils_get_palettes_v,
};
