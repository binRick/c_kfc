#pragma once
//////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////
#include "bench/bench.h"
#include "c_vector/vector/vector.h"
#include "kfc-utils/kfc-utils-module.h"
#define KFC_MODULE_DEBUG_MODE    false

//////////////////////////////////////
int kfc_utils_module_init(module(kfc_utils) *exports) {
  clib_module_init(kfc_utils, exports);
  exports->mode = ((KFC_MODULE_DEBUG_MODE == true) || getenv("DEBUG_MODE") != NULL) ? KFC_LOG_DEBUG : KFC_LOG_DEFAULT;
  if (exports->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] <module init> started\n", getpid(), __FUNCTION__);
  }
  exports->palettes            = palette_t_list;
  exports->palettes_v          = kfc_utils_get_palettes_v();
  exports->palette_names_v     = kfc_utils_get_palette_names_v();
  exports->palettes_data_bytes = kfc_utils_get_palettes_data_bytes();
  exports->palettes_qty        = vector_size(exports->palettes_v);
  if (exports->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] <module init> Loaded %lu Palettes\n",
            getpid(), __FUNCTION__,
            exports->get_palettes_qty()
            );
  }
  return(0);
}

//////////////////////////////////////
void kfc_utils_module_deinit(module(kfc_utils) *exports) {
  if (KFC_MODULE_DEBUG_MODE == true) {
    fprintf(stderr, "<%d> [%s] [mode:%d] <module cleanup>\n",
            getpid(),
            __FUNCTION__,
            require(kfc_utils)->mode
            );
  }

  clib_module_deinit(kfc_utils);
}

//////////////////////////////////////
size_t kfc_utils_get_palettes_qty(){
  return(vector_size(require(kfc_utils)->palettes_v));
}
//////////////////////////////////////
