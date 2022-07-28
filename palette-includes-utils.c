#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "palette-includes.c"
#include "palette-includes.h"
#define DEBUG_PALETTES false


void __debug_palettes(void){
    struct inc_palette_t *tmp = palette_t_list;
    for(size_t i = 0; i < 1000 && tmp->data != NULL; tmp++, i++){
      printf ("__setup_palette: palette #%lu/%lu\n |name:%s|size:%d|file:%s|content:\n'%s'\n===================\n"
	  ,i+1,
	  PALETTES_QTY,
	  tmp->name,
	  tmp->size,
	  tmp->file,
	  tmp->data
	  );
      
    }

}
void __setup_palettes(void){
  if(DEBUG_PALETTES == true)
      __debug_palettes();
  fprintf(stderr,"Loaded %lu Included Palettes\n",PALETTES_QTY);
}
