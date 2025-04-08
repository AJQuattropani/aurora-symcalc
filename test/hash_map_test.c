#include "../src/objects/hash_map.h"
#include "../src/env/operators.h"
#include "../src/objects/vector.h"

void test_default_map(Map *map) {
  printf("Building map.\n");
  cinsert(map, "+", (_value){.bOperation = vb_add, .ty = BOPER});
  cinsert(map, "-", (_value){ .bOperation = vb_sub, .ty = BOPER});
  cinsert(map, "*", (_value){ .bOperation = vb_mul, .ty = BOPER});
  cinsert(map, "/", (_value){ .bOperation = vb_div, .ty = BOPER});
  cinsert(map, "^", (_value){ .bOperation = vb_pow, .ty = BOPER});
  cinsert(map, "_", (_value){ .bOperation = vb_log, .ty = BOPER});
  cinsert(map, "\\sin", (_value){.uOperation = vu_sin, .ty = UOPER});
  cinsert(map, "\\cos", (_value){.uOperation = vu_cos, .ty = UOPER});
  cinsert(map, "\\tan", (_value){.uOperation = vu_tan, .ty = UOPER});
  cinsert(map, "\\sec", (_value){.uOperation = vu_sec, .ty = UOPER});
  cinsert(map, "\\csc", (_value){.uOperation = vu_csc, .ty = UOPER});
  cinsert(map, "\\cot", (_value){.uOperation = vu_cot, .ty = UOPER});
  cinsert(map, "\\log", (_value){.uOperation = vu_log, .ty = UOPER});
  cinsert(map, "\\arcsin", (_value){.uOperation = vu_asin, .ty = UOPER});
  cinsert(map, "\\arccos", (_value){.uOperation = vu_acos, .ty = UOPER});
  cinsert(map, "\\arctan", (_value){.uOperation = vu_atan, .ty = UOPER});
}

int main(void) {
  Map map;
  test_default_map(&map);
  
  printf("C1\n");
  
  mString add = m_from_cstr("+"); 
  mString mult = {"*", 1};

  print_map(&map);

  vd_literal l = alloc_vdliteral(10);
  vd_literal r = alloc_vdliteral(10);
  vd_literal o;
  vd_literal o2;
  printf("C2\n");
  
  for (size_t i = 0; i < l.size; i++) {
    l.data[i] = i;
    r.data[i] = 9-i;
  }
  
  const Object *obj = lookup_value_const(&map, add);
  const Object *obj2 = lookup_value_const(&map, mult);
  if (obj == NULL || obj2 == NULL) {
    printf("SEARCH FAILED.\n");
    goto cleanup;
  }
  if (obj->ty == BOPER) {
    printf("C3\n");
    int i = obj->bOperation(&o, &l, &r);
  }
  if (obj2->ty == BOPER) {
    printf("C4\n");
    int i = obj2->bOperation(&o2, &l, &r);
  }

  for (size_t i = 0; i < l.size; i++) {
    printf("%lf = %lf + %lf\n", o.data[i], l.data[i], r.data[i]);
    printf("%lf = %lf * %lf\n", o2.data[i], l.data[i], r.data[i]);
  }
cleanup:
  empty_map(&map);
  free_vdliteral(&l);
  free_vdliteral(&r);
  free_vdliteral(&o);
  free_vdliteral(&o2);
  m_deletestr(&add);
}
