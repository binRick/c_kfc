#pragma once
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <libproc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

typedef struct inc_color_t inc_color_t;
struct inc_color_t { char *name; char *hex; };
const size_t INC_COLORS_QTY;
