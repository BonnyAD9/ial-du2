#include "hashtable.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

#define INSERT_TEST_DATA(TABLE)                                                \
  ht_insert_many(TABLE, TEST_DATA, sizeof(TEST_DATA) / sizeof(TEST_DATA[0]));

const ht_item_t TEST_DATA[15] = {
    {"Bitcoin", 53247.71}, {"Ethereum", 3208.67}, {"Binance Coin", 409.15},
    {"Cardano", 1.82},     {"Tether", 0.86},      {"XRP", 0.93},
    {"Solana", 134.50},    {"Polkadot", 34.99},   {"Dogecoin", 0.22},
    {"USD Coin", 0.86},    {"Uniswap", 21.68},    {"Terra", 30.67},
    {"Litecoin", 156.87},  {"Avalanche", 47.03},  {"Chainlink", 21.90}};

void init_test() {
  printf("Hash Table - testing script\n");
  printf("---------------------------\n");
  HT_SIZE = 13;
  printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
  printf("\n");
}

TEST(test_table_init, "Initialize the table")
  ht_init(test_table);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
  ht_init(test_table);
  success &= !ht_search(test_table, "Ethereum");
ENDTEST

TEST(test_insert_simple, "Insert a new item")
  ht_init(test_table);
  ht_insert(test_table, "Ethereum", 3208.67);
  float *f = ht_get(test_table, "Ethereum");
  success &= f && *f == 3208.67f;
ENDTEST

TEST(test_search_exist, "Search for an existing item")
  ht_init(test_table);
  ht_insert(test_table, "Ethereum", 3208.67);
  ht_item_t *f = ht_search(test_table, "Ethereum");
  success &= f && f->value == 3208.67f;
ENDTEST

TEST(test_insert_many, "Insert many new items")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  size_t data_size = sizeof(TEST_DATA) / sizeof(*TEST_DATA);
  for (const ht_item_t *data = TEST_DATA; data_size; --data_size) {
    float *f = ht_get(test_table, data->key);
    success &= f && *f == data->value;
  }
ENDTEST

TEST(test_search_collision, "Search for an item with colliding hash")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  ht_item_t *f = ht_search(test_table, "Terra");
  success &= f && f->value == 30.67f;
ENDTEST

TEST(test_insert_update, "Update an item")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  ht_insert(test_table, "Ethereum", 12.34);
  float *f = ht_get(test_table, "Ethereum");
  success &= f && *f == 12.34f;
ENDTEST

TEST(test_get, "Get an item's value")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  float *f = ht_get(test_table, "Ethereum");
  success &= f && *f == 3208.67f;
ENDTEST

TEST(test_delete, "Delete an item")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  ht_delete(test_table, "Terra");
  success &= !ht_get(test_table, "Terra");
ENDTEST

TEST(test_delete_all, "Delete all the items")
  ht_init(test_table);
  INSERT_TEST_DATA(test_table)
  ht_delete_all(test_table);
  size_t data_size = sizeof(TEST_DATA) / sizeof(*TEST_DATA);
  for (const ht_item_t *data = TEST_DATA; data_size; --data_size) {
    success &= !ht_get(test_table, data->key);
  }
ENDTEST

int main(int argc, char *argv[]) {
  init_uninitialized_item();
  init_test();
  bool success = true;

  success &= test_table_init();
  success &= test_search_nonexist();
  success &= test_insert_simple();
  success &= test_search_exist();
  success &= test_insert_many();
  success &= test_search_collision();
  success &= test_insert_update();
  success &= test_get();
  success &= test_delete();
  success &= test_delete_all();

  if (success) {
    printf("\x1b[92mALL PASS\x1b[0m\n");
  } else {
    printf("\x1b[91mSOME FAIL\x1b[0m\n");
  }

  free(uninitialized_item);
}
