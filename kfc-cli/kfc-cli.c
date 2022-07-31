#define DEBUG_MODE         true
#define KFC_CLI_VERSION    "0.0.1"
#ifndef DEBUG_MEMORY
#define DEBUG_MEMORY
#endif
////////////////////////////////////////////
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
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
#include "kfc-cli/kfc-cli-exports.h"
#include "kfc-cli/kfc-cli-options.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
////////////////////////////////////////////
#include "bytes/bytes.h"
#include "c_stringfn/include/stringfn.h"
#include "cargs/include/cargs.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "log.h/log.h"
#include "process/process.h"
#include "rgba/src/rgba.h"
#include "tempdir.c/tempdir.h"
////////////////////////////////////////////
module(kfc_utils_cache) * kfc_utils_cache;
module(kfc_utils_maps) * kfc_utils_maps;
module(kfc_utils_table) * kfc_utils_table;
module(kfc_utils_terminal) * kfc_utils_terminal;
module(kfc_utils_cli) * kfc_utils_cli;
module(kfc_utils_kitty) * kfc_utils_kitty;
module(kfc_utils_palette_property) * kfc_utils_palette_property;
module(kfc_utils_palette) * kfc_utils_palette;
module(kfc_utils_palettes) * kfc_utils_palettes;
module(kfc_utils_modes) * kfc_utils_modes;


void deinit_modules(void){
  clib_module_free(kfc_utils_maps);
  clib_module_free(kfc_utils_modes);
  clib_module_free(kfc_utils_cache);
  clib_module_free(kfc_utils_table);
  clib_module_free(kfc_utils_terminal);
  clib_module_free(kfc_utils_cli);
  clib_module_free(kfc_utils_kitty);
  clib_module_free(kfc_utils_palette_property);
  clib_module_free(kfc_utils_palette);
  clib_module_free(kfc_utils_palettes);
}


void init_modules(void){
  kfc_utils_maps             = require(kfc_utils_maps);
  kfc_utils_palettes         = require(kfc_utils_palettes);
  kfc_utils_modes            = require(kfc_utils_modes);
  kfc_utils_cache            = require(kfc_utils_cache);
  kfc_utils_table            = require(kfc_utils_table);
  kfc_utils_terminal         = require(kfc_utils_terminal);
  kfc_utils_cli              = require(kfc_utils_cli);
  kfc_utils_kitty            = require(kfc_utils_kitty);
  kfc_utils_palette_property = require(kfc_utils_palette_property);
  kfc_utils_palette          = require(kfc_utils_palette);
}


////////////////////////////////////////////
int kfc_cli_list_palettes(void){
  log_debug("Listing embedded");
  log_debug("%lu palettes", kfc_utils_palettes->get_palettes_qty());
  log_debug("%lu palettes", vector_size(kfc_utils_palettes->get_palettes_v()));
  log_debug("%lu palettes", vector_size(kfc_utils_palettes->palettes_v));
  struct inc_palette_t *p;
  for (size_t i = 0; i < vector_size(kfc_utils_palettes->palettes_v); i++) {
    p = vector_get(kfc_utils_palettes->palettes_v, i);
    if (p->name && strlen(p->name) > 0) {
      printf("%s\n", p->name);
    }
  }
}
void __attribute__((constructor)) __kfc_cli_constructor(){
  init_modules();
  log_debug("palettes vector qty:%lu", kfc_utils_palettes->get_palettes_qty());
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  deinit_modules();
#ifdef DEBUG_MEMORY
  log_debug("<%d> [%s] Checking for memory leaks", getpid(), __FUNCTION__);
  print_allocated_memory();
  log_debug("<%d> [%s] OK", getpid(), __FUNCTION__);
#endif
}


int kfc_cli_print_invalid_palette_properties(void){
  struct Vector *v = get_invalid_palette_property_names();

  for (size_t i = 0; i < vector_size(v); i++) {
    char *n = vector_get(v, i);
    if (n) {
      printf("%s\n", n);
      free(n);
    }
  }
  fprintf(stderr, AC_RESETALL AC_YELLOW "%lu invalid palette properties" AC_RESETALL "\n", vector_size(v));
  vector_release(v);
  return(EXIT_SUCCESS);
}


