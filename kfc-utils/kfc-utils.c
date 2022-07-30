#pragma once
/////////////////////////////////////
#include <assert.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
/////////////////////////////////////
#define DEBUG_PALETTES          false
#define DEBUG_PALETTE_CODES     false
#define KFC_UTILS_DEBUG_MODE    false
#define PALETTES_QTY_LIMIT      999
/////////////////////////////////////
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "submodules/bench/bench.h"
#include "submodules/c_ansi/ansi-utils/ansi-utils.h"
#include "submodules/c_fsio/include/fsio.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
#include "submodules/c_vector/include/vector.h"
#include "submodules/meson_deps/submodules/bytes/bytes.h"
#include "submodules/meson_deps/submodules/c_vector/include/vector.h"
#include "submodules/meson_deps/submodules/debug-memory/debug_memory.h"
#include "submodules/meson_deps/submodules/libfort/src/fort.h"
#include "submodules/timestamp/timestamp.h"
/////////////////////////////////////
static module(kfc_utils) * KFC;
static char *get_palette_name_property_value(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME);
static char *get_palette_item_sequence(const struct palette_property_t *pp);
static char *get_palette_name_sequence(const char *PALETTE_NAME);
static bool is_valid_palette_item_name(const char *PALETTE_ITEM_NAME);
static char *get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME);

static struct palette_name_translations_t       palette_name_translations[] = {
  { .src = "color0", .dst = "color00", },
  { .src = "color1", .dst = "color01", },
  { .src = "color2", .dst = "color02", },
  { .src = "color3", .dst = "color03", },
  { .src = "color4", .dst = "color04", },
  { .src = "color5", .dst = "color05", },
  { .src = "color6", .dst = "color06", },
  { .src = "color7", .dst = "color07", },
  { .src = "color8", .dst = "color08", },
  { .src = "color9", .dst = "color09", },
  { 0 },
};

static struct palette_code_value_translations_t palette_code_value_translations[] = {
  { .name = "cursorstyle",   .src = "under",     .dst = "3 q", },
  { .name = "cursorstyle",   .src = "block",     .dst = "1 q", },
  { .name = "cursorstyle",   .src = "bar",       .dst = "5 q", },
  { .name = "cursorblink",   .src = "off",       .dst = "l",   },
  { .name = "cursorblink",   .src = "on",        .dst = "h",   },
  { .name = "altscreen",     .src = "off",       .dst = "l",   },
  { .name = "altscreen",     .src = "on",        .dst = "h",   },
  { .name = "screen",        .src = "off",       .dst = "l",   },
  { .name = "screen",        .src = "on",        .dst = "h",   },
  { .name = "cursorvisible", .src = "off",       .dst = "l",   },
  { .name = "cursorvisible", .src = "on",        .dst = "h",   },
  { .name = "reportfocus",   .src = "off",       .dst = "l",   },
  { .name = "reportfocus",   .src = "on",        .dst = "h",   },
  { .name = "mouse",         .src = "clickdrag", .dst = "2h",  },
  { .name = "mouse",         .src = "motion",    .dst = "3h",  },
  { .name = "mouse",         .src = "click",     .dst = "0h",  },
  { .name = "mouse",         .src = "off",       .dst = "0l",  },
  { .name = "reset",         .src = "yes",       .dst = "c",   },
  { 0 },
};
static struct palette_code_t                    palette_codes[] = {
  { .name = "color00",              .code = "]4;0;#",              },
  { .name = "color01",              .code = "]4;1;#",              },
  { .name = "color02",              .code = "]4;2;#",              },
  { .name = "color03",              .code = "]4;3;#",              },
  { .name = "color04",              .code = "]4;4;#",              },
  { .name = "color05",              .code = "]4;5;#",              },
  { .name = "color06",              .code = "]4;6;#",              },
  { .name = "color07",              .code = "]4;7;#",              },
  { .name = "color08",              .code = "]4;8;#",              },
  { .name = "color09",              .code = "]4;9;#",              },
  { .name = "color10",              .code = "]4;10;#",             },
  { .name = "color11",              .code = "]4;11;#",             },
  { .name = "color12",              .code = "]4;12;#",             },
  { .name = "color13",              .code = "]4;13;#",             },
  { .name = "color14",              .code = "]4;14;#",             },
  { .name = "color15",              .code = "]4;15;#",             },
  { .name = "foreground",           .code = "]10;#",               },
  { .name = "background",           .code = "]11;#",               },
  { .name = "selection_background", .code = "]17;#",               },
  { .name = "selection_foreground", .code = "]19;#",               },
  { .name = "title",                .code = "]2;",                 },
  { .name = "cursor",               .code = "]12;#",               },
  { .name = "cursorvisible",        .code = "[?25",                },
  { .name = "cursorstyle",          .code = "[",                   },
  { .name = "cursorblink",          .code = "[?12",                },
  { .name = "altscreen",            .code = "[?47",                },
  { .name = "screen",               .code = "[?5",                 },
  { .name = "icontitle",            .code = "]0;",                 },
  { .name = "icon",                 .code = "]1;",                 },
  { .name = "title",                .code = "]2;",                 },
  { .name = "reportfocus",          .code = "[?1004",              },
  { .name = "mouse",                .code = "[?100",               },
  { .name = "notification",         .code = "]99;i=1:d=1:p=body;", },
  { .name = "reset",                .code = "",                    },
  { 0 },
};

