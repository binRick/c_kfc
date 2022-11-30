#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
////////////////////////////////////////////
#define KFC_CLI_VERSION        "0.0.1"
#define IS_DEBUG_MODE          (KFC->mode >= KFC_LOG_DEBUG || ctx.debug_mode == true)
#define MAX_BRIGHTNESS_DESC    "Brightness Threshold\n\t\t\t\t\t\t(0.00 - 100) (Default: " DEFAULT_MAX_BRIGHTNESS ")"
#define OPTION_MODE_COLOR      AC_BRIGHT_YELLOW AC_BOLD
#define OPTION_VALUE_COLOR     AC_BRIGHT_MAGENTA AC_ITALIC
#define OPTION_COLOR           AC_BRIGHT_CYAN AC_ITALIC
#define KFC_CLI_OPTION_MODE_COLORIZED(MODE_NAME)             AC_RESETALL OPTION_MODE_COLOR MODE_NAME AC_RESETALL
#define KFC_CLI_OPTION_COLORIZED(OPTION_NAME)                AC_RESETALL OPTION_COLOR OPTION_NAME AC_RESETALL
#define KFC_CLI_OPTION_VALUE_COLORIZED(OPTION_VALUE_NAME)    AC_RESETALL OPTION_VALUE_COLOR OPTION_VALUE_NAME AC_RESETALL
////////////////////////////////////////////
#include "ansi-rgb-utils/ansi-rgb-utils.h"
#include "ansi-utils/ansi-utils.h"
#include "bytes/bytes.h"
#include "c_fsio/include/fsio.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_stringfn/include/stringfn.h"
#include "c_stringfn/include/stringfn.h"
#include "c_timer/include/c_timer.h"
#include "c_vector/vector/vector.h"
#include "cargs/include/cargs.h"
#include "exec-fzf/exec-fzf.h"
#include "hsluv-c/src/hsluv.h"
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-colors.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "log/log.h"
#include "ms/ms.h"
#include "rgba/src/rgba.h"
#include "tempdir.c/tempdir.h"
#include "tiny-regex-c/re.h"
////////////////////////////////////////////
#define KFC    ctx.kfc_utils
////////////////////////////////////////////
static int kfc_cli_load_palette(void);
static int kfc_cli_list_palettes(void);
static int kfc_cli_print_last_palette_name(void);
static int kfc_cli_print_palette_background(void);
static int kfc_cli_print_usage(void);
static int kfc_cli_print_palettes_table(void);
static int kfc_cli_print_palette_table(void);
static int kfc_cli_print_palette_colors_table(void);
static int kfc_cli_print_version(void);
static int kfc_cli_print_palette_properties_table(void);
static int kfc_cli_print_invalid_palette_properties(void);
static int kfc_cli_print_unique_palette_properties(void);
static int kfc_cli_detect_terminal_type(void);
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
static int kfc_cli_print_color_boxes(void);
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
  { .identifier  = 'N', .access_letters = "N",
    .access_name = "palette-background", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Last Loaded Palette Name"), },
  { .identifier  = 'O', .access_letters = "O",
    .access_name = "last-palette-name", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Last Loaded Palette Name"), },
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
  { .identifier  = 'H', .access_letters = "H",
    .access_name = "print-color-boxes", .value_name = NULL,
    .description = KFC_CLI_OPTION_MODE_COLORIZED("Print Color Boxes"), },
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
} static kfc_mode_handlers[KFC_CLI_MODES_QTY] = {
  [KFC_CLI_MODE_LIST_PALETTES]                        = { .handler = kfc_cli_list_palettes,                        .identifier = 'l', },
  [KFC_CLI_MODE_PRINT_LAST_LOADED_PALETTE_NAME]       = { .handler = kfc_cli_print_last_palette_name,              .identifier = 'O', },
  [KFC_CLI_MODE_PRINT_PALETTE_BACKGROUND]             = { .handler = kfc_cli_print_palette_background,             .identifier = 'N', },
  [KFC_CLI_MODE_LOAD_PALETTE]                         = { .handler = kfc_cli_load_palette,                         .identifier = 'L', },
  [KFC_CLI_MODE_PRINT_PALETTES_TABLE]                 = { .handler = kfc_cli_print_palettes_table,                 .identifier = 't', },
  [KFC_CLI_MODE_PRINT_PALETTE_TABLE]                  = { .handler = kfc_cli_print_palette_table,                  .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_PALETTE_PROPERTIES_TABLE]       = { .handler = kfc_cli_print_palette_properties_table,       .identifier = 'T', },
  [KFC_CLI_MODE_PRINT_VERSION]                        = { .handler = kfc_cli_print_version,                        .identifier = 'v', },
  [KFC_CLI_MODE_PRINT_UNIQUE_PALETTE_PROPERTIES]      = { .handler = kfc_cli_print_unique_palette_properties,      .identifier = 'U', },
  [KFC_CLI_MODE_PRINT_INVALID_PALETTE_PROPERTIES]     = { .handler = kfc_cli_print_invalid_palette_properties,     .identifier = 'I', },
  [KFC_CLI_MODE_DETECT_TERMINAL_TYPE]                 = { .handler = kfc_cli_detect_terminal_type,                 .identifier = 'D', },
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
  [KFC_CLI_MODE_PRINT_COLOR_BOXES]                    = { .handler = kfc_cli_print_color_boxes,                    .identifier = 'H', },
};
static void __attribute__((constructor)) __kfc_cli_constructor(){
  ctx.max_brightness = atof(DEFAULT_MAX_BRIGHTNESS);
  ctx.modes          = vector_new();
  KFC                = require(kfc_utils);
}

static void __attribute__((destructor)) __kfc_cli_destructor(){
  clib_module_free(KFC);
  if (IS_DEBUG_MODE)
    log_info("<%d> [%s] Destructor", getpid(), __FUNCTION__);
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
  if (MODE >= KFC_CLI_MODES_QTY)
    return(-1);

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
    case 'r': ctx.palette_name       = kfc_utils_get_palette_name_by_index(kfc_utils_random_dark_palette_index(ctx.max_brightness)); break;
    case 'B': ctx.max_brightness     = atof(cag_option_get_value(&context)); break;
    case 'P': ctx.palette_property   = cag_option_get_value(&context); break;
    case 'V': ctx.palette_value      = cag_option_get_value(&context); break;
    }
    for (size_t i = 0; i < (sizeof(options) / sizeof(options[0])); i++)
      for (size_t ii = 0; ii < KFC_CLI_MODES_QTY && ii < (sizeof(kfc_mode_handlers) / sizeof(kfc_mode_handlers[0])); ii++)
        if (kfc_mode_handlers[ii].identifier != 0 && options[i].identifier == identifier && options[i].identifier == kfc_mode_handlers[ii].identifier)
          add_mode_to_ctx_modes(ii);
  }
  if (vector_size(ctx.modes) == 0) {
    if (ctx.palette_name != NULL)
      add_mode_to_ctx_modes(KFC_CLI_MODE_LOAD_PALETTE);
    else
      add_mode_to_ctx_modes(KFC_CLI_MODE_PRINT_USAGE);
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
  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  if (ctx.debug_mode == true)
    log_debug("Printing Palette %s Properties Table", ctx.palette_name);
  char *tbl = KFC->get_palette_properties_table(ctx.palette_name);
  printf("%s", tbl);

  return(0);
}

