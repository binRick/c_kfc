#define DEBUG_MODE         false
#define KFC_CLI_VERSION    "0.0.1"
////////////////////////////////////////////
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
////////////////////////////////////////////
#define IS_DEBUG_MODE          (KFC->mode >= KFC_LOG_DEBUG || ctx.debug_mode == true)
#define MAX_BRIGHTNESS_DESC    "Brightness Threshold\n\t\t\t\t\t\t(0.00 - 100) (Default: " DEFAULT_MAX_BRIGHTNESS ")"
#define OPTION_MODE_COLOR      AC_BRIGHT_YELLOW AC_BOLD
#define OPTION_VALUE_COLOR     AC_BRIGHT_MAGENTA AC_ITALIC
#define OPTION_COLOR           AC_BRIGHT_CYAN AC_ITALIC
#define KFC_CLI_OPTION_MODE_COLORIZED(MODE_NAME)             AC_RESETALL OPTION_MODE_COLOR MODE_NAME AC_RESETALL
#define KFC_CLI_OPTION_COLORIZED(OPTION_NAME)                AC_RESETALL OPTION_COLOR OPTION_NAME AC_RESETALL
#define KFC_CLI_OPTION_VALUE_COLORIZED(OPTION_VALUE_NAME)    AC_RESETALL OPTION_VALUE_COLOR OPTION_VALUE_NAME AC_RESETALL
////////////////////////////////////////////
#include "bytes/bytes.h"
#include "c_stringfn/include/stringfn.h"
#include "cargs/include/cargs.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "kfc-cli/kfc-cli-vt100-utils.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-colors.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "log.h/log.h"
#include "process/process.h"
#include "rgba/src/rgba.h"
#include "rgba/src/rgba.h"
#include "tempdir.c/tempdir.h"
#include "vt100utils/demos/tuibox.h"
#include "vt100utils/vt100utils.h"
////////////////////////////////////////////
#define KFC    ctx.kfc_utils
////////////////////////////////////////////
static int kfc_cli_load_palette(void);
static int kfc_cli_list_palettes(void);
static int kfc_cli_print_usage(void);
static int kfc_cli_print_palettes_table(void);
static int kfc_cli_print_palette_table(void);
static int kfc_cli_print_palette_colors_table(void);
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
static int kfc_cli_render_bright_colors_demo(void);
static int kfc_cli_vt100utils_demo(void);
static int kfc_cli_render_bright_colors_vt100utils_demo(void);
static int kfc_cli_print_palette_color_property_names(void);
static int kfc_cli_print_palette_colored_property_names(void);
static char *kfc_cli_get_bright_colors_demo_string(void);

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
  { .identifier  = 'h', .access_letters = "h",
    .access_name = "help",
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Show Help / Usage"), },
  { .identifier  = 'c', .access_letters = "c",
    .access_name = "palette-colors-table", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palette Colors Table"), },
  { .identifier  = 'T', .access_letters = "T",
    .access_name = "palette-properties-table", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palette Properties Table"), },
  { .identifier  = 't', .access_letters = "t",
    .access_name = "palettes-table", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palettes Table"), },
  { .identifier  = 'o', .access_letters = "o",
    .access_name = "print-colored-properties", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Colored Property Names"), },
  { .identifier  = 'F', .access_letters = "F",
    .access_name = "print-color-properties", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Color Property Names"), },
  { .identifier  = 'u', .access_letters = "u",
    .access_name = "render-palettes-template", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Render Unja Template"), },
  { .identifier  = 'v', .access_letters = "v",
    .access_name = "version", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Version"), },
  { .identifier  = 'G', .access_letters = "G",
    .access_name = "print-palette-history", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palette History"), },
  { .identifier  = 'Q', .access_letters = "Q",
    .access_name = "reset-terminal", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Reset Terminal State") },
  { .identifier  = 'e', .access_letters = "e",
    .access_name = "print-palette-data", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palettte Data"), },
  { .identifier  = 'E', .access_letters = "E",
    .access_name = "print-palette-sequence", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Palette Escaped Sequence") },
  { .identifier  = 'R', .access_letters = "R",
    .access_name = "print-color-demo", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Color Demo"), },
  { .identifier  = 'A', .access_letters = "A",
    .access_name = "select-apply-palette", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Select & Apply Palettes"), },
  { .identifier  = 'S', .access_letters = "S",
    .access_name = "select-palettes", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Select Palettes"), },
  { .identifier  = 's', .access_letters = "s",
    .access_name = "select-palette", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Select Palette"), },
  { .identifier  = 'Z', .access_letters = "Z",
    .access_name = "bright-colors-demo", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Bright Colors Demo"), },
  { .identifier  = 'k', .access_letters = "k",
    .access_name = "dark-backgrounds", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Dark Background Palettes"), },
  { .identifier  = 'X', .access_letters = "X",
    .access_name = "bright-colors-vt100utils-demo", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Bright Colors VT100 Utils Demo"), },
  { .identifier  = 'z', .access_letters = "z",
    .access_name = "vt100utils-demo", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("VT100 Utils Demo"), },
  { .identifier  = 'b', .access_letters = "b",
    .access_name = "bright-backgrounds", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Bright Background Palettes"), },
  { .identifier  = 'C', .access_letters = "C",
    .access_name = "test-colors", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Test Colors"), },
  { .identifier  = 'K', .access_letters = "K",
    .access_name = "test-kitty-socket", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Test Kitty Socket"), },
  { .identifier  = 'D', .access_letters = "D",
    .access_name = "detect-terminal", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Detect Terminal Type"), },
  { .identifier  = 'U', .access_letters = "U",
    .access_name = "unique-items", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("List Unique Palette Items"), },
  { .identifier  = 'I', .access_letters = "I",
    .access_name = "invalid-items", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("List Invalid Palette Items"), },
  { .identifier  = 'L', .access_letters = "L",
    .access_name = "load-palette", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Load Palette"), },
  { .identifier  = 'l', .access_letters = "l",
    .access_name = "list-palettes", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("List Palettes"), },
//////////////////////////////////////////////////////////////////////////////////////////////////////
  { .identifier  = 'B', .access_letters = "B",
    .access_name = "max-brightness", .value_name = "MAX_BRIGHTNESS",
    .description = KFC_CLI_OPTION_VALUE_COLORIZED(MAX_BRIGHTNESS_DESC), },
  { .identifier  = 'p', .access_letters = "p",
    .access_name = "palette", .value_name = "PALETTE_NAME",
    .description = KFC_CLI_OPTION_VALUE_COLORIZED("Specified Palette"), },
  { .identifier  = 'V', .access_letters = "V",
    .access_name = "palette-value", .value_name = "PALETTE_VALUE",
    .description = KFC_CLI_OPTION_VALUE_COLORIZED("Specified Palette Value"), },
  { .identifier  = 'P', .access_letters = "P",
    .access_name = "palette-property", .value_name = "PALETTE_PROPERTY",
    .description = KFC_CLI_OPTION_VALUE_COLORIZED("Specified Palette Property"), },
  { .identifier  = 'q', .access_letters = "q",
    .access_name = "limit-palettes-qty", .value_name = "PALETTES_QTY",
    .description = KFC_CLI_OPTION_VALUE_COLORIZED("Limit Palettes Qty"), },
//////////////////////////////////////////////////////////////////////////////////////////////////////
  { .identifier  = 'r', .access_letters = "r",
    .access_name = "random-palette", .value_name = NULL,
    .description = KFC_CLI_OPTION_COLORIZED("Load Random Palette"), },
  { .identifier  = 'd', .access_letters = "d",
    .access_name = "debug", .value_name = NULL,
    .description = KFC_CLI_OPTION_COLORIZED("Debug Mode"), },
};
struct kfc_mode_handlers_t {
  int  (*handler)(void *);
  char identifier;
};
static struct kfc_mode_handlers_t kfc_mode_handlers[KFC_CLI_MODES_QTY] = {
  [KFC_CLI_MODE_LIST_PALETTES]                        = { .handler = kfc_cli_list_palettes,                        .identifier = 'l', },
  [KFC_CLI_MODE_LOAD_PALETTE]                         = { .handler = kfc_cli_load_palette,                         .identifier = 'L', },
  [KFC_CLI_MODE_PRINT_PALETTES_TABLE]                 = { .handler = kfc_cli_print_palettes_table,                 .identifier = 't', },
  [KFC_CLI_MODE_PRINT_PALETTE_TABLE]                  = { .handler = kfc_cli_print_palette_table,                  .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE]       = { .handler = kfc_cli_print_palette_properties_table,       .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_VERSION]                        = { .handler = kfc_cli_print_version,                        .identifier = 'v', },
  [KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES]      = { .handler = kfc_cli_print_unique_palette_properties,      .identifier = 'U', },
  [KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES]     = { .handler = kfc_cli_print_invalid_palette_properties,     .identifier = 'I', },
  [KFC_CLI_MODE_DETECT_TERMINAL_TYPE]                 = { .handler = kfc_cli_detect_terminal_type,                 .identifier = 'D', },
  [KFC_CLI_MODE_TEST_KITTY_SOCKET]                    = { .handler = kfc_cli_test_kitty_socket,                    .identifier = 'K', },
  [KFC_CLI_MODE_PRINT_USAGE]                          = { .handler = kfc_cli_print_usage,                          .identifier = 'h', },
  [KFC_CLI_MODE_TEST_COLORS]                          = { .handler = kfc_cli_test_colors,                          .identifier = 'C' },
  [KFC_CLI_MODE_BRIGHT_BACKGROUNDS]                   = { .handler = kfc_cli_bright_backgrounds,                   .identifier = 'b', },
  [KFC_CLI_MODE_VT100UTILS_DEMO]                      = { .handler = kfc_cli_vt100utils_demo,                      .identifier = 'z', },
  [KFC_CLI_MODE_DARK_BACKGROUNDS]                     = { .handler = kfc_cli_dark_backgrounds,                     .identifier = 'k', },
  [KFC_CLI_MODE_SELECT_PALETTE]                       = { .handler = kfc_cli_select_palette,                       .identifier = 's', },
  [KFC_CLI_MODE_SELECT_APPLY_PALETTE]                 = { .handler = kfc_cli_select_apply_palette,                 .identifier = 'L', },
  [KFC_CLI_MODE_SELECT_PALETTES]                      = { .handler = kfc_cli_select_palettes,                      .identifier = 'S', },
  [KFC_CLI_MODE_COLOR_REPORT]                         = { .handler = kfc_cli_color_report,                         .identifier = 'C', },
  [KFC_CLI_MODE_PALETTE_PRINT_ESCAPED_SEQUENCE]       = { .handler = kfc_cli_print_escaped_sequence,               .identifier = 'E', },
  [KFC_CLI_MODE_PRINT_PALETTE_DATA]                   = { .handler = kfc_cli_print_palette_data,                   .identifier = 'e', },
  [KFC_CLI_MODE_RESET_TERMINAL]                       = { .handler = kfc_cli_reset_terminal,                       .identifier = 'Q', },
  [KFC_CLI_MODE_PRINT_PALETTE_HISTORY]                = { .handler = kfc_cli_print_palette_history,                .identifier = 'G', },
  [KFC_CLI_MODE_RENDER_UNJA_TEMPLATE]                 = { .handler = kfc_cli_render_palettes_template,             .identifier = 'u', },
  [KFC_CLI_MODE_RENDER_BRIGHT_COLORS_DEMO]            = { .handler = kfc_cli_render_bright_colors_demo,            .identifier = 'z', },
  [KFC_CLI_MODE_BRIGHT_COLORS_VT100UTILS_DEMO]        = { .handler = kfc_cli_render_bright_colors_vt100utils_demo, .identifier = 'X', },
  [KFC_CLI_MODE_PRINT_PALETTE_COLORS_TABLE]           = { .handler = kfc_cli_print_palette_colors_table,           .identifier = 'c', },
  [KFC_CLI_MODE_PRINT_PALETTE_COLOR_PROPERTY_NAMES]   = { .handler = kfc_cli_print_palette_color_property_names,   .identifier = 'F', },
  [KFC_CLI_MODE_PRINT_PALETTE_COLORED_PROPERTY_NAMES] = { .handler = kfc_cli_print_palette_colored_property_names, .identifier = 'o', },
};
void __attribute__((constructor)) __kfc_cli_constructor(){
  ctx.max_brightness = atof(DEFAULT_MAX_BRIGHTNESS);
  ctx.modes          = vector_new();
  KFC                = require(kfc_utils);
}

void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
  if (IS_DEBUG_MODE) {
    log_info("<%d> [%s] OK", getpid(), __FUNCTION__);
  }
}


static int kfc_cli_print_invalid_palette_properties(void){
  struct Vector *v = kfc_utils_get_invalid_palette_property_names();

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
  struct Vector *unique_palette_properties_v = kfc_utils_get_unique_palette_property_names();

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
    case 'r': ctx.palette_name       = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index()); break;
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
  fprintf(stdout,
          AC_RESETALL AC_GREEN AC_BOLD "Usage: kfc" AC_RESETALL
          AC_RESETALL " " AC_RESETALL
          OPTION_MODE_COLOR "[MODE]" AC_RESETALL
          AC_RESETALL " " AC_RESETALL
          OPTION_VALUE_COLOR "[OPTION-VALUE]" AC_RESETALL
          AC_RESETALL " " AC_RESETALL
          OPTION_COLOR "[OPTION]" AC_RESETALL
          AC_RESETALL "\n" AC_RESETALL
          );
  cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_version(void){
  puts("kfc" KFC_CLI_VERSION);
  return(0);
}


static int kfc_cli_print_palette_properties_table(void){
  if (ctx.palette_name == NULL) {
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
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
  struct inc_palette_t *P = kfc_utils_get_palette_t_by_name(ctx.palette_name);
  if (P == NULL) {
    fprintf(stderr, AC_RESETALL AC_RED "Palette '%s' not found" AC_RESETALL "\n", ctx.palette_name);
    return(1);
  }
  if (ctx.debug_mode == true) {
    fprintf(stderr, AC_RESETALL AC_GREEN "Loading embedded palette %s of %s" AC_RESETALL "\n", P->name, bytes_to_string(P->size));
  }
  size_t written_bytes = kfc_utils_load_palette_name(P->name);
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
  struct palette_property_t *p = kfc_utils_get_palette_name_properties_v(pname);
  for (size_t i = 0; i < vector_size(p); i++) {
    struct palette_property_t *pp = vector_get(p, i);
    if (strcmp("background", pp->name) == 0) {
      bool is_dark = kfc_utils_palette_background_is_brightness_type(pp->translated_value, BACKGROUND_BRIGHTNESS_DARK, ctx.max_brightness);
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
  struct Vector *v  = kfc_utils_get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_BRIGHT, ctx.max_brightness);
  size_t        qty = vector_size(v);

  for (size_t i = 0; i < vector_size(v); i++) {
    char *pn = vector_get(v, i);
    puts(pn);
    free(pn);
  }
  vector_release(v);
  if (ctx.debug_mode) {
    fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu bright palettes\n" AC_RESETALL, qty);
  }
  return(EXIT_SUCCESS);
}


static int kfc_cli_dark_backgrounds(void){
  struct Vector *v  = kfc_utils_get_palette_names_by_brightness_type(BACKGROUND_BRIGHTNESS_DARK, ctx.max_brightness);
  size_t        qty = vector_size(v);

  for (size_t i = 0; i < vector_size(v); i++) {
    char *pn = vector_get(v, i);
    puts(pn);
    free(pn);
  }
  vector_release(v);
  if (ctx.debug_mode) {
    fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu dark palettes\n" AC_RESETALL, qty);
  }
  return(EXIT_SUCCESS);
}


static int kfc_cli_select_palette(void *CTX){
  char *palette_name = kfc_utils_select_palette((void *)CTX);

  if (palette_name != NULL) {
    kfc_utils_load_palette_name(palette_name);
    fflush(stdout);
    fprintf(stderr, "%s\n", palette_name);
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
    kfc_utils_load_palette_name(palette_name);
    //fflush(stdout);
    fprintf(stderr, " \n");
    fprintf(stderr, "%s\n", palette_name);
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
  if (ctx.palette_name == NULL) {
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  }
  char *s     = kfc_utils_get_palette_name_sequence(ctx.palette_name);
  char *s_esc = strdup_escaped(s);

  if (ctx.debug_mode) {
    fprintf(stderr, "%s\n", ctx.palette_name);
  }
  fprintf(stdout, "%s\n", s_esc);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_data(void){
  if (ctx.palette_name == NULL) {
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  }
  char *pd = kfc_utils_get_palette_name_data(ctx.palette_name);

  if (pd != NULL) {
    if (ctx.debug_mode) {
      fprintf(stderr, "%s\n", ctx.palette_name);
    }
    fprintf(stdout, "%s\n", pd);
  }
}


static int kfc_cli_reset_terminal(void){
  fprintf(stdout, "%s", kfc_utils_get_ansi_reset_sequence());
  fflush(stdout);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_history(void){
  fprintf(stdout, "%s\n", stringfn_mut_trim(kfc_utils_get_palette_history()));
  return(EXIT_SUCCESS);
}


static int kfc_cli_render_palettes_template(void){
  char   *s     = kfc_utils_get_rendered_template();
  size_t qty    = strlen(s);
  bool   qty_ok = (qty > 1024);

  printf("kfc_utils_get_rendered_template returned %lub\n", qty);

  return((qty_ok == true) ? 0 : 1);
}


static char *kfc_cli_get_bright_colors_demo_string(void){
  struct StringBuffer *sb  = stringbuffer_new();
  size_t              cqty = 0;

  for (size_t i = 0; i < 5; i++) {
    bright_color_init(i);
    for (int x = 0; x < 5; x++) {
      float  *c = calloc(4, sizeof(float));
      bright_color(x, c);
      char   s[1024];
      char   hex_s[256];
      int    rgb_i[3] = {
        (int)round(c[0] * 255.0),
        (int)round(c[1] * 255.0),
        (int)round(c[2] * 255.0),
      };
      rgba_t rgba_s = {
        .r = c[0],
        .g = c[1],
        .b = c[2],
        .a = 1,
      };
      rgba_to_string(rgba_s, hex_s, 256);

      sprintf(s, AC_RESETALL "Bright Color #%5lu:     "
              "\x1b[38;2;%d;%d;%dm"
              "%3d/%3d/%3d|%7s" AC_RESETALL
              AC_RESETALL "\t"
              AC_RESETALL "\x1b[48;2;%d;%d;%dm"
              "                   " AC_RESETALL
              "",
              cqty + 1,
              rgb_i[0], rgb_i[1], rgb_i[2],
              rgb_i[0], rgb_i[1], rgb_i[2],
              hex_s,
              rgb_i[0], rgb_i[1], rgb_i[2]
              );
      stringbuffer_append_string(sb, strdup(s));
      stringbuffer_append_string(sb, "\n");
      free(c);
      cqty++;
    }
  }
  char *ret_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  return(ret_s);
} /* get_bright_colors_demo_string */


static int kfc_cli_render_bright_colors_demo(void){
  char *bright_colors = kfc_cli_get_bright_colors_demo_string();

  fprintf(stdout, "%s\n", bright_colors);
  return(EXIT_SUCCESS);
}


static int kfc_cli_render_bright_colors_vt100utils_demo(void){
  char *bright_colors = kfc_cli_get_bright_colors_demo_string();

  fprintf(stdout, AC_ALT_SCREEN_ON);
  fprintf(stdout, "%s\n", bright_colors);
  fprintf(stdout, AC_ALT_SCREEN_OFF);
  return(EXIT_SUCCESS);

  struct vt100_node_t *tmp;
  int                 x;
  int                 y;

  head = vt100_decode(bright_colors);
  ui_new(0, &u);

  x   = (u.ws.ws_col - 50) / 2;
  y   = (u.ws.ws_row - 10) / 2;
  tmp = head->next;

  while (tmp != NULL) {
    ui_add(
      x, y,
      tmp->len, 1,
      0,
      NULL, 0,
      draw,
      click,
      NULL,
      tmp,
      NULL,
      &u
      );
    x += tmp->len;
    if (x > (u.ws.ws_col + 50) / 2) {
      x  = (u.ws.ws_col - 50) / 2;
      y += 2;
    }
    tmp = tmp->next;
  }

  ui_key("q", stop, &u);

  ui_draw(&u);

  ui_loop(&u) {
    ui_update(&u);
  }

  printf("kk\n");
  return(EXIT_SUCCESS);
} /* kfc_cli_render_bright_colors_vt100utils_demo */


static int kfc_cli_vt100utils_demo(void){
  struct vt100_node_t *tmp;
  int                 x;
  int                 y;

  head = vt100_decode("\x1B[38;5;100mClick\x1B[38;5;101many\x1B[38;5;102mword\x1B[38;5;103mto\x1B[38;5;104mchange\x1B[38;5;105mits\x1B[38;5;106mcolor!\x1B[38;5;107mThis\x1B[38;5;108mis\x1B[38;5;109ma\x1B[38;5;110mlong\x1B[38;5;111mparagraph\x1B[38;5;112mof\x1B[38;5;113mtext,\x1B[38;5;114mand\x1B[38;5;115mevery\x1B[38;5;116mword\x1B[38;5;117mcan\x1B[38;5;118mbe\x1B[38;5;119mclicked.\x1B[38;5;120mWhile\x1B[38;5;121mbehavior\x1B[38;5;122mlike\x1B[38;5;123mthis\x1B[38;5;124mis\x1B[38;5;125mpossible\x1B[38;5;126mwith\x1B[38;5;127mstandalone\x1B[38;5;128mtuibox\x1B[38;5;129m(or\x1B[38;5;130mother\x1B[38;5;131mlibraries),\x1B[38;5;132mit\x1B[38;5;133mwould\x1B[38;5;134mbe\x1B[38;5;135mincredibly\x1B[38;5;136mchallenging\x1B[38;5;137mand\x1B[38;5;138mcumbersome.\x1B[38;5;139mThis\x1B[38;5;140mdemo\x1B[38;5;141mis\x1B[38;5;142mwritten\x1B[38;5;143min\x1B[38;5;144mless\x1B[38;5;145mthan\x1B[38;5;146m100\x1B[38;5;147mlines\x1B[38;5;148mof\x1B[38;5;149mcode.");

  ui_new(0, &u);

  x = (u.ws.ws_col - 50) / 2;
  y = (u.ws.ws_row - 10) / 2;

  tmp = head->next;
  while (tmp != NULL) {
    ui_add(
      x, y,
      tmp->len, 1,
      0,
      NULL, 0,
      draw,
      click,
      NULL,
      tmp,
      NULL,
      &u
      );
    x += tmp->len;
    if (x > (u.ws.ws_col + 50) / 2) {
      x  = (u.ws.ws_col - 50) / 2;
      y += 2;
    }
    tmp = tmp->next;
  }

  ui_key("q", stop, &u);

  ui_draw(&u);

  ui_loop(&u) {
    ui_update(&u);
  }
  return(EXIT_SUCCESS);
} /* kfc_cli_vt100utils_demo */


static int kfc_cli_print_palette_colors_table(void){
  if (ctx.palette_name == NULL) {
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  }
  char *palette_colors_table = kfc_utils_get_palette_colors_table(ctx.palette_name);
  printf("%s\n", palette_colors_table);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_colored_property_names(void){
  if (ctx.palette_name == NULL) {
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  }
  char *s = kfc_utils_get_palette_colored_properties(ctx.palette_name);
  if (ctx.debug_mode) {
    fprintf(stderr, AC_RESETALL AC_YELLOW "%s" AC_RESETALL "\n", ctx.palette_name);
  }
  fprintf(stdout, "%s\n", s);
  return(EXIT_SUCCESS);
}


static int kfc_cli_print_palette_color_property_names(void){
  struct Vector *props = kfc_utils_get_palette_property_color_names_v();

  for (size_t i = 0; i < vector_size(props); i++) {
    char *p = vector_get(props, i);
    printf("%s\n", p);
  }
  if (ctx.debug_mode) {
    fprintf(stderr, "%lu color prop names\n", vector_size(props));
  }
}
#undef KFC