void __attribute__((constructor)) __kfc_utils_constructor(){
  KFC       = require(kfc_utils);
  KFC->mode = KFC_LOG_DEBUG;

  if (KFC_UTILS_DEBUG_MODE == true) {
    printf("<%d> [%s] palettes qty:%lu\n",
           getpid(),
           __FUNCTION__,
           KFC->get_palettes_qty()
           );
  }
}
void __attribute__((destructor)) __kfc__utils_destructor(){
  clib_module_free(KFC);
#ifdef DEBUG_MEMORY
  if (KFC->mode >= KFC_LOG_DEBUG) {
    fprintf(stderr, "<%d> [%s] Checking for memory leaks\n", getpid(), __FUNCTION__);
  }
  print_allocated_memory();
#endif
}
#define FREE_PALETTE_PROPERTIES(pp)    do { \
    if (pp) {                               \
      if (pp->name) {                       \
        free(pp->name);                     \
      }                                     \
      if (pp->translated_name) {            \
        free(pp->translated_name);          \
      }                                     \
      if (pp->value) {                      \
        free(pp->value);                    \
      }                                     \
      if (pp->translated_value) {           \
        free(pp->translated_value);         \
      }                                     \
      free(pp);                             \
    }                                       \
} while (0)


/////////////////////////////////////////////////////////////////////
char *get_palette_properties_table(const char *PALETTE_NAME){
  struct palette_property_t *palette_properties_v = get_palette_name_properties_v(PALETTE_NAME);
  ft_table_t                *table                = ft_create_table();

  ft_set_border_style(table, FT_SOLID_ROUND_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);

  ft_write_ln(table,
              "Property",
              "Value",
              "Valid Property",
              "Property is Translated",
              "Value is Translated",
              "Prefix",
              "Code",
              "Translated Value",
              "Suffix",
              "Sequence"
              );

  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    struct palette_property_t *pp = vector_get(palette_properties_v, i);
    ft_printf_ln(table,
                 "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s",
                 pp->name,
                 pp->value,
                 pp->is_valid_name ? "Yes" : "No",
                 pp->translated_name,
                 pp->is_translated ? "Yes" : "No",
                 pp->escaped_prefix,
                 pp->escaped_code,
                 pp->escaped_translated_value,
                 pp->escaped_suffix,
                 pp->escaped_sequence
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_RED);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_RED);
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, pp->is_valid_name ? FT_COLOR_GREEN : FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, pp->is_translated ? FT_COLOR_GREEN : FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
    ft_set_cell_prop(table, i + 1, 5, FT_CPROP_CONT_FG_COLOR, FT_COLOR_CYAN);
    ft_set_cell_prop(table, i + 1, 6, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_RED);
    ft_set_cell_prop(table, i + 1, 7, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 8, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 8, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    ft_set_cell_prop(table, i + 1, 9, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
    ft_set_cell_prop(table, i + 1, 9, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
    ft_set_cell_prop(table, i + 1, 10, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 10, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    FREE_PALETTE_PROPERTIES(pp);
  }
  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  char *seq_summary, *seq_title;

  asprintf(&seq_title,
           AC_RESETALL "Palette"
           AC_RESETALL " "
           AC_RESETALL AC_CYAN AC_BOLD "%s"
           AC_RESETALL " ",
           PALETTE_NAME
           );
  struct StringBuffer *sb = stringbuffer_new();

  stringbuffer_append_string(sb, seq_title);
  stringbuffer_append_string(sb, "\n");
  stringbuffer_append_string(sb, table_s);
  char *seq = get_palette_name_sequence(PALETTE_NAME);

  asprintf(&seq_summary,
           AC_RESETALL AC_YELLOW AC_BOLD "%s"
           AC_RESETALL " Sequence from "
           AC_RESETALL AC_BLUE AC_REVERSED "%lu"
           AC_RESETALL " properties: "
           AC_RESETALL AC_YELLOW AC_ITALIC AC_REVERSED "%s" AC_RESETALL,
           bytes_to_string(strlen(seq)),
           vector_size(palette_properties_v),
           strdup_escaped(seq)
           );
  stringbuffer_append_string(sb, seq_summary);
  stringbuffer_append_string(sb, "\n");
  char *table_seq_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  vector_release(palette_properties_v);
  return(table_seq_s);
} /* get_palette_properties_table */


char *get_palettes_table() {
  unsigned long started_ts = timestamp();
  ft_table_t    *table     = ft_create_table();

  ft_set_border_style(table, FT_SOLID_ROUND_STYLE);
  ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 0);
  ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, 0);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_BG_COLOR, FT_COLOR_BLACK);

  ft_write_ln(table,
              "Palette",
              "Size",
              "File",
              "# Props"
              );

  for (size_t i = 0; i < vector_size(KFC->palettes_v) && i < PALETTES_QTY_LIMIT; i++) {
    struct inc_palette_t *p  = vector_get(require(kfc_utils)->palettes_v, i);
    struct Vector        *pp = get_palette_name_properties_v(p->name);
    ft_printf_ln(table,
                 "%s|%s|%s|%lu",
                 p->name,
                 bytes_to_string(p->size),
                 p->file,
                 vector_size(pp)
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
    ft_set_cell_prop(table, i + 1, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_BLUE);
    ft_set_cell_prop(table, i + 1, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);

    for (size_t i = 0; i < vector_size(pp); i++) {
      struct palette_property_t *_pp = vector_get(pp, i);
      FREE_PALETTE_PROPERTIES(_pp);
    }

    vector_release(pp);
  }

  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  unsigned long ended_ts = timestamp();

  printf("dur:%lu\n", ended_ts - started_ts);
  return(table_s);
} /* get_palettes_table */
/////////////////////////////////////////////////////////////////////
struct Vector *get_palette_name_properties_v(const char *PALETTE_NAME){
  struct Vector        *v = vector_new();
  struct inc_palette_t *p = get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    fprintf(stderr, "No properties for palette %s found\n", PALETTE_NAME);
    return(v);
  }
  struct StringFNStrings lines = stringfn_split_lines_and_trim(p->data);

  for (size_t i = 0; i < lines.count; i++) {
    struct StringFNStrings items = stringfn_split(lines.strings[i], '=');
    if (items.count < 2) {
      continue;
    }
    if (strlen(items.strings[0]) == 0) {
      continue;
    }
    struct palette_property_t *pp = malloc(sizeof(struct palette_property_t));
    pp->name                     = strdup(items.strings[0]);
    pp->value                    = strdup(items.strings[1]);
    pp->escaped_value            = strdup_escaped(pp->value);
    pp->is_valid_name            = is_valid_palette_item_name(pp->name);
    pp->is_translated            = palette_item_name_is_translated(pp->name);
    pp->translated_name          = (pp->is_valid_name == true) ? strdup(pp->name) : get_translated_palette_property_name(pp->name);
    pp->translated_value         = (pp->is_translated == true) ? translate_palette_item_value(pp->translated_name, pp->value) : strdup(pp->value);
    pp->escaped_translated_value = strdup_escaped(pp->translated_value);
    pp->code                     = get_palette_item_code(pp->translated_name);
    pp->escaped_code             = pp->code ? strdup_escaped(pp->code) : "";
    pp->sequence                 = get_palette_item_sequence(pp);
    pp->escaped_sequence         = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(pp->sequence) : "";
    pp->suffix                   = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_SUFFIX) : "";
    pp->prefix                   = is_valid_palette_item_name(pp->translated_name) ? strdup_escaped(CODE_PREFIX) : "";
    pp->escaped_prefix           = strdup_escaped(pp->prefix);
    pp->escaped_suffix           = strdup_escaped(pp->suffix);
    vector_push(v, pp);
    stringfn_release_strings_struct(items);
  }
  return(v);
}


