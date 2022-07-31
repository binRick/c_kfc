#define DEBUG_MODE         false
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
#define IS_DEBUG_MODE    (KFC->mode >= KFC_LOG_DEBUG || ctx.debug_mode == true)
////////////////////////////////////////////
#include "bytes/bytes.h"
#include "c_stringfn/include/stringfn.h"
#include "cargs/include/cargs.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "log.h/log.h"
#include "process/process.h"
#include "rgba/src/rgba.h"
#include "tempdir.c/tempdir.h"
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
static int kfc_cli_test_colors(void);
static int kfc_cli_bright_backgrounds(void);
static int kfc_cli_dark_backgrounds(void);
static int kfc_cli_select_palettes(void);
static int kfc_cli_select_apply_palette(void);
static int kfc_cli_select_palette(void);
static int kfc_cli_color_report(void);
static int kfc_cli_print_escaped_sequence(void);
static int kfc_cli_print_palette_data(void);
static int kfc_cli_reset_terminal(void);
static int kfc_cli_print_palette_history(void);

const char               *EXECUTABLE_PATH_DIRNAME = NULL, *EXECUTABLE_NAME = NULL, *EXECUTABLE_ABSOLUTE = NULL;
////////////////////////////////////////////
static struct cag_option options[] = {
  { .identifier  = 'G', .access_letters = "G",
    .access_name = "print-palette-history", .value_name = NULL, .description = "Print Palette History" },
  { .identifier  = 'Q', .access_letters = "Q",
    .access_name = "reset-terminal", .value_name = NULL, .description = "Reset Terminal State" },
  { .identifier  = 'e', .access_letters = "e",
    .access_name = "print-palette-data", .value_name = NULL, .description = "Print Palettte Data" },
  { .identifier  = 'E', .access_letters = "E",
    .access_name = "escaped-sequence", .value_name = NULL, .description = "Escaped Sequence" },
  { .identifier  = 'R', .access_letters = "R",
    .access_name = "color-report", .value_name = NULL, .description = "Color Report" },
  { .identifier  = 'A', .access_letters = "A",
    .access_name = "select-apply-palette", .value_name = NULL, .description = "Select & Apply Palettes" },
  { .identifier  = 'S', .access_letters = "S",
    .access_name = "select-palettes", .value_name = NULL, .description = "Select Palettes" },
  { .identifier  = 's', .access_letters = "s",
    .access_name = "select-palette", .value_name = NULL, .description = "Select Palette" },
  { .identifier  = 'k', .access_letters = "k",
    .access_name = "dark-backgrounds", .value_name = NULL, .description = "Dark Backgrounds" },
  { .identifier  = 'b', .access_letters = "b",
    .access_name = "bright-backgrounds", .value_name = NULL, .description = "Bright Backgrounds" },
  { .identifier  = 'B', .access_letters = "B",
    .access_name = "max-brightness", .value_name = "MAX_BRIGHTNESS", .description = "Max Brightness (0.00 - 100)" },
  { .identifier  = 'C', .access_letters = "C",
    .access_name = "test-colors", .value_name = NULL, .description = "Test Colors" },
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
  [KFC_CLI_MODE_TEST_COLORS]                      = { .handler = kfc_cli_test_colors,                      },
  [KFC_CLI_MODE_BRIGHT_BACKGROUNDS]               = { .handler = kfc_cli_bright_backgrounds,               },
  [KFC_CLI_MODE_DARK_BACKGROUNDS]                 = { .handler = kfc_cli_dark_backgrounds,                 },
  [KFC_CLI_MODE_SELECT_PALETTE]                   = { .handler = kfc_cli_select_palette,                   },
  [KFC_CLI_MODE_SELECT_APPLY_PALETTE]             = { .handler = kfc_cli_select_apply_palette,             },
  [KFC_CLI_MODE_SELECT_PALETTES]                  = { .handler = kfc_cli_select_palettes,                  },
  [KFC_CLI_MODE_COLOR_REPORT]                     = { .handler = kfc_cli_color_report,                     },
  [KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE]   = { .handler = kfc_cli_print_escaped_sequence,           },
  [KFC_CLI_MODE_PRINT_PALETTE_DATA]               = { .handler = kfc_cli_print_palette_data,               },
  [KFC_CLI_MODE_RESET_TERMINAL]                   = { .handler = kfc_cli_reset_terminal,                   },
  [KFC_CLI_MODE_PRINT_PALETTE_HISTORY]            = { .handler = kfc_cli_print_palette_history,            },
};
static struct ctx_t {
  char            *palette_name, *random_palette_name, *palette_property, *palette_value;
  size_t          random_palette_index;
  bool            debug_mode;
  float           max_brightness;
  enum kfc_mode_t mode;
  module(kfc_utils) * kfc_utils;
} ctx = {
  .palette_name         = NULL,
  .debug_mode           = false,
  .random_palette_index = 1,
  .palette_property     = NULL,
  .max_brightness       = DEFAULT_MAX_BRIGHTNESS,
  .palette_value        = NULL,
  .kfc_utils            = NULL,
  .mode                 = KFC_CLI_MODE_LOAD_PALETTE,
};
void __attribute__((constructor)) __kfc_cli_constructor(){
  KFC       = require(kfc_utils);
  KFC->mode = (ctx.debug_mode == true || (getenv("DEBUG_MODE") != NULL)) ? KFC_LOG_DEBUG : KFC_LOG_ERROR;
  if (KFC->mode >= KFC_LOG_DEBUG) {
    log_info("palettes vector qty:%lu", KFC->get_palettes_qty());
  }
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
#ifdef DEBUG_MEMORY
  if (IS_DEBUG_MODE) {
    log_info("<%d> [%s] Checking for memory leaks", getpid(), __FUNCTION__);
  }
  print_allocated_memory();
  if (IS_DEBUG_MODE) {
    log_info("<%d> [%s] OK", getpid(), __FUNCTION__);
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
  fprintf(stderr, AC_RESETALL AC_YELLOW "%lu invalid palette properties" AC_RESETALL "\n", vector_size(v));
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
    case 'G': ctx.mode                = KFC_CLI_MODE_PRINT_PALETTE_HISTORY; break;
    case 'Q': ctx.mode                = KFC_CLI_MODE_RESET_TERMINAL; break;
    case 'e': ctx.mode                = KFC_CLI_MODE_PRINT_PALETTE_DATA; break;
    case 'E': ctx.mode                = KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE; break;
    case 'R': ctx.mode                = KFC_CLI_MODE_COLOR_REPORT; break;
    case 'C': ctx.mode                = KFC_CLI_MODE_TEST_COLORS; break;
    case 'K': ctx.mode                = KFC_CLI_MODE_TEST_KITTY_SOCKET; break;
    case 'b': ctx.mode                = KFC_CLI_MODE_BRIGHT_BACKGROUNDS; break;
    case 'k': ctx.mode                = KFC_CLI_MODE_DARK_BACKGROUNDS; break;
    case 's': ctx.mode                = KFC_CLI_MODE_SELECT_PALETTE; break;
    case 'S': ctx.mode                = KFC_CLI_MODE_SELECT_PALETTES; break;
    case 'A': ctx.mode                = KFC_CLI_MODE_SELECT_APPLY_PALETTE; break;
    case 'B': ctx.max_brightness      = atof(cag_option_get_value(&context)); break;
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
} /* parse_args */


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
    fprintf(stderr, AC_RESETALL AC_RED "Must Specify Palette Name" AC_RESETALL "\n");
    kfc_cli_print_usage();
    return(1);
  }
  if (ctx.debug_mode == true) {
    log_debug("Printing Palette %s Properties Table", ctx.palette_name);
  }
  char *tbl = KFC->get_palette_properties_table(ctx.palette_name);
  printf("%s", tbl);

  return(0);
}


