#pragma once
#include "vt100utils/demos/tuibox.h"
#include "vt100utils/vt100utils.h"

#define MIN(a, b)    (a < b ? a : b)

ui_t u;
struct vt100_node_t *head;


void draw(ui_box_t *b, char *out) {
  struct vt100_node_t *node = b->data1;
  char                *sgr  = vt100_sgr(node, NULL);

  sprintf(out, "%s%s", sgr, node->str);
  free(sgr);
}


void click(ui_box_t *b, int x, int y) {
  struct vt100_node_t *node = b->data1;

  node->fg.value += 10;
  if (node->fg.value > 255) {
    node->fg.value = 10;
  }
  ui_draw(&u);
}


void stop() {
  ui_free(&u);
  vt100_free(head);
  exit(0);
}

