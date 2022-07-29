#pragma once
//////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////
#include "bench/bench.h"
#include "c_vector/include/vector.h"
#include "kfc-utils/kfc-utils-module.h"


//////////////////////////////////////
int kfc_utils_init(module(kfc_utils) *exports) {
  clib_module_init(kfc_utils, exports);
  exports->mode = KFC_LOG_DEFAULT;
  if (require(kfc_utils)->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] <module init> started\n", getpid(), __FUNCTION__);
  }
  exports->palettes            = palette_t_list;
  exports->palettes_v          = get_palettes_v();
  exports->palette_names_v     = get_palette_names_v();
  exports->palettes_data_bytes = get_palettes_data_bytes();
  exports->palettes_qty        = vector_size(exports->palettes_v);
  if (require(kfc_utils)->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] <module init> Loaded %lu Palettes\n",
            getpid(), __FUNCTION__,
            exports->get_palettes_qty()
            );
  }
  return(0);
}


//////////////////////////////////////
void kfc_utils_deinit(module(kfc_utils) *exports) {
  if (require(kfc_utils)->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] <module cleanup>\n", getpid(), __FUNCTION__);
  }

  clib_module_deinit(kfc_utils);
}


//////////////////////////////////////
size_t get_palettes_qty(){
  return(vector_size(require(kfc_utils)->palettes_v));
}
//////////////////////////////////////

