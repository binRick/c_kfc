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
#include "bytes/bytes.h"
#include "cargs/include/cargs.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
////////////////////////////////////////////
#define KFC    ctx.kfc_utils
////////////////////////////////////////////
static int kfc_cli_load_palette(void);
static int kfc_cli_list_palettes(void);
static int kfc_cli_print_usage(void);
static int kfc_cli_print_palettes_table(void);
static int kfc_cli_print_palette_table(void);
static int kfc_cli_print_version(void);
static int kfc_cli_print_palette_properties_table(void);
static int kfc_cli_print_invalid_palette_properties(void);
static int kfc_cli_print_unique_palette_properties(void);
static int kfc_cli_detect_terminal_type(void);
static int kfc_cli_test_kitty_socket(void);

////////////////////////////////////////////
static struct cag_option options[] = {
  { .identifier  = 'K', .access_letters = "K",
    .access_name = "test-kitty-socket", .value_name = NULL, .description = "Test Kitty Socket" },
  { .identifier  = 'D', .access_letters = "D",
    .access_name = "detect-terminal", .value_name = NULL, .description = "Detect Terminal Type" },
  { .identifier  = 'U', .access_letters = "U",
    .access_name = "unique-items", .value_name = NULL, .description = "List Unique Palette Items" },
  { .identifier  = 'I', .access_letters = "I",
    .access_name = "invalid-items", .value_name = NULL, .description = "List Invalid Palette Items" },
  { .identifier  = 'L', .access_letters = "L",
    .access_name = "load-palette", .value_name = NULL, .description = "Load Palette" },
  { .identifier  = 'l', .access_letters = "l",
    .access_name = "list-palettes", .value_name = NULL, .description = "List Palettes" },
  { .identifier  = 'd', .access_letters = "d",
    .access_name = "debug", .value_name = NULL, .description = "Debug Mode" },
  { .identifier  = 'p', .access_letters = "p",
    .access_name = "palette", .value_name = "PALETTE_NAME", .description = "Specified Palette" },
  { .identifier  = 'V', .access_letters = "V",
    .access_name = "palette-value", .value_name = "PALETTE_VALUE", .description = "Specified Palette Value" },
  { .identifier  = 'P', .access_letters = "P",
    .access_name = "palette-property", .value_name = "PALETTE_PROPERTY",
    .description = "Specified Palette Property" },
  { .identifier  = 'v', .access_letters = "v",
    .access_name = "version", .value_name = NULL, .description = "Print Version" },
  { .identifier  = 'T', .access_letters = "T",
    .access_name = "palette-properties-table", .value_name = NULL, .description = "Print Palette Properties Table" },
  { .identifier  = 't', .access_letters = "t",
    .access_name = "palettes-table", .value_name = NULL, .description = "Print Palettes Table" },
  { .identifier  = 'r', .access_letters = "r",
    .access_name = "random-palette", .value_name = NULL, .description = "Load Random Palette" },
  { .identifier  = 'q', .access_letters = "q",
    .access_name = "limit-palettes-qty", .value_name = "PALETTES_QTY", .description = "Limit Palettes Qty" },
  { .identifier  = 'h', .access_letters = "h",
    .access_name = "help", .description = "Shows the command help" }
};
static struct kfc_mode_handlers_t {
  int (*handler)(void);
} kfc_mode_handlers[KFC_CLI_MODES_QTY] = {
  [KFC_CLI_MODE_LIST_PALETTES]                    = { .handler = kfc_cli_list_palettes,                    },
  [KFC_CLI_MODE_LOAD_PALETTE]                     = { .handler = kfc_cli_load_palette,                     },
  [KFC_CLI_MODE_PRINT_PALETTES_TABLE]             = { .handler = kfc_cli_print_palettes_table,             },
  [KFC_CLI_MODE_PRINT_PALETTE_TABLE]              = { .handler = kfc_cli_print_palette_table,              },
  [KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE]   = { .handler = kfc_cli_print_palette_properties_table,   },
  [KFC_CLI_MODE_PRINT_VERSION]                    = { .handler = kfc_cli_print_version,                    },
  [KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES]  = { .handler = kfc_cli_print_unique_palette_properties,  },
  [KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES] = { .handler = kfc_cli_print_invalid_palette_properties, },
  [KFC_CLI_MODE_DETECT_TERMINAL_TYPE]             = { .handler = kfc_cli_detect_terminal_type,             },
  [KFC_CLI_MODE_TEST_KITTY_SOCKET]                = { .handler = kfc_cli_test_kitty_socket,                },
  [KFC_CLI_MODE_PRINT_USAGE]                      = { .handler = kfc_cli_print_usage,                      },
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
  .random_palette_index = 1,
  .palette_property     = NULL,
  .palette_value        = NULL,
  .kfc_utils            = NULL,
  .mode                 = KFC_CLI_MODE_LOAD_PALETTE,
};