static int kfc_cli_print_palette_table(void){
  if (ctx.debug_mode == true) {
    log_debug("Printing Palette Table");
  }
  return(0);
}


static int kfc_cli_print_palettes_table(void){
  if (ctx.debug_mode == true) {
    log_debug("Printing Palettes Table");
  }
  fprintf(stdout, "%s", KFC->get_palettes_table());
  return(0);
}


static int kfc_cli_list_palettes(void){
  if (ctx.debug_mode == true) {
    log_debug("Listing embedded");
  }
  struct inc_palette_t *p;
  for (size_t i = 0; i < vector_size(KFC->palettes_v); i++) {
    p = vector_get(KFC->palettes_v, i);
    if (p->name && strlen(p->name) > 0) {
      printf("%s\n", p->name);
    }
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

  printf("%s\n", terminal_type);
  return(EXIT_SUCCESS);
}


static int kfc_cli_test_kitty_socket(void){
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


char *exec_file(const char *argv0){
  if (fsio_file_exists(argv0)) {
    return(argv0);
  }

  char                   *p = getenv("PATH");
  log_info("exec_path p: %s", p);
  struct StringFNStrings paths = stringfn_split(p, ':');
  for (size_t i = 0; i < paths.count; i++) {
    log_info("%s", paths.strings[i]);
    char *_e;
    asprintf(&_e, "%s/%s", paths.strings[i], argv0);
    if (fsio_file_exists(_e)) {
      log_info("found: %s", _e);
    }
  }

  return(NULL);
}


char *exec_path(const char *argv0){
  char *ef = exec_file(argv0);

  if (ef[0] == '/') {
    return(ef);
  }

  char *ep;
  char *cwd = get_cwd();

  asprintf(&ep, "%s/%s", cwd, ef);

//    if(!fsio_file_exists(buf))
//        return NULL;

  return(ep);
}


char * app_path(char *path, const char *argv0){
  char buf[PATH_MAX];
  char *pos;

  if (argv0[0] == '/') { // run with absolute path
    strcpy(buf, argv0);
  } else {               // run with relative path
    if (NULL == getcwd(buf, PATH_MAX)) {
      perror("getcwd error");
      return(NULL);
    }
    strcat(buf, "/");
    strcat(buf, argv0);
  }
  if (NULL == realpath(buf, path)) {
    perror("realpath error");
    return(NULL);
  }
  return(path);
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (ctx.mode < KFC_CLI_MODES_QTY) {
    if (ctx.debug_mode == true) {
      log_debug("Loading mode #%d", ctx.mode);
    }
    return(kfc_mode_handlers[ctx.mode].handler());
  }
  log_error("Unknown mode #%d", ctx.mode);
  return(1);
} /* main */


static int kfc_cli_test_colors(void){
  log_debug("max_brightness: %f", ctx.max_brightness);
  char *hex;
  char *pname = "vscode";
  pname = "github";
  pname = "vscode";
  struct palette_property_t *p = get_palette_name_properties_v(pname);
  for (size_t i = 0; i < vector_size(p); i++) {
    struct palette_property_t *pp = vector_get(p, i);
    if (strcmp("background", pp->name) == 0) {
      bool is_dark = palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, ctx.max_brightness);
      printf("palette %s with background %s is dark?: %s",
             pname, pp->translated_value,
             is_dark ? "Yes" : "No"
             );
    }
    FREE_PALETTE_PROPERTIES(pp);
  }

  return(EXIT_SUCCESS);
} /* kfc_cli_test_colors */


static int kfc_cli_bright_backgrounds(void){
  struct Vector *v  = get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_BRIGHT, ctx.max_brightness);
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


static int kfc_cli_dark_backgrounds(void){
  struct Vector *v  = get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_DARK, ctx.max_brightness);
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


static int kfc_cli_select_palette(void){
  char *palette_name = kfc_utils_select_palette();

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


static int kfc_cli_select_palettes(void){
  struct Vector *selected_palettes = kfc_utils_select_palettes();

  log_info("Selected %lu options", vector_size(selected_palettes));
  for (size_t i = 0; i < vector_size(selected_palettes); i++) {
    log_info(" - %s", (char *)vector_get(selected_palettes, i));
  }
  return(EXIT_SUCCESS);
}


static int kfc_cli_select_apply_palette(void){
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


static int kfc_cli_color_report(void){
  kfc_utils_color_report();
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_escaped_sequence(void){
  char *s     = get_palette_name_sequence(ctx.palette_name);
  char *s_esc = strdup_escaped(s);

  fprintf(stdout, "%s\n", s_esc);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_data(void){
  char *pd = get_palette_name_data(ctx.palette_name);

  if (pd != NULL) {
    fprintf(stdout, "%s\n", pd);
  }
}


static int kfc_cli_reset_terminal(void){
  fprintf(stdout, "%s", get_ansi_reset_sequence());
  fflush(stdout);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_history(void){
  fprintf(stdout, "%s\n", stringfn_mut_trim(get_palette_history()));
  return(EXIT_SUCCESS);
}
#undef KFC