static int kfc_cli_print_palette_table(void){
  if (ctx.debug_mode == true)
    log_debug("Printing Palette Table");
  return(0);
}

static int kfc_cli_print_palettes_table(void){
  if (ctx.debug_mode == true)
    log_debug("Printing Palettes Table");
  fprintf(stdout, "%s", KFC->get_palettes_table());
  return(0);
}

static int kfc_cli_print_palette_background(void){
  struct inc_palette_t *P;

  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  if (!(P = kfc_utils_get_palette_t_by_name(ctx.palette_name))) {
    fprintf(stderr, AC_RESETALL AC_RED "Palette '%s' not found" AC_RESETALL "\n", ctx.palette_name);
    return(EXIT_FAILURE);
  }
  int                       ansi_code;
  struct palette_property_t *p = kfc_utils_get_palette_name_properties_v(ctx.palette_name);
  struct palette_property_t *pp;

  for (size_t i = 0; i < vector_size(p); i++)
    if ((pp = vector_get(p, i)) && (strcmp("background", pp->name) == 0))
      if ((ansi_code = au_hex_ansicode(pp->translated_value)) >= 0)
        Dbg(ansi_code, %d);
//      Ds(pp->translated_value);
  //Ds(pp->escaped_code);

  log_info("%s bg", ctx.palette_name);
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_last_palette_name(void){
  char                   *name;
  char                   *history = fsio_read_text_file(kfc_utils_palettes_history_file);
  struct StringFNStrings lines    = stringfn_split_lines_and_trim(history);

  free(history);
  char *last_line;

  if (!(last_line = lines.strings[lines.count - 2])) {
    log_error("Failed to read last history line");
    return(EXIT_FAILURE);
  }
  struct StringFNStrings split = stringfn_split(last_line, ':');

  if (split.count < 4) {
    log_error("Failed to read valid last history line");
    return(EXIT_FAILURE);
  }
  if (!(name = split.strings[1]) || strlen(name) < 1) {
    log_error("Failed to read valid last name");
    return(EXIT_FAILURE);
  }
  fprintf(stdout, "%s\n", name);
  stringfn_release_strings_struct(lines);
  stringfn_release_strings_struct(split);
  return(EXIT_SUCCESS);
}

static int kfc_cli_list_palettes(void){
  if (ctx.debug_mode == true)
    log_debug("Listing embedded");
  struct inc_palette_t *p;
  for (size_t i = 0; i < vector_size(KFC->palettes_v); i++) {
    p = vector_get(KFC->palettes_v, i);
    if (p->name && strlen(p->name) > 0)
      printf("%s\n", p->name);
  }
  return(EXIT_SUCCESS);
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
  if (ctx.debug_mode == true)
    fprintf(stderr, AC_RESETALL AC_GREEN "Loading embedded palette %s of %s" AC_RESETALL "\n", P->name, bytes_to_string(P->size));
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

static int kfc_cli_test_colors(void){
  log_debug("max_brightness: %f", ctx.max_brightness);
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
  if (ctx.debug_mode)
    fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu bright palettes\n" AC_RESETALL, qty);
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
  if (ctx.debug_mode)
    fprintf(stderr, AC_RESETALL AC_YELLOW AC_REVERSED "%lu dark palettes\n" AC_RESETALL, qty);
  return(EXIT_SUCCESS);
}

static int kfc_cli_select_palette(void *CTX){
  char *palette_name = kfc_utils_select_palette((void *)CTX);

  if (palette_name != NULL) {
    kfc_utils_load_palette_name(palette_name);
    fflush(stdout);
    fprintf(stderr, "%s\n", palette_name);
  }else
    fprintf(stdout, "Selected no Palette\n");
  return(EXIT_SUCCESS);
}

static int kfc_cli_select_palettes(void){
  struct Vector *selected_palettes = kfc_utils_select_palettes();

  log_info("Selected %lu options", vector_size(selected_palettes));
  for (size_t i = 0; i < vector_size(selected_palettes); i++)
    log_info(" - %s", (char *)vector_get(selected_palettes, i));
  return(EXIT_SUCCESS);
}

static int kfc_cli_select_apply_palette(void){
  char *palette_name = kfc_utils_select_apply_palette();

  if (palette_name != NULL) {
    kfc_utils_load_palette_name(palette_name);
    fprintf(stderr, " \n");
    fprintf(stderr, "%s\n", palette_name);
  }else
    fprintf(stdout, "Selected no Palette\n");

  return(EXIT_SUCCESS);
}

static int kfc_cli_color_report(void){
  kfc_utils_color_report();
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_escaped_sequence(void){
  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  char *s     = kfc_utils_get_palette_name_sequence(ctx.palette_name);
  char *s_esc = strdup_escaped(s);

  if (ctx.debug_mode)
    fprintf(stderr, "%s\n", ctx.palette_name);
  fprintf(stdout, "%s\n", s_esc);
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_palette_data(void){
  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  char *pd = kfc_utils_get_palette_name_data(ctx.palette_name);

  if (pd != NULL) {
    if (ctx.debug_mode)
      fprintf(stderr, "%s\n", ctx.palette_name);
    fprintf(stdout, "%s\n", pd);
  }
  return(EXIT_SUCCESS);
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
  char                              *msg;
  struct rendered_template_result_t *res = kfc_utils_get_rendered_template();

  if (!res)
    return(-1);

  if (ctx.debug_mode)
    log_debug("kfc_utils_get_rendered_template returned %lub\n", res->qty);

  asprintf(&msg,
           "Rendered " AC_YELLOW "%s" AC_RESETALL " " AC_RED "%s" AC_RESETALL
           " using " AC_YELLOW "%lu" AC_RESETALL " properties from "
           AC_YELLOW "%lu" AC_RESETALL " palettes comprised of "
           AC_YELLOW "%s" AC_RESETALL " and " AC_BLUE "%lu" AC_RESETALL " lines"
           " and template "
           AC_BLUE "%s" AC_RESETALL
           " in " AC_GREEN "%s" AC_RESETALL
           "",
           bytes_to_string(res->size),
           res->path,
           res->palette_file_properties,
           res->qty,
           bytes_to_string(res->palette_file_bytes), res->palette_file_lines,
           res->template_path,
           milliseconds_to_string(res->dur)
           );
  fprintf(stdout, "%s\n", msg);
  free(res);
  return(EXIT_SUCCESS);
}

static char *kfc_cli_get_bright_colors_demo_string(void){
  struct StringBuffer *sb  = stringbuffer_new();
  size_t              cqty = 0;

  for (size_t i = 0; i < 30; i++) {
    bright_color_init(i);
    for (int x = 0; x < 10; x++) {
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

  fprintf(stdout, "%s\n", bright_colors);
  return(EXIT_SUCCESS);
} /* kfc_cli_render_bright_colors_vt100utils_demo */

static int kfc_cli_vt100utils_demo(void){
  return(EXIT_SUCCESS);
} /* kfc_cli_vt100utils_demo */

static int kfc_cli_print_palette_colors_table(void){
  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  char *palette_colors_table = kfc_utils_get_palette_colors_table(ctx.palette_name);
  printf("%s\n", palette_colors_table);
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_palette_colored_property_names(void){
  if (ctx.palette_name == NULL)
    ctx.palette_name = kfc_utils_get_palette_name_by_index(kfc_utils_random_palette_index());
  char *s = kfc_utils_get_palette_colored_properties(ctx.palette_name);
  if (ctx.debug_mode)
    fprintf(stderr, AC_RESETALL AC_YELLOW "%s" AC_RESETALL "\n", ctx.palette_name);
  fprintf(stdout, "%s\n", s);
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_palette_color_property_names(void){
  struct Vector *props = kfc_utils_get_palette_property_color_names_v();

  for (size_t i = 0; i < vector_size(props); i++) {
    char *p = vector_get(props, i);
    printf("%s\n", p);
  }
  if (ctx.debug_mode)
    fprintf(stderr, "%lu color prop names\n", vector_size(props));
  return(EXIT_SUCCESS);
}

static int kfc_cli_print_color_boxes(void){
  char *s = get_color_boxes();

  printf("%s", s);
  return(EXIT_SUCCESS);
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
#undef KFC
