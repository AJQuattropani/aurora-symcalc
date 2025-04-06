#include "../src/objects/hash_map.h"

int main(void) {

  printf("C1\n");
  Map map;

  mString pigs = from_cstr("pigs");
  mString cows = from_cstr("cows");
  mString geese = from_cstr("geese");
  mString ducks = from_cstr("ducks");
  mString dogs = from_cstr("dogs");
  insert(&map, pigs, 2);
  insert(&map, cows, 10);
  insert(&map, geese, 4);
  insert(&map, ducks, 25);
  
  print_map(&map);
  
  empty_map(&map);
  
  insert(&map, pigs, 2);
  insert(&map, dogs, 4);
  insert(&map, geese, 42);
  
  _value* pig_count = lookup_value(&map, pigs);
  printf("Pig Count: %d\n", *pig_count);
  *pig_count = 54;
  const _value* duck_count = lookup_value_const(&map, ducks);
  if (duck_count != NULL) exit(1);

  print_map(&map);
  printf("\n");
  
  _value* cow_count = acquire_value(&map, cows);
  *cow_count = 54;
  
  print_map(&map);
  printf("\n");

  delete_pair(&map, cows);
  printf("Deleted pair.\n");

  print_map(&map);
  printf("\n");

  delete_pair(&map, cows);
  delete_pair(&map, geese);
  
  print_map(&map);
  printf("\n");

  empty_map(&map);
  delete_mstr(&pigs);
  delete_mstr(&cows);
  delete_mstr(&geese);
  delete_mstr(&ducks);
  delete_mstr(&dogs);

}
