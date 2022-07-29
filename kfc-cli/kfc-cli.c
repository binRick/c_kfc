#define DEBUG_MODE     false
#define KFC_VERSION    "0.1.2"
#define DEBUG_MEMORY
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
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils-module.h"
#include "kfc-utils/kfc-utils.h"
////////////////////////////////////////////
struct ctx_t {
  char *embedded_sval;
  bool debug_mode;
  int  random_palette_index;
  char *random_palette_name;
  module(kfc_utils) * kfc_utils;
} ctx = {
  .embedded_sval        = NULL,
  .debug_mode           = NULL,
  .random_palette_index = 0,
  .kfc_utils            = NULL,
};


void __attribute__((constructor)) premain(){
  ctx.kfc_utils = require(kfc_utils);
}

void __attribute__((destructor)) postmain(){
  clib_module_free(ctx.kfc_utils);
#ifdef DEBUG_MEMORY
  printf("\nChecking for memory leak\n");
  print_allocated_memory();
#endif
}


////////////////////////////////////////////
static void usage(void){
  fprintf(stderr, "\
usage: kfc [-d] [-V|-h|-l|-r|-p palette] [-P property] [-V value]\n \
-d          Enable Debug Mode\n \
-l          List all palettes\n \
-r          Load Random Palette\n \
-p palette  Load Specified Palette\n \
-P property Restrict Palette Property (cursorcolor, title, etc)\n \
-V value    Set Palette Value if Palette Property Specified\n\
-v          Show version information\n");
  exit(1);
}


int main(int argc, char **argv) {
  int    cval;
  size_t len;
  char   *sel, *tmp;


  if (argc == 1) {
    fprintf(stderr, "No argument(s) provided\n");
    usage();
    return(1);
  }

  while ((cval = getopt(argc, argv, "rlrhvdp:")) != -1) {
    switch (cval) {
    case 'v': puts("kfc " KFC_VERSION);  break;
    case 'd':
      ctx.debug_mode = true;
      break;
    case 'l':
      fprintf(stderr, "Listing embedded\n");
      struct inc_palette_t *tmp = ctx.kfc_utils->palettes;
      for (size_t i = 0; i < PALETTES_QTY && tmp->data != NULL; tmp++, i++) {
        printf("%s\n", tmp->name);
      }
      return(0);

      break;
    case 'r':
      ctx.random_palette_index = random_palette_index();
      ctx.embedded_sval        = get_palette_name_by_index(ctx.random_palette_index);
      break;
    case 'p':
      ctx.embedded_sval = strdup(optarg);
      break;
    case 'h':
      usage();
      return(0);

      break;
    case '?': return(1);
    } /* switch */
  }

  if (ctx.embedded_sval != NULL) {
    struct inc_palette_t *P = get_palette_t_by_name(ctx.embedded_sval);
    if (P == NULL) {
      printf("Embedded palette %s not found\n", ctx.embedded_sval);
      exit(1);
    }
    if (ctx.debug_mode) {
      fprintf(stderr, "Loading embedded palette %s of %db\n", P->name, P->size);
    }
    load_palette_name(P->name);
    if (ctx.debug_mode) {
      fprintf(stderr, "%s", PALETTE);
    }
    free(ctx.embedded_sval);
    return(0);
  }else{
    usage();
    return(1);
  }
  return(0);
} /* main */
