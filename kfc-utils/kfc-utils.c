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
#define DEBUG_PALETTES         false
#define DEBUG_PALETTE_CODES    false
/////////////////////////////////////
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
#include "submodules/c_fsio/include/fsio.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
#include "submodules/c_vector/include/vector.h"
#include "submodules/meson_deps/submodules/bytes/bytes.h"
#include "submodules/meson_deps/submodules/c_vector/include/vector.h"
#include "submodules/meson_deps/submodules/debug-memory/debug_memory.h"
#include "submodules/meson_deps/submodules/libfort/src/fort.h"
/////////////////////////////////////
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
  { 0 },
};
static struct palette_code_t                    palette_codes[] = {
  { .name = "color00",              .code = "]4;0;#",  },
  { .name = "color01",              .code = "]4;1;#",  },
  { .name = "color02",              .code = "]4;2;#",  },
  { .name = "color03",              .code = "]4;3;#",  },
  { .name = "color04",              .code = "]4;4;#",  },
  { .name = "color05",              .code = "]4;5;#",  },
  { .name = "color06",              .code = "]4;6;#",  },
  { .name = "color07",              .code = "]4;7;#",  },
  { .name = "color08",              .code = "]4;8;#",  },
  { .name = "color09",              .code = "]4;9;#",  },
  { .name = "color10",              .code = "]4;10;#", },
  { .name = "color11",              .code = "]4;11;#", },
  { .name = "color12",              .code = "]4;12;#", },
  { .name = "color13",              .code = "]4;13;#", },
  { .name = "color14",              .code = "]4;14;#", },
  { .name = "color15",              .code = "]4;15;#", },
  { .name = "foreground",           .code = "]10;#",   },
  { .name = "background",           .code = "]11;#",   },
  { .name = "selection_background", .code = "]17;#",   },
  { .name = "selection_foreground", .code = "]19;#",   },
  { .name = "title",                .code = "]2;",     },
  { .name = "cursor",               .code = "]12;#",   },
  { .name = "cursorvisible",        .code = "[?25",    },
  { .name = "cursorstyle",          .code = "[",       },
  { .name = "cursorblink",          .code = "[?12",    },
  { .name = "altscreen",            .code = "[?47",    },
  { .name = "screen",               .code = "[?5",     },
  { .name = "icontitle",            .code = "]0;",     },
  { .name = "icon",                 .code = "]1;",     },
  { .name = "title",                .code = "]2;",     },
  { .name = "reportfocus",          .code = "[?1004",  },
  { .name = "mouse",                .code = "[?100",   },
  { 0 },
};

#define TABLE_LIMIT    5


/////////////////////////////////////////////////////////////////////
char *get_palette_properties_table(const char *PALETTE_NAME){
  ft_table_t *table = ft_create_table();

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
              "Code",
              "Translated"
              );
  struct palette_property_t *palette_properties_v = get_palette_name_properties_v(PALETTE_NAME);

  for (size_t i = 0; i < vector_size(palette_properties_v); i++) {
    char                      *translated_value;
    struct palette_property_t *pp = (struct palette_property_t *)vector_get(palette_properties_v, i);
    if (palette_item_name_is_translated(pp->name)) {
      translated_value = translate_palette_item_value(pp->name, pp->value);
    }else{
      translated_value = strdup("");
    }
    ft_printf_ln(table,
                 "%s|%s|%s|%s",
                 pp->name,
                 pp->value,
                 get_palette_item_code(pp->name),
                 translated_value
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    if (translated_value) {
      free(translated_value);
    }
    free(pp->name);
    free(pp->value);
    free(pp);
  }

  vector_release(palette_properties_v);

  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  return(table_s);
} /* get_palette_properties_table */


char *get_palettes_table() {
  ft_table_t *table = ft_create_table();

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
              "Name",
              "Size"
              "File"
              "Properties"
              );

  struct Vector *palettes_v = require(kfc_utils)->palettes_v;

  for (size_t i = 0; i < vector_size(palettes_v) && i < TABLE_LIMIT; i++) {
    struct inc_palette_t *p  = (struct inc_palette_t *)vector_get(palettes_v, i);
    struct Vector        *pp = get_palette_name_properties_v(p->name);
    ft_printf_ln(table,
                 "%s|%d|%s|%lu|",
                 p->name,
                 p->size,
                 p->file,
                 vector_size(pp)
                 );

    ft_set_cell_prop(table, i + 1, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_CYAN);
    ft_set_cell_prop(table, i + 1, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);
    for (size_t i = 0; i < vector_size(pp); i++) {
      struct palette_property_t *_pp = vector_get(pp, i);
      free(_pp->name);
      free(_pp->value);
      free(_pp);
    }

    vector_release(pp);
  }

  char *table_s = ft_to_string(table);

  ft_destroy_table(table);
  return(table_s);
} /* list_windows */
/////////////////////////////////////////////////////////////////////
struct Vector *get_palette_name_properties_v(const char *PALETTE_NAME){
  struct Vector        *v = vector_new();
  struct inc_palette_t *p = get_palette_t_by_name(PALETTE_NAME);

  if (p == NULL) {
    return(v);
  }
  struct StringFNStrings lines = stringfn_split_lines_and_trim(p->data);

