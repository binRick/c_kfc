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
#include "kfc-utils/kfc-utils.h"
#include "submodules/c_fsio/include/fsio.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
/////////////////////////////////////
extern struct inc_palette_t palette_t_list[];
extern const size_t         PALETTES_QTY;


int kfc0(void) {
  printf("kfc0............\n");
  return(0);
}
/////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////


bool palette_item_name_is_translated(const char *ITEM_NAME){
  struct palette_code_value_translations_t *tmp = palette_code_value_translations;

  for (size_t i = 0; i < 1000 && tmp->name != NULL; tmp++, i++) {
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
  for (size_t i = 0; i < 1000 && tmp->name != NULL; tmp++, i++) {
    if (strcmp(ITEM_NAME, tmp->name) == 0 && strcmp(ITEM_VALUE, tmp->src) == 0) {
      return(strdup(tmp->dst));
    }
  }

  return(strdup(ITEM_VALUE));
}


int random_palette_index(){
  srand(time(NULL) + getpid());
  return(rand() % (PALETTES_QTY));
}


char *get_palette_name_t_by_index(const int INDEX){
  return(strdup(palette_t_list[INDEX].name));
}
struct inc_palette_t *get_palette_t_by_name(const char *PALETTE_NAME){
  struct inc_palette_t *tmp = palette_t_list;

  for (size_t i = 0; i < 1000 && tmp->data != NULL; tmp++, i++) {
    if (strcmp(PALETTE_NAME, tmp->name) == 0) {
      return(tmp);
    }
  }
  return(NULL);
}


char *get_palette_item_code(const char *PALETTE_ITEM_NAME){
  struct palette_code_t *tmp = palette_codes;

  for (size_t i = 0; i < 1000 && tmp->name != NULL; tmp++, i++) {
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
    free(item_key);
    free(item_val);
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

  for (size_t i = 0; i < 1000000 && tmp->data != NULL; tmp++, i++) {
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
