#pragma once
//////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////
#include "bench/bench.h"
#include "c_vector/include/vector.h"
#include "kfc-utils/kfc-utils-module.h"
#include "log.h/log.h"
#include "tempdir.c/tempdir.h"
#ifndef __PALETTES_HASH__
#define PALETTES_HASH    ""
#else
#define PALETTES_HASH    __PALETTES_HASH__
#endif

#define DEBUG_MODULE_DEINIT(NAME)                    \
  log_debug("<%d> [%s] [mode:%d] <module deinit>\n", \
            getpid(),                                \
            __FUNCTION__,                            \
            require(NAME)->mode                      \
            );

#define DEBUG_MODULE_INIT(NAME)                    \
  log_debug("<%d> [%s] [mode:%d] <module init>\n", \
            getpid(),                              \
            __FUNCTION__,                          \
            require(NAME)->mode                    \
            );


module(kfc_utils_palettes) * kfc_utils_palettes;
module(kfc_utils_cache) * kfc_utils_cache;
module(kfc_utils_table) * kfc_utils_table;
module(kfc_utils_maps) * kfc_utils_maps;
module(kfc_utils_terminal) * kfc_utils_terminal;
module(kfc_utils_cli) * kfc_utils_cli;
module(kfc_utils_kitty) * kfc_utils_kitty;
module(kfc_utils_palette_property) * kfc_utils_palette_property;
module(kfc_utils_palette) * kfc_utils_palette;
module(kfc_utils_modes) * kfc_utils_modes;
void __attribute__((constructor)) __kfc_modules_constructor(){
}

void __attribute__((destructor)) __kfc_modes_destructor(){
}


//////////////////////////////////////
int kfc_utils_terminal_module_init(module(kfc_utils_terminal) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_terminal)
  clib_module_init(kfc_utils_terminal, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_terminal_module_deinit(module(kfc_utils_terminal) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_terminal)
  clib_module_deinit(kfc_utils_terminal);
}


int kfc_utils_table_module_init(module(kfc_utils_table) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_table)
  clib_module_init(kfc_utils_table, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_table_module_deinit(module(kfc_utils_table) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_table)
  clib_module_deinit(kfc_utils_table);
}


int kfc_utils_maps_module_init(module(kfc_utils_maps) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_maps)
  clib_module_init(kfc_utils_maps, exports);
  djbhash_init(&exports->palette_properties_h);
  djbhash_init(&exports->valid_palette_property_names_h);
  djbhash_init(&exports->invalid_palette_property_names_h);

  return(EXIT_SUCCESS);
}


void kfc_utils_maps_module_deinit(module(kfc_utils_maps) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_maps)

  djbhash_destroy(&exports->invalid_palette_property_names_h);
  djbhash_destroy(&exports->valid_palette_property_names_h);
  djbhash_destroy(&exports->palette_properties_h);
  clib_module_deinit(kfc_utils_maps);
}


int kfc_utils_modes_module_init(module(kfc_utils_modes) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_modes)
  clib_module_init(kfc_utils_modes, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_modes_module_deinit(module(kfc_utils_modes) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_modes)
  clib_module_deinit(kfc_utils_modes);
}


int kfc_utils_kitty_module_init(module(kfc_utils_kitty) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_kitty)
  clib_module_init(kfc_utils_kitty, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_kitty_module_deinit(module(kfc_utils_kitty) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_kitty)
  clib_module_deinit(kfc_utils_kitty);
}


int kfc_utils_cli_module_init(module(kfc_utils_cli) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_cli)
  clib_module_init(kfc_utils_cli, exports);
  exports->ctx = &(struct ctx_t){
    .palette_name         = NULL,
    .debug_mode           = true,
    .random_palette_index = 1,
    .palette_property     = NULL,
    .max_brightness       = DEFAULT_MAX_BRIGHTNESS,
    .palette_value        = NULL,
    .mode                 = KFC_CLI_MODE_LIST_PALETTES,
  };
  return(EXIT_SUCCESS);
}


void kfc_utils_cli_module_deinit(module(kfc_utils_cli) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_cli)
  clib_module_deinit(kfc_utils_cli);
}


int kfc_utils_cache_module_init(module(kfc_utils_cache) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_cache)
  clib_module_init(kfc_utils_cache, exports);
  if (strcmp(PALETTES_HASH, "") != 0) {
    char *ymd = require(kfc_utils_cache)->get_cache_ymd();
    for (size_t i = 0; i < CACHE_ITEMS_QTY; i++) {
      asprintf(&require(kfc_utils_cache)->CACHE_ITEMS[i].path,
               "%s%s-%s-%s.txt",
               gettempdir(),
               require(kfc_utils_cache)->CACHE_ITEMS[i].name,
               ymd,
               PALETTES_HASH
               );
      log_debug("palettes table hash %s|%s", PALETTES_HASH, require(kfc_utils_cache)->CACHE_ITEMS[PALETTES_TABLE].path);
    }
  }
  return(EXIT_SUCCESS);
}


void kfc_utils_cache_module_deinit(module(kfc_utils_cache) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_cache)
  clib_module_deinit(kfc_utils_cache);
}


int kfc_utils_palette_property_module_init(module(kfc_utils_palette_property) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_palette_property)
  clib_module_init(kfc_utils_palette_property, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_palette_property_module_deinit(module(kfc_utils_palette_property) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_palette_property)
  clib_module_deinit(kfc_utils_palette_property);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int kfc_utils_palette_module_init(module(kfc_utils_palette) *exports) {
  kfc_utils_palettes = require(kfc_utils_palettes);
  DEBUG_MODULE_INIT(kfc_utils_palette)
  clib_module_init(kfc_utils_palette, exports);
  return(EXIT_SUCCESS);
}


void kfc_utils_palette_module_deinit(module(kfc_utils_palette) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_palette)
  clib_module_deinit(kfc_utils_palette);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int kfc_utils_palettes_module_init(module(kfc_utils_palettes) *exports) {
  DEBUG_MODULE_INIT(kfc_utils_palettes)
  clib_module_init(kfc_utils_palettes, exports);
  exports->palettes            = palette_t_list;
  exports->palettes_v          = get_palettes_v();
  exports->palette_names_v     = get_palette_names_v();
  exports->palettes_data_bytes = get_palettes_data_bytes();
  exports->palettes_qty        = vector_size(exports->palettes_v);
  log_debug("%lu palettes\n", vector_size(exports->palettes_v));
  return(EXIT_SUCCESS);
}


void kfc_utils_palettes_module_deinit(module(kfc_utils_palettes) *exports) {
  DEBUG_MODULE_DEINIT(kfc_utils_palettes)
  clib_module_deinit(kfc_utils_palettes);
}


//////////////////////////////////////
size_t get_palettes_qty(){
  return(vector_size(require(kfc_utils_palettes)->get_palettes_v()));
}
