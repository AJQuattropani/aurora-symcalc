#include "representations.h"

static void print_node_recurse(gString *restrict gstr, f_node *restrict curr, int ntabs) {
  g_put_char(gstr, '\n', 1);
  g_put_char(gstr, ' ', ntabs);
  switch (curr->ty) {
  case BINARY:
    g_append_back(gstr, curr->name.cstring, curr->name.size);
    print_node_recurse(gstr, curr->bf.left, ntabs + 2);
    print_node_recurse(gstr, curr->bf.right, ntabs + 2);
    break;
  case UNARY:
    g_append_back(gstr, curr->name.cstring, curr->name.size);
    print_node_recurse(gstr, curr->uf.in, ntabs + 2);
    break;
  case CONSTANT: 
    g_append_back(gstr, curr->name.cstring, curr->name.size);
    break;
  case IDENTITY:
    g_append_back(gstr, curr->name.cstring, curr->name.size);
    break;
  }
}

void sprint_tree(gString * restrict gstr, f_object * restrict func) {
  print_node_recurse(gstr, func->root, 0); 
}
