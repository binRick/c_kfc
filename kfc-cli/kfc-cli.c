#define DEBUG_MODE         false
#define KFC_CLI_VERSION    "0.0.1"
#ifndef DEBUG_MEMORY
//#define DEBUG_MEMORY
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
//#include "debug-memory/debug_memory.h"
#endif
#define IS_DEBUG_MODE          (KFC->mode >= KFC_LOG_DEBUG || ctx.debug_mode == true)
#define MAX_BRIGHTNESS_DESC    "Brightness Threshold (0.00 - 100) (Default: " DEFAULT_MAX_BRIGHTNESS ")"
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
static int kfc_cli_select_palette(void *);
static int kfc_cli_color_report(void);
static int kfc_cli_print_escaped_sequence(void);
static int kfc_cli_print_palette_data(void);
static int kfc_cli_reset_terminal(void);
static int kfc_cli_print_palette_history(void);
static int kfc_cli_render_palettes_template(void);

////////////////////////////////////////////
static struct ctx_t      ctx = {
  .palette_name       = NULL,
  .debug_mode         = false,
  .palette_property   = NULL,
  .max_brightness     = 0.00,
  .palettes_limit_qty = 0,
  .palette_value      = NULL,
  .modes              = NULL,
};
static struct cag_option options[] = {
  { .identifier  = 'u', .access_letters = "u",
    .access_name = "render-palettes-template", .value_name = NULL, .description = "Render Unja Template" },
  { .identifier  = 'v', .access_letters = "v",
    .access_name = "version", .value_name = NULL, .description = "Print Version" },
  { .identifier  = 'G', .access_letters = "G",
    .access_name = "print-palette-history", .value_name = NULL, .description = "Print Palette History" },
  { .identifier  = 'Q', .access_letters = "Q",
    .access_name = "reset-terminal", .value_name = NULL, .description = "Reset Terminal State" },
  { .identifier  = 'e', .access_letters = "e",
    .access_name = "print-palette-data", .value_name = NULL, .description = "Print Palettte Data" },
  { .identifier  = 'E', .access_letters = "E",
    .access_name = "print-palette-sequence", .value_name = NULL,
    .description = AC_RESETALL AC_YELLOW "Print Palette Escaped Sequence" AC_RESETALL },
  { .identifier  = 'R', .access_letters = "R",
    .access_name = "print-color-demo", .value_name = NULL, .description = "Print Color Demo" },
  { .identifier  = 'A', .access_letters = "A",
    .access_name = "select-apply-palette", .value_name = NULL,
    .description = "Select & Apply Palettes" },
  { .identifier  = 'S', .access_letters = "S",
    .access_name = "select-palettes", .value_name = NULL, .description = "Select Palettes" },
  { .identifier  = 's', .access_letters = "s",
    .access_name = "select-palette", .value_name = NULL, .description = "Select Palette" },
  { .identifier  = 'k', .access_letters = "k",
    .access_name = "dark-backgrounds", .value_name = NULL, .description = "Print Dark Background Palettes" },
  { .identifier  = 'b', .access_letters = "b",
    .access_name = "bright-backgrounds", .value_name = NULL, .description = "Print Bright Background Palettes" },
  { .identifier  = 'B', .access_letters = "B",
    .access_name = "max-brightness", .value_name = "MAX_BRIGHTNESS",
    .description = MAX_BRIGHTNESS_DESC, },
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
struct kfc_mode_handlers_t {
  int  (*handler)(void *);
  char identifier;
};
static struct kfc_mode_handlers_t kfc_mode_handlers[KFC_CLI_MODES_QTY] = {
  [KFC_CLI_MODE_LIST_PALETTES]                    = { .handler = kfc_cli_list_palettes,                    .identifier = 'l', },
  [KFC_CLI_MODE_LOAD_PALETTE]                     = { .handler = kfc_cli_load_palette,                     .identifier = 'L', },
  [KFC_CLI_MODE_PRINT_PALETTES_TABLE]             = { .handler = kfc_cli_print_palettes_table,             .identifier = 't', },
  [KFC_CLI_MODE_PRINT_PALETTE_TABLE]              = { .handler = kfc_cli_print_palette_table,              .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE]   = { .handler = kfc_cli_print_palette_properties_table,   .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_VERSION]                    = { .handler = kfc_cli_print_version,                    .identifier = 'v', },
  [KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES]  = { .handler = kfc_cli_print_unique_palette_properties,  .identifier = 'U', },
  [KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES] = { .handler = kfc_cli_print_invalid_palette_properties, .identifier = 'I', },
  [KFC_CLI_MODE_DETECT_TERMINAL_TYPE]             = { .handler = kfc_cli_detect_terminal_type,             .identifier = 'D', },
  [KFC_CLI_MODE_TEST_KITTY_SOCKET]                = { .handler = kfc_cli_test_kitty_socket,                .identifier = 'K', },
  [KFC_CLI_MODE_PRINT_USAGE]                      = { .handler = kfc_cli_print_usage,                      .identifier = 'h', },
  [KFC_CLI_MODE_TEST_COLORS]                      = { .handler = kfc_cli_test_colors,                      .identifier = 'C' },
  [KFC_CLI_MODE_BRIGHT_BACKGROUNDS]               = { .handler = kfc_cli_bright_backgrounds,               .identifier = 'b', },
  [KFC_CLI_MODE_DARK_BACKGROUNDS]                 = { .handler = kfc_cli_dark_backgrounds,                 .identifier = 'k', },
  [KFC_CLI_MODE_SELECT_PALETTE]                   = { .handler = kfc_cli_select_palette,                   .identifier = 's', },
  [KFC_CLI_MODE_SELECT_APPLY_PALETTE]             = { .handler = kfc_cli_select_apply_palette,             .identifier = 'L', },
  [KFC_CLI_MODE_SELECT_PALETTES]                  = { .handler = kfc_cli_select_palettes,                  .identifier = 'S', },
  [KFC_CLI_MODE_COLOR_REPORT]                     = { .handler = kfc_cli_color_report,                     .identifier = 'C', },
  [KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE]   = { .handler = kfc_cli_print_escaped_sequence,           .identifier = 'E', },
  [KFC_CLI_MODE_PRINT_PALETTE_DATA]               = { .handler = kfc_cli_print_palette_data,               .identifier = 'e', },
  [KFC_CLI_MODE_RESET_TERMINAL]                   = { .handler = kfc_cli_reset_terminal,                   .identifier = 'Q', },
  [KFC_CLI_MODE_PRINT_PALETTE_HISTORY]            = { .handler = kfc_cli_print_palette_history,            .identifier = 'G', },
  [KFC_CLI_MODE_RENDER_UNJA_TEMPLATE]             = { .handler = kfc_cli_render_palettes_template,         .identifier = 'u', },
};
void __attribute__((constructor)) __kfc_cli_constructor(){
  ctx.max_brightness = atof(DEFAULT_MAX_BRIGHTNESS);
  ctx.modes          = vector_new();
  KFC                = require(kfc_utils);
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
#ifdef DEBUG_MEMORY
  if (IS_DEBUG_MODE) {
    log_info("<%d> [%s] Checking for memory leaks", getpid(), __FUNCTION__);
  }
//  print_allocated_memory();
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


static int add_mode_to_ctx_modes(enum kfc_mode_t MODE){
  if (MODE >= KFC_CLI_MODES_QTY) {
    return(-1);
  }
  vector_push(ctx.modes, (void *)MODE);
  return(0);
}


static int parse_args(int argc, char *argv[]){
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    char identifier = cag_option_get(&context);
    switch (identifier) {
    case 'd': ctx.debug_mode         = true; break;
    case 'q': ctx.palettes_limit_qty = KFC->palettes_limit_qty = atoi(cag_option_get_value(&context));  break;
    case 'p': ctx.palette_name       = cag_option_get_value(&context); break;
    case 'r': ctx.palette_name       = get_palette_name_by_index(random_palette_index()); break;
    case 'B': ctx.max_brightness     = atof(cag_option_get_value(&context)); break;
    case 'P': ctx.palette_property   = cag_option_get_value(&context); break;
    case 'V': ctx.palette_value      = cag_option_get_value(&context); break;
    }
    for (size_t i = 0; i < (sizeof(options) / sizeof(options[0])); i++) {
      for (size_t ii = 0; ii < KFC_CLI_MODES_QTY && ii < (sizeof(kfc_mode_handlers) / sizeof(kfc_mode_handlers[0])); ii++) {
        if (kfc_mode_handlers[ii].identifier != 0 && options[i].identifier == identifier && options[i].identifier == kfc_mode_handlers[ii].identifier) {
          add_mode_to_ctx_modes(ii);
        }
      }
    }
  }
  if (vector_size(ctx.modes) == 0) {
    if (ctx.palette_name != NULL) {
      add_mode_to_ctx_modes(KFC_CLI_MODE_LOAD_PALETTE);
    }else{
      add_mode_to_ctx_modes(KFC_CLI_MODE_PRINT_USAGE);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */


static int kfc_cli_print_usage(void){
  fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: kfc [OPTION]\n" AC_RESETALL);
  cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_version(void){
  puts("kfc" KFC_CLI_VERSION);
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
  size_t modes_qty = vector_size(ctx.modes);
  int    res       = 1;
  for (size_t i = 0; i < modes_qty; i++) {
    if (ctx.debug_mode == true) {
      log_debug("Loading mode #%d", ctx.mode);
    }
    res = kfc_mode_handlers[(enum kfc_mode_t)(size_t)vector_get(ctx.modes, i)].handler((void *)&ctx);
    assert(res == 0);
  }
  return(res);
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
      printf("palette %s with background %s is dark?: %s\n",
             pname, pp->translated_value,
             is_dark
                ? AC_RESETALL AC_GREEN "Yes" AC_RESETALL
                : AC_RESETALL AC_RED "No" AC_RESETALL
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


static int kfc_cli_select_palette(void *CTX){
  char *palette_name = kfc_utils_select_palette((void *)CTX);

  if (palette_name != NULL) {
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
    //fflush(stdout);
    fprintf(stdout, " \n");
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


static int kfc_cli_render_palettes_template(void){
  char *s = kfc_utils_get_rendered_template();

  puts(s);
  return(0);
}
#undef KFC