size_t get_palettes_data_bytes(){
  size_t               s    = 0;
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    s += (size_t)strlen(tmp->data);
  }
  return(s);
}

struct Vector *get_palette_names_v(){
  struct Vector        *v   = vector_new();
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    vector_push(v, (char *)tmp->name);
  }
  return(v);
}

struct Vector *get_palettes_v(){
  struct Vector        *v   = vector_new();
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL && i < PALETTES_QTY_LIMIT; tmp++, i++) {
    vector_push(v, (void *)tmp);
  }
  return(v);
}


int kfc_utils_module_test(void) {
  KFC->mode = KFC_LOG_DEBUG;
  printf("palettes vector qty:%lu\n", vector_size(KFC->palettes_v));
  printf("palette names vector qty:%lu\n", vector_size(KFC->palette_names_v));
  printf("palettes data bytes:%s\n", bytes_to_string(KFC->palettes_data_bytes));
  printf("palettes table:\n%s\n", KFC->get_palettes_table());
  for (size_t i = 0; i < 2; i++) {
    struct inc_palette_t *p = KFC->get_palette_t_by_index(i);
    printf("palette #%lu> %s\n%s\n", i, p->name, get_palette_properties_table(p->name));
  }
  printf("palette #%s\n%s\n", "vscode", KFC->get_palette_properties_table("vscode"));

  size_t i     = KFC->random_palette_index();
  char   *name = KFC->get_palette_name_by_index(i);
  printf("random palette index:%lu> |%s|%s|%s|\n",
         i,
         name,
         (KFC->get_palette_t_by_index(i))->name,
         KFC->get_palette_t_by_name(name)->name
         );
  if (name) {
    free(name);
  }

  clib_module_free(KFC);
  return(0);
}


