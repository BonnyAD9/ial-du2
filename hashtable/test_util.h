#ifndef IAL_HASHTABLE_TEST_UTIL_H
#define IAL_HASHTABLE_TEST_UTIL_H

#include "hashtable.h"

#define TEST(NAME, DESCRIPTION)                                                \
  bool NAME() {                                                                \
    printf("[%s] %s\n", #NAME, DESCRIPTION);                                   \
    ht_table_t *test_table;                                                    \
    init_test_table(&test_table);                                              \
    bool success = true;

#define ENDTEST                                                                \
  printf("\n");                                                                \
  ht_print_table(test_table);                                                  \
  ht_delete_all(test_table);                                                   \
  free(test_table);                                                            \
  printf("\n");                                                                \
  if (!success) printf("\x1b[91mFAILED\x1b[0m\n");                             \
  return success;                                                              \
  }

extern ht_item_t *uninitialized_item;

void ht_print_item_value(float *value);
void ht_print_item(ht_item_t *item);
void ht_print_table(ht_table_t *table);
void ht_insert_many(ht_table_t *table, const ht_item_t items[], int count);

void init_uninitialized_item();
void init_test_table(ht_table_t **table);

#endif