  for (size_t i = 0; i < lines.count; i++) {
    struct StringFNStrings items = stringfn_split(lines.strings[i], '=');
    if (items.count != 2) {
      continue;
    }
    if (strlen(items.strings[0]) == 0) {
      continue;
    }
    struct palette_property_t *pp = malloc(sizeof(struct palette_property_t));
    pp->name  = strdup(items.strings[0]);
    pp->value = strdup(items.strings[1]);
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

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    vector_push(v, (void *)tmp);
  }
  return(v);
}


int kfc_utils_module_test(void) {
  module(kfc_utils) * kfc_utils = require(kfc_utils);

  kfc_utils->mode = LOGGER_DEBUG;
  kfc_utils->info("hello");
  kfc_utils->error("oops");
  kfc_utils->debug("help");
  printf("palettes vector qty:%lu\n", vector_size(kfc_utils->palettes_v));
  printf("palette names vector qty:%lu\n", vector_size(kfc_utils->palette_names_v));
  printf("palettes data bytes:%s\n", bytes_to_string(kfc_utils->palettes_data_bytes));

  printf("palettes table:\n%s\n", kfc_utils->get_palettes_table());
  for (size_t i = 0; i < 2; i++) {
    struct inc_palette_t *p = kfc_utils->get_palette_t_by_index(i);
    printf("palette #%lu> %s\n%s\n", i, p->name, get_palette_properties_table(p->name));
  }

  printf("palette #%s\n%s\n", "vscode", kfc_utils->get_palette_properties_table("vscode"));

  size_t i     = kfc_utils->random_palette_index();
  char   *name = kfc_utils->get_palette_name_by_index(i);
  printf("random palette index:%lu> |%s|%s|%s|\n",
         i,
         name,
         (kfc_utils->get_palette_t_by_index(i))->name,
         kfc_utils->get_palette_t_by_name(name)->name
         );
  free(name);

  clib_module_free(kfc_utils);
  return(0);
}


/////////////////////////////////////////////////////////////////////


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
  return(strdup(palette_t_list[INDEX].name));
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


char *get_palette_item_code(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < PALETTES_QTY && tmp->name != NULL; tmp++, i++) {
    if (strcmp(tmp->name, PALETTE_ITEM_NAME) == 0) {
      return(tmp->code);
    }
  }

  return(NULL);
}


bool load_palette_name(const char *PALETTE_NAME){
  struct StringBuffer    *palette_codes_b = stringbuffer_new();
  struct inc_palette_t   *P               = get_palette_t_by_name(PALETTE_NAME);
  struct StringFNStrings lines            = stringfn_split_lines_and_trim((char *)P->data);
  char                   *OVERRIDE_PROP   = getenv("PROP");
  char                   *OVERRIDE_VAL    = getenv("VAL");

  if (OVERRIDE_PROP != NULL) {
    fprintf(stderr, "Restricting to property %s\n", OVERRIDE_PROP);
  }

  for (int i = 0; i < lines.count; i++) {
    struct StringFNStrings items = stringfn_split(lines.strings[i], '=');
    if (items.count != 2) {
      continue;
    }
    if (strlen(items.strings[0]) == 0) {
      continue;
    }
    if (OVERRIDE_PROP != NULL && strcmp(OVERRIDE_PROP, items.strings[0]) != 0) {
      continue;
    }
    char *code = get_palette_item_code(items.strings[0]);
    if (code == NULL || strlen(code) == 0) {
      continue;
    }
    char *item_key = strdup(items.strings[0]);
    char *item_val = strdup(items.strings[1]);
    char *translated_value;
    if (OVERRIDE_VAL != NULL) {
      fprintf(stderr, "Overriding %s | %s -> %s\n", item_key, item_val, OVERRIDE_VAL);
      item_val = strdup(OVERRIDE_VAL);
    }
    if (true == palette_item_name_is_translated(item_key)) {
      translated_value = translate_palette_item_value(item_key, item_val);
    }else{
      translated_value = strdup(item_val);
    }
    stringbuffer_append_string(palette_codes_b, CODE_PREFIX);
    stringbuffer_append_string(palette_codes_b, code);
    stringbuffer_append_string(palette_codes_b, translated_value);
    stringbuffer_append_string(palette_codes_b, CODE_SUFFIX);
    stringfn_release_strings_struct(items);
    if (item_key) {
      free(item_key);
    }
    if (item_val) {
      free(item_val);
    }
    if (translated_value) {
      free(translated_value);
    }
  }
  stringfn_release_strings_struct(lines);
  stringbuffer_append_string(palette_codes_b, CODES_SUFFIX);
  char *palette_codes = stringbuffer_to_string(palette_codes_b);

  stringbuffer_release(palette_codes_b);
  int qty = fprintf(stdout, "%s", palette_codes);

  fflush(stdout);

  return((qty > 0));
} /* load_palette_name */


void __debug_palette_codes(void){
  printf("__debug_palette_codes\n");
}


void __debug_palettes(void){
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
    printf("__setup_palette: palette #%lu/%lu\n |name:%s|size:%d|file:%s|content:\n'%s'\n===================\n",
           i + 1,
           PALETTES_QTY,
           tmp->name,
           tmp->size,
           tmp->file,
           tmp->data
           );
  }
}


void __setup_palettes(void){
  if (DEBUG_PALETTES == true) {
    __debug_palettes();
  }
  if (DEBUG_PALETTE_CODES == true) {
    __debug_palette_codes();
  }
  if (DEBUG_PALETTES) {
    fprintf(stderr, "Loaded %lu Included Palettes\n", PALETTES_QTY);
  }
}

