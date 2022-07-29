#define DEBUG_MODE         false
#define KFC_CLI_VERSION    "0.0.1"
#ifndef DEBUG_MEMORY
#define DEBUG_MEMORY
#endif
////////////////////////////////////////////
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
////////////////////////////////////////////
#ifdef DEBUG_MEMORY
#include "debug-memory/debug_memory.h"
#endif
////////////////////////////////////////////
#include "cargs/include/cargs.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
////////////////////////////////////////////
#define KFC    ctx.kfc_utils
////////////////////////////////////////////
static struct cag_option options[] = {
  { .identifier  = 'l', .access_letters = "l",
    .access_name = "list-palettes", .value_name = "LIST_PALETTES_MODE", .description = "List Palettes" },
  { .identifier  = 'd', .access_letters = "d",
    .access_name = "debug", .value_name = "DEBUG_MODE", .description = "Debug Mode" },
  { .identifier  = 'p', .access_letters = "p",
    .access_name = "palette", .value_name = "PALETTE_NAME", .description = "Specified Palette" },
  { .identifier  = 'V', .access_letters = "V",
    .access_name = "palette-value", .value_name = "PALETTE_VALUE", .description = "Specified Palette Value" },
  { .identifier  = 'P', .access_letters = "P",
    .access_name = "palette-property", .value_name = "PALETTE_PROPERTY",
    .description = "Specified Palette Property" },
  { .identifier  = 'v', .access_letters = "v",
    .access_name = "version", .value_name = "VERSION_MODE", .description = "Print Version" },
  { .identifier  = 't', .access_letters = "t",
    .access_name = "palettes-table", .value_name = "PALETTES_TABLE", .description = "Print Palettes Table" },
  { .identifier  = 'r', .access_letters = "r",
    .access_name = "random-palette", .value_name = "RANDOM_PALETTE", .description = "Load Random Palette" },
  { .identifier  = 'L', .access_letters = "L",
    .access_name = "limit-palettes-qty", .value_name = "PALETTES_QTY", .description = "Limit Palettes Qty" },
  { .identifier  = 'h', .access_letters = "h",
    .access_name = "help", .description = "Shows the command help" }
};
static struct kfc_mode_handlers_t {
  int (*handler)(void);
} kfc_mode_handlers[KFC_CLI_MODES_QTY] = {
  [KFC_CLI_MODE_LIST_PALETTES]        = { .handler = kfc_cli_list_palettes,        },
  [KFC_CLI_MODE_LOAD_PALETTE]         = { .handler = kfc_cli_load_palette,         },
  [KFC_CLI_MODE_PRINT_PALETTES_TABLE] = { .handler = kfc_cli_print_palettes_table, },
  [KFC_CLI_MODE_PRINT_PALETTE_TABLE]  = { .handler = kfc_cli_print_palette_table,  },
  [KFC_CLI_MODE_PRINT_VERSION]        = { .handler = kfc_cli_print_version,        },
};
static struct ctx_t {
  char            *palette_name, *random_palette_name, *palette_property, *palette_value;
  size_t          random_palette_index;
  bool            debug_mode;
  enum kfc_mode_t mode;
  module(kfc_utils) * kfc_utils;
} ctx = {
  .palette_name         = NULL,
  .debug_mode           = false,
  .random_palette_index = -1,
  .palette_property     = NULL,
  .palette_value        = NULL,
  .kfc_utils            = NULL,
  .mode                 = -1,
};

void __attribute__((constructor)) __kfc_cli_constructor(){
  KFC       = require(kfc_utils);
  KFC->mode = ctx.debug_mode ? KFC_LOG_DEBUG : KFC_LOG_ERROR;
  printf("palettes vector qty:%lu\n", KFC->get_palettes_qty());
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
#ifdef DEBUG_MEMORY
  fprintf(stderr, "<%d> [%s] Checking for memory leaks\n", getpid(), __FUNCTION__);
  print_allocated_memory();
  fprintf(stderr, "<%d> [%s] OK\n", getpid(), __FUNCTION__);
#endif
}


static int parse_args(int argc, char *argv[]){
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    char identifier = cag_option_get(&context);
    switch (identifier) {
    case 'p': ctx.mode = KFC_CLI_MODE_LOAD_PALETTE;
      ctx.palette_name = cag_option_get_value(&context);
      break;
    case 'P': ctx.palette_property    = cag_option_get_value(&context); break;
    case 'V': ctx.palette_value       = cag_option_get_value(&context); break;
    case 'L': KFC->palettes_limit_qty = atoi(cag_option_get_value(&context));  break;
    case 'l': ctx.mode                = KFC_CLI_MODE_LIST_PALETTES; break;
    case 'v': ctx.mode                = KFC_CLI_MODE_PRINT_VERSION; break;
    case 'r': ctx.mode                = KFC_CLI_MODE_LOAD_PALETTE;
      ctx.random_palette_index        = random_palette_index();
      ctx.palette_name                = get_palette_name_by_index(ctx.random_palette_index);
      break;
    case 't': ctx.mode       = KFC_CLI_MODE_PRINT_PALETTES_TABLE; break;
    case 'd': ctx.debug_mode = true; break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: kfc-cli [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  KFC->mode = ctx.debug_mode ? KFC_LOG_DEBUG : KFC->mode;
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_version(void){
  puts("kfc-cli " KFC_CLI_VERSION);
  return(0);
}


static int kfc_cli_print_palette_table(void){
  if (ctx.debug_mode) {
    fprintf(stderr, "Printing Palette Table\n");
  }
  return(0);
}


static int kfc_cli_print_palettes_table(void){
  if (ctx.debug_mode) {
    fprintf(stderr, "Printing Palettes Table\n");
  }
  printf("palettes table:\n%s", KFC->get_palettes_table());
  return(0);
}


static int kfc_cli_list_palettes(void){
  if (ctx.debug_mode) {
    fprintf(stderr, "Listing embedded\n");
  }
  struct inc_palette_t *p;
  for (size_t i = 0; i < vector_size(KFC->palettes_v); i++) {
    p = vector_get(KFC->palettes_v, i);
    printf("%s\n", p->name);
  }
}


static int kfc_cli_load_palette(void){
  if (ctx.palette_name != NULL) {
    struct inc_palette_t *P = get_palette_t_by_name(ctx.palette_name);
    if (ctx.debug_mode) {
      fprintf(stderr, "Loading embedded palette %s of %db\n", P->name, P->size);
    }
    if (P == NULL) {
      printf("Embedded palette %s not found\n", ctx.palette_name);
    }else{
      load_palette_name(P->name);
      if (ctx.debug_mode) {
        fprintf(stderr, "%s", PALETTE);
      }
    }
  }
  return(0);
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (ctx.mode < KFC_CLI_MODES_QTY) {
    return(kfc_mode_handlers[ctx.mode].handler());
  }
  printf("Unknown mode #%d\n", ctx.mode);
  return(1);
} /* main */


#undef KFC
