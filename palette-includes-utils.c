#pragma once
/////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////
#define DEBUG_PALETTES         false
#define DEBUG_PALETTE_CODES    false
/////////////////////////////////////////////////////////////////////
#include "palette-includes.c"
#include "palette-includes.h"
/////////////////////////////////////////////////////////////////////
#include "submodules/c_fsio/include/fsio.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
/////////////////////////////////////////////////////////////////////
static struct palette_code_t palette_codes[] = {
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
  { .name = "cursor",               .code = "]12;#",   },
  { .name = "selection_background", .code = "]17;#",   },
  { .name = "selection_foreground", .code = "]19;#",   },
  { 0 },
};
/////////////////////////////////////////////////////////////////////
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
  struct StringBuffer *palette_codes_b = stringbuffer_new();
  struct inc_palette_t *P = get_palette_t_by_name(PALETTE_NAME);
  struct StringFNStrings lines = stringfn_split_lines_and_trim((char *)P->data);
  struct StringFNStrings items;
  char *line;

  for (int i = 0; i < lines.count; i++) {
    line  = lines.strings[i];
    items = stringfn_split(line, '=');
    if (items.count != 2) {
      continue;
    }
    if (strlen(items.strings[0]) == 0) {
      continue;
    }
    char *code = get_palette_item_code(items.strings[0]);
    if (code == NULL || strlen(code) == 0) {
      continue;
    }
    stringbuffer_append_string(palette_codes_b, CODE_PREFIX);
    stringbuffer_append_string(palette_codes_b, code);
    stringbuffer_append_string(palette_codes_b, items.strings[1]);
    stringbuffer_append_string(palette_codes_b, CODE_SUFFIX);
  }
  stringbuffer_append_string(palette_codes_b, CODES_SUFFIX);
  char *palette_codes = stringbuffer_to_string(palette_codes_b);

  stringbuffer_release(palette_codes_b);
  //printf(">%lub code\n", strlen(palette_codes));
  //assert(true == fsio_write_text_file("/tmp/codes.txt", palette_codes));
  printf("%s", palette_codes);
  //fprintf(stdout, "%s", palette_codes);
  return(true);
}

void __debug_palette_codes(void){
  printf("__debug_palette_codes\n");
//  load_palette_name("vscode");
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
  fprintf(stderr, "Loaded %lu Included Palettes\n", PALETTES_QTY);
}