int kfc_cli_print_unique_palette_properties(void){
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


int parse_args(int argc, char *argv[]){
  kfc_utils_cli->prepare_args(&kfc_utils_cli->args_context, kfc_utils_cli->arg_options, CAG_ARRAY_SIZE(kfc_utils_cli->arg_options), argc, argv);
  while (kfc_utils_cli->get_args_option(&kfc_utils_cli->args_context)) {
    char identifier = kfc_utils_cli->get_args_identifier(&kfc_utils_cli->args_context);
    switch (identifier) {
    case 'G': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_PALETTE_HISTORY; break;
    case 'Q': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_RESET_TERMINAL; break;
    case 'e': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_PALETTE_DATA; break;
    case 'E': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE; break;
    case 'R': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_COLOR_REPORT; break;
    case 'C': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_TEST_COLORS; break;
    case 'K': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_TEST_KITTY_SOCKET; break;
    case 'b': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_BRIGHT_BACKGROUNDS; break;
    case 'k': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_DARK_BACKGROUNDS; break;
    case 's': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_SELECT_PALETTE; break;
    case 'S': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_SELECT_PALETTES; break;
    case 'A': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_SELECT_APPLY_PALETTE; break;
    case 'B': kfc_utils_cli->ctx->max_brightness     = atof(cag_option_get_value(&kfc_utils_cli->args_context)); break;
    case 'L': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_LOAD_PALETTE; break;
    case 'P': kfc_utils_cli->ctx->palette_property   = cag_option_get_value(&kfc_utils_cli->args_context); break;
    case 'V': kfc_utils_cli->ctx->palette_value      = cag_option_get_value(&kfc_utils_cli->args_context); break;
    case 'q': kfc_utils_palettes->palettes_limit_qty = atoi(cag_option_get_value(&kfc_utils_cli->args_context));  break;
    case 'l': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_LIST_PALETTES; break;
    case 'U': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES; break;
    case 'D': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_DETECT_TERMINAL_TYPE; break;
    case 'I': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES; break;
    case 'v': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_VERSION; break;
    case 't': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_PALETTES_TABLE; break;
    case 'T': kfc_utils_cli->ctx->mode               = KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE; break;
    case 'd': kfc_utils_cli->ctx->debug_mode         = true; break;
    case 'p':
      kfc_utils_cli->ctx->palette_name = cag_option_get_value(&kfc_utils_cli->args_context);
      break;
    case 'r':
      kfc_utils_cli->ctx->mode                 = KFC_CLI_MODE_LOAD_PALETTE;
      kfc_utils_cli->ctx->random_palette_index = kfc_utils_palettes->get_random_palette_index();
      kfc_utils_cli->ctx->palette_name         = get_palette_name_by_index(kfc_utils_cli->ctx->random_palette_index);
      break;
    case 'h': kfc_utils_cli->ctx->mode = KFC_CLI_MODE_PRINT_USAGE; break;
      break;
    }
  }
  kfc_utils_cli->mode = (kfc_utils_cli->ctx->debug_mode == true) ? KFC_LOG_DEBUG : kfc_utils_cli->mode;
  return(EXIT_SUCCESS);
} /* parse_args */


int kfc_cli_print_usage(void){
  fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: kfc-cli [OPTION]\n" AC_RESETALL);
  cag_option_print(kfc_utils_cli->arg_options, CAG_ARRAY_SIZE(kfc_utils_cli->arg_options), stdout);
  return(EXIT_SUCCESS);
}


int kfc_cli_print_version(void){
  puts("kfc-cli " KFC_CLI_VERSION);
  return(0);
}


int kfc_cli_print_palette_properties_table(void){
  if (kfc_utils_cli->ctx->palette_name == NULL) {
    fprintf(stderr, AC_RESETALL AC_RED "Must Specify Palette Name" AC_RESETALL "\n");
    kfc_cli_print_usage();
    return(1);
  }
  if (kfc_utils_cli->ctx->debug_mode == true) {
    log_debug("Printing Palette %s Properties Table", kfc_utils_cli->ctx->palette_name);
  }
  char *tbl = kfc_utils_table->get_palette_properties_table(kfc_utils_cli->ctx->palette_name);
  printf("%s", tbl);

  return(0);
}


int kfc_cli_print_palette_table(void){
  if (kfc_utils_cli->ctx->debug_mode == true) {
    log_debug("Printing Palette Table");
  }
  return(0);
}


int kfc_cli_print_palettes_table(void){
  if (kfc_utils_cli->ctx->debug_mode == true) {
    log_debug("Printing Palettes Table");
  }
  fprintf(stdout, "%s", kfc_utils_table->get_palettes_table());
  return(0);
}


int kfc_cli_load_palette(void){
  if (kfc_utils_cli->ctx->palette_name == NULL) {
    kfc_cli_print_usage();
    fprintf(stderr, AC_RESETALL AC_RED "Must Specify a Palette to load" AC_RESETALL "\n");
    return(1);
  }
  struct inc_palette_t *P = get_palette_t_by_name(kfc_utils_cli->ctx->palette_name);
  if (P == NULL) {
    fprintf(stderr, AC_RESETALL AC_RED "Palette '%s' not found" AC_RESETALL "\n", kfc_utils_cli->ctx->palette_name);
    return(1);
  }
  if (kfc_utils_cli->ctx->debug_mode == true) {
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


int kfc_cli_detect_terminal_type(void){
  char *terminal_type = kfc_utils_detect_terminal_type();

  printf("%s\n", terminal_type);
  return(EXIT_SUCCESS);
}


int kfc_cli_test_kitty_socket(void){
  fprintf(stdout, "Testing kitty socket\n");
  bool ok = kfc_utils_test_kitty_socket();
  fprintf(stdout, "ok:%d\n", ok);
  return(EXIT_SUCCESS);
}


char *get_cwd(){
  char *buf = malloc(PATH_MAX);

  if (NULL == getcwd(buf, PATH_MAX)) {
    perror("getcwd error");
    return(NULL);
  }
  char *buf1 = malloc(PATH_MAX);

  if (NULL == realpath(buf, buf1)) {
    perror("realpath error");
    return(NULL);
  }
  return(buf1);
}


int main(int argc, char **argv) {
  kfc_utils_cli->parse_args(argc, argv);
  if (kfc_utils_cli->ctx->mode < KFC_CLI_MODES_QTY) {
    log_debug("Loading mode #%d", kfc_utils_cli->ctx->mode);
    return(kfc_utils_modes->modes[kfc_utils_cli->ctx->mode].handler());
  }
  log_error("Unknown mode #%d", kfc_utils_cli->ctx->mode);
  return(1);
} /* main */


int kfc_cli_test_colors(void){
  log_debug("max_brightness: %f", kfc_utils_cli->ctx->max_brightness);
  char *hex;
  char *pname = "vscode";
  pname = "github";
  pname = "vscode";
  struct palette_property_t *p = get_palette_name_properties_v(pname);
  for (size_t i = 0; i < vector_size(p); i++) {
    struct palette_property_t *pp = vector_get(p, i);
    if (strcmp("background", pp->name) == 0) {
      bool is_dark = palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, kfc_utils_cli->ctx->max_brightness);
      printf("palette %s with background %s is dark?: %s",
             pname, pp->translated_value,
             is_dark ? "Yes" : "No"
             );
    }
    FREE_PALETTE_PROPERTIES(pp);
  }

  return(EXIT_SUCCESS);
} /* kfc_cli_test_colors */


int kfc_cli_bright_backgrounds(void){
  struct Vector *v  = get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_BRIGHT, kfc_utils_cli->ctx->max_brightness);
  size_t        qty = vector_size(v);

  for (size_t i = 0; i < vector_size(v); i++) {
    char *pn = vector_get(v, i);
    puts(pn);
    free(pn);
  }
  vector_release(v);
  fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu bright palettes\n" AC_RESETALL, qty);
  return(EXIT_SUCCESS);
}


int kfc_cli_dark_backgrounds(void){
  struct Vector *v  = get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_DARK, kfc_utils_cli->ctx->max_brightness);
  size_t        qty = vector_size(v);

  for (size_t i = 0; i < vector_size(v); i++) {
    char *pn = vector_get(v, i);
    puts(pn);
    free(pn);
  }
  vector_release(v);
  fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu dark palettes\n" AC_RESETALL, qty);
  return(EXIT_SUCCESS);
}