/////////////////////////////////////////////////////////////////////


static char *get_translated_palette_property_name(const char *PALETTE_PROPERTY_NAME){
  struct palette_name_translations_t *tmp = palette_name_translations;

  for (size_t i = 0; tmp->src != NULL && tmp->dst != NULL && tmp != NULL; tmp++, i++) {
    if (strcmp(PALETTE_PROPERTY_NAME, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }
  return(strdup(PALETTE_PROPERTY_NAME));
}


bool palette_item_name_is_translated(const char *ITEM_NAME){
  struct palette_code_value_translations_t *tmp = palette_code_value_translations;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0) {
      return(true);
    }
  }
  return(false);
}


char *translate_palette_item_value(const char *ITEM_NAME, const char *ITEM_VALUE){
  if (false == palette_item_name_is_translated(ITEM_NAME)) {
    return(strdup(ITEM_VALUE));
  }

  struct palette_code_value_translations_t *tmp = palette_code_value_translations;
  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0 && strcmp(ITEM_VALUE, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }

  return(strdup(ITEM_VALUE));
}


size_t random_palette_index(){
  srand(time(NULL) + getpid());
  return((size_t)(rand() % (PALETTES_QTY)));
}


char *get_palette_name_by_index(const int INDEX){
  return(palette_t_list[INDEX].name);
}

struct inc_palette_t *get_palette_t_by_index(const size_t INDEX){
  return(&(palette_t_list[INDEX]));
}

struct inc_palette_t *get_palette_t_by_name(const char *PALETTE_NAME){
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    if (strcmp(PALETTE_NAME, tmp->name) == 0) {
      return(tmp);
    }
  }
  return(NULL);
}


static bool is_valid_palette_item_name(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(true);
    }
  }

  return(false);
}


char *get_palette_item_code(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(tmp->code);
    }
  }

  return(NULL);
}


static char *get_palette_name_property_value(const char *PALETTE_NAME, const char *PALETTE_PROPERTY_NAME){
  struct inc_palette_t   *P                  = get_palette_t_by_name(PALETTE_NAME);
  struct Vector          *palette_properties = get_palette_name_properties_v(PALETTE_NAME);
  struct StringFNStrings lines               = stringfn_split_lines_and_trim((char *)P->data);

  for (size_t i = 0; i < vector_size(palette_properties); i++) {
    struct palette_property_t *pp = vector_get(palette_properties, i);
    if (strcmp(PALETTE_PROPERTY_NAME, pp->name) == 0) {
      char *s = strdup(pp->value);
      FREE_PALETTE_PROPERTIES(pp);
      return(s);
    }
  }
  return(NULL);
}


static char *get_palette_item_sequence(const struct palette_property_t *pp){
  struct StringBuffer *palette_codes_b = stringbuffer_new();

  stringbuffer_append_string(palette_codes_b, CODE_PREFIX);
  stringbuffer_append_string(palette_codes_b, pp->code);
  stringbuffer_append_string(palette_codes_b, pp->translated_value);
  stringbuffer_append_string(palette_codes_b, CODE_SUFFIX);
  char *palette_code = stringbuffer_to_string(palette_codes_b);

  stringbuffer_release(palette_codes_b);
  return(palette_code);
}


char *get_palette_name_sequence(const char *PALETTE_NAME){
  struct StringBuffer *sb = stringbuffer_new();
  struct Vector       *pp = get_palette_name_properties_v(PALETTE_NAME);

  for (size_t i = 0; i < vector_size(pp); i++) {
    struct palette_property_t *p = vector_get(pp, i);
    stringbuffer_append_string(sb, CODE_PREFIX);
    stringbuffer_append_string(sb, p->code);
    stringbuffer_append_string(sb, p->translated_value);
    stringbuffer_append_string(sb, CODE_SUFFIX);
    FREE_PALETTE_PROPERTIES(p);
  }
  stringbuffer_append_string(sb, CODES_SUFFIX);
  char *seq_s = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  return(seq_s);
}


bool load_palette_name(const char *PALETTE_NAME){
  char *seq = get_palette_name_sequence(PALETTE_NAME);
  int  qty  = fprintf(stdout, "%s", seq);

  fflush(stdout);
  return((qty > 0));
} /* load_palette_name */