void __attribute__((constructor)) __kfc_cli_constructor(){
  KFC       = require(kfc_utils);
  KFC->mode = (ctx.debug_mode == true) ? KFC_LOG_DEBUG : KFC_LOG_ERROR;
  if (ctx.debug_mode == true) {
    printf("palettes vector qty:%lu\n", KFC->get_palettes_qty());
  }
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
#ifdef DEBUG_MEMORY
  if (ctx.debug_mode == true) {
    fprintf(stderr, "<%d> [%s] Checking for memory leaks\n", getpid(), __FUNCTION__);
  }
  print_allocated_memory();
  if (ctx.debug_mode == true) {
    fprintf(stderr, "<%d> [%s] OK\n", getpid(), __FUNCTION__);
  }
#endif
}


static int kfc_cli_print_invalid_palette_properties(void){
  struct Vector *v = get_invalid_palette_property_names();

  for (size_t i = 0; i < vector_size(v); i++) {
    char *n = vector_get(v, i);
    if (n) {
      printf("%s\n", n);
      free(n);
    }
  }
  printf(AC_RESETALL AC_YELLOW "%lu invalid palette properties" AC_RESETALL "\n", vector_size(v));
  vector_release(v);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_unique_palette_properties(void){
  struct Vector *unique_palette_properties_v = get_unique_palette_property_names();

  for (size_t i = 0; i < vector_size(unique_palette_properties_v); i++) {
    char *pp = vector_get(unique_palette_properties_v, i);
    if (pp) {
      printf("%s\n", pp);
      free(pp);
    }
  }
  printf(AC_RESETALL AC_YELLOW "%lu unique palette properties" AC_RESETALL "\n", vector_size(unique_palette_properties_v));
  vector_release(unique_palette_properties_v);
  return(EXIT_SUCCESS);
}


static int parse_args(int argc, char *argv[]){
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    char identifier = cag_option_get(&context);
    switch (identifier) {
    case 'K': ctx.mode                = KFC_CLI_MODE_TEST_KITTY_SOCKET; break;
    case 'L': ctx.mode                = KFC_CLI_MODE_LOAD_PALETTE; break;
    case 'P': ctx.palette_property    = cag_option_get_value(&context); break;
    case 'V': ctx.palette_value       = cag_option_get_value(&context); break;
    case 'q': KFC->palettes_limit_qty = atoi(cag_option_get_value(&context));  break;
    case 'l': ctx.mode                = KFC_CLI_MODE_LIST_PALETTES; break;
    case 'U': ctx.mode                = KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES; break;
    case 'D': ctx.mode                = KFC_CLI_MODE_DETECT_TERMINAL_TYPE; break;
    case 'I': ctx.mode                = KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES; break;
    case 'v': ctx.mode                = KFC_CLI_MODE_PRINT_VERSION; break;
    case 't': ctx.mode                = KFC_CLI_MODE_PRINT_PALETTES_TABLE; break;
    case 'T': ctx.mode                = KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE; break;
    case 'd': ctx.debug_mode          = true; break;
    case 'p':
      ctx.palette_name = cag_option_get_value(&context);
      break;
    case 'r':
      ctx.random_palette_index = random_palette_index();
      ctx.palette_name         = get_palette_name_by_index(ctx.random_palette_index);
      break;
    case 'h': ctx.mode = KFC_CLI_MODE_PRINT_USAGE; break;
      break;
    }
  }
  KFC->mode = (ctx.debug_mode == true) ? KFC_LOG_DEBUG : KFC->mode;
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_usage(void){
  fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: kfc-cli [OPTION]\n" AC_RESETALL);
  cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_version(void){
  puts("kfc-cli " KFC_CLI_VERSION);
  return(0);
}


static int kfc_cli_print_palette_properties_table(void){
  if (ctx.palette_name == NULL) {
    fprintf(stdout, "Must Specify Palette Name\n");
    kfc_cli_print_usage();
    return(1);
  }
  if (ctx.debug_mode == true) {
    fprintf(stdout, "Printing Palette %s Properties Table\n", ctx.palette_name);
  }
  char *tbl = KFC->get_palette_properties_table(ctx.palette_name);
  printf("%s", tbl);

  return(0);
}


static int kfc_cli_print_palette_table(void){
  if (ctx.debug_mode == true) {
    fprintf(stderr, "Printing Palette Table\n");
  }
  return(0);
}


static int kfc_cli_print_palettes_table(void){
  if (ctx.debug_mode == true) {
    fprintf(stderr, "Printing Palettes Table\n");
  }
  printf("palettes table:\n%s", KFC->get_palettes_table());
  return(0);
}


static int kfc_cli_list_palettes(void){
  if (ctx.debug_mode == true) {
    fprintf(stderr, "Listing embedded\n");
  }
  struct inc_palette_t *p;
  for (size_t i = 0; i < vector_size(KFC->palettes_v); i++) {
    p = vector_get(KFC->palettes_v, i);
    printf("%s\n", p->name);
  }
}


static int kfc_cli_load_palette(void){
  if (ctx.palette_name == NULL) {
    kfc_cli_print_usage();
    fprintf(stderr, AC_RESETALL AC_RED "Must Specify a Palette to load" AC_RESETALL "\n");
    return(1);
  }
  struct inc_palette_t *P = get_palette_t_by_name(ctx.palette_name);
  if (P == NULL) {
    fprintf(stderr, AC_RESETALL AC_RED "Palette '%s' not found" AC_RESETALL "\n", ctx.palette_name);
    return(1);
  }
  if (ctx.debug_mode == true) {
    fprintf(stderr, AC_RESETALL AC_GREEN "Loading embedded palette %s of %s" AC_RESETALL "\n", P->name, bytes_to_string(P->size));
  }
  size_t written_bytes = load_palette_name(P->name);
  if (written_bytes < 1) {
    fprintf(stderr, "Failed to write %s palette sequence\n", P->name);
    return(1);
  }
  fprintf(stderr, "%s\n", P->name);
  return(0);
}


static int kfc_cli_detect_terminal_type(void){
  char *terminal_type = kfc_utils_detect_terminal_type();

  printf("Detected term '"AC_RESETALL AC_YELLOW AC_REVERSED "%s" AC_RESETALL "'\n", terminal_type);
  return(EXIT_SUCCESS);
}


static int kfc_cli_test_kitty_socket(void){
  printf("Testing kitty socket\n");
  bool ok = kfc_utils_test_kitty_socket();
  printf("ok:%d\n", ok);
  return(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (ctx.mode < KFC_CLI_MODES_QTY) {
    if (ctx.debug_mode == true) {
      printf("Loading mode #%d\n", ctx.mode);
    }
    return(kfc_mode_handlers[ctx.mode].handler());
  }
  printf("Unknown mode #%d\n", ctx.mode);
  return(1);
} /* main */

#undef KFC