int kfc_cli_select_palette(void){
  char *palette_name = kfc_utils_select_palette();

  if (palette_name != NULL) {
//    fprintf(stdout, "\ec");
    load_palette_name(palette_name);
    fflush(stdout);
    fprintf(stdout, "%s\n", palette_name);
    free(palette_name);
  }else{
    fprintf(stdout, "Selected no Palette\n");
  }
  return(EXIT_SUCCESS);
}


int kfc_cli_select_palettes(void){
  struct Vector *selected_palettes = kfc_utils_select_palettes();

  fprintf(stderr, "Selected %lu options", vector_size(selected_palettes));
  for (size_t i = 0; i < vector_size(selected_palettes); i++) {
    fprintf(stdout, "%s\n", (char *)vector_get(selected_palettes, i));
  }
  return(EXIT_SUCCESS);
}


int kfc_cli_select_apply_palette(void){
  char *palette_name = kfc_utils_select_apply_palette();

  if (palette_name != NULL) {
//    fprintf(stdout, "\ec");
    load_palette_name(palette_name);
    fflush(stdout);
    fprintf(stdout, "%s\n", palette_name);
  }else{
    fprintf(stdout, "Selected no Palette\n");
  }

  return(EXIT_SUCCESS);
}


int kfc_cli_color_report(void){
  kfc_utils_color_report();
  return(EXIT_SUCCESS);
}


int kfc_cli_print_escaped_sequence(void){
  char *s     = get_palette_name_sequence(kfc_utils_cli->ctx->palette_name);
  char *s_esc = strdup_escaped(s);

  fprintf(stdout, "%s\n", s_esc);
  return(EXIT_SUCCESS);
}


int kfc_cli_print_palette_data(void){
  char *pd = get_palette_name_data(kfc_utils_cli->ctx->palette_name);

  if (pd != NULL) {
    fprintf(stdout, "%s\n", pd);
  }
}


int kfc_cli_reset_terminal(void){
  fprintf(stdout, "%s", get_ansi_reset_sequence());
  fflush(stdout);
  return(EXIT_SUCCESS);
}


int kfc_cli_print_palette_history(void){
  fprintf(stdout, "%s\n", stringfn_mut_trim(get_palette_history()));
  return(EXIT_SUCCESS);
}
