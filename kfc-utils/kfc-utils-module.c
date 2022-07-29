#pragma once
#include <stdio.h>
//////////////////////////////////////
#include "c_vector/include/vector.h"
#include "kfc-utils/kfc-utils-module.h"
//////////////////////////////////////
static inline void kfc_utils_info(char *message);
static inline void kfc_utils_error(char *message);
static inline void kfc_utils_info(char *message);
static inline void kfc_utils_debug(char *message);


//////////////////////////////////////
int kfc_utils_init(module(kfc_utils) *exports) {
  fprintf(stderr, "[%d]module init>\n", getpid());
  clib_module_init(kfc_utils, exports);
  exports->palettes            = palette_t_list;
  exports->palettes_v          = get_palettes_v();
  exports->palette_names_v     = get_palette_names_v();
  exports->palettes_data_bytes = get_palettes_data_bytes();
  exports->palettes_qty        = vector_size(exports->palettes_v);
  exports->mode                = LOGGER_NONE;
  return(0);
}


void kfc_utils_deinit(module(kfc_utils) *exports) {
  fprintf(stderr, "[%d]module cleanup>\n", getpid());
  clib_module_deinit(kfc_utils);
}

