#define DEBUG_MODE    false
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
#include "kfc-cli/kfc-cli.h"
#include "kfc-utils/kfc-utils.h"
extern struct inc_palette_t palette_t_list[];
////////////////////////////////////////////
#define CONFIG_PATH_ENV_VAR    "XDG_CONFIG_HOME"
#define KFC_VERSION            "0.1.2"
#define DEBUG_CONFIG_DIR       false
#define _POSIX_C_SOURCE        200809L
#define PALETTES_DIR           "/usr/local/share/kfc/palettes"
////////////////////////////////////////////
static char *seq;                        /* path to palette source directory  */
static char *conf;                       /* path to configuration file */
static char *mode          = "dark";     /* selected mode string */
static char *sval          = NULL;       /* selected palette string */
static char *embedded_sval = NULL;       /* selected embedded palette string */


////////////////////////////////////////////
static void usage(void){
  fprintf(stderr, "\
usage: kfc [-L] [-r|-s palette] [-l|-p|-v]\n \
-L          Set light themes (modifier for -s/-r)\n \
-r          Select a random palette (dark theme by default)\n \
-s palette  Select a palette (dark theme by default)\n \
-S palette  Select an embedded palette\n \
-P property Select an embedded palette property (cursorcolor, title, etc)\n \
-l          List all palettes (dark themes by default)\n \
-p          Print current palette\n \
-e          List Embedded Palettes\n \
-v          Show version information\n");
  exit(1);
}


static void find_palettes(void) {
  if (access(PALETTES_DIR, F_OK) == 0) {
    seq = PALETTES_DIR;
  }else if (access("palettes", F_OK) == 0) {
    seq = "palettes";
  }else {
    fprintf(stderr, "Palette source directory not found\n");
    exit(1);
  }
}


static void list_palette(char *sel) {
  struct dirent **de;
  int           n = scandir(sel, &de, NULL, alphasort);

  while (n--) {
    if (de[n]->d_type == 8) {
      printf("%s/%s\n", mode, de[n]->d_name);
    }
  }

  free(de);
}


static void random_palette(char *sel) {
  struct dirent **de;
  int           n = scandir(sel, &de, NULL, alphasort);

  srand(time(0));
  int i = (rand() % (n - 0 + 1)) + 0;

  while (n--) {
    if (i == n) {
      sval = de[n]->d_name;
    }
  }

  free(de);
}


int main(int argc, char **argv) {
  int    cval;
  int    rflag = 0, lflag = 0, pflag = 0;
  size_t len;
  char   *sel, *tmp;


  if (argc == 1) {
    fprintf(stderr, "No argument(s) provided\n");
    usage();
    return(1);
  }

  tmp = getenv(CONFIG_PATH_ENV_VAR);

  if (tmp != NULL) {
    len  = strlen(tmp) + sizeof("/kfc/current");
    conf = malloc(sizeof(char) * len);
    snprintf(conf, len, "%s", tmp);
  }else {
    if (DEBUG_CONFIG_DIR) {
      puts("XDG_CONFIG_HOME is not defined. Defaulting to HOME/.config");
    }
    tmp  = getenv("HOME");
    len  = strlen(tmp) + sizeof("/.config/kfc/current");
    conf = malloc(sizeof(char) * len);
    snprintf(conf, len, "%s/.config", tmp);
  }

  strcat(conf, "/kfc");

  if (mkdir(conf, 0777) == 0) {
    puts("Created 'kfc' directory in XDG_CONFIG_HOME");
  }

  strcat(conf, "/current");
  find_palettes();

  int ri;

  while ((cval = getopt(argc, argv, "RerlLpvsS:")) != -1) {
    switch (cval) {
    case 'v': puts("kfc " KFC_VERSION);  break;
    case 'L': mode = "light";     break;
    case 'l': lflag++;            break;
    case 'r': rflag++;            break;
    case 'p': pflag++;            break;
    case 'e':
      fprintf(stderr, "Listing embedded\n");
      struct inc_palette_t *tmp = palette_t_list;
      for (size_t i = 0; i < 1000 && tmp->data != NULL; tmp++, i++) {
        printf("%s\n", tmp->name);
      }
      break;
    case 'R':
      ri = random_palette_index();
      char *n = get_palette_name_t_by_index(ri);
      embedded_sval = n;
      break;
    case 'S':
      if (rflag) {
        fprintf(stderr, "Cannot specify -r with -s\n");
        return(1);
      }
      embedded_sval = optarg;
      break;
    case 's':
      if (rflag) {
        fprintf(stderr, "Cannot specify -r with -s\n");
        return(1);
      }
      sval = optarg;
      break;
    case 'h':
      usage();
      return(0);

      break;
    case '?': return(1);
    } /* switch */
  }

  if (embedded_sval != NULL) {
    struct inc_palette_t *P = get_palette_t_by_name(embedded_sval);
    if (P == NULL) {
      printf("Embedded palette %s not found\n", embedded_sval);
      exit(1);
    }
    printf("Loading embedded palette %s of %db\n", P->name, P->size);
    load_palette_name(P->name);
    //fprintf(stdout, "%s", PALETTE);
    exit(0);
  }

  return(0);
} /* main */
