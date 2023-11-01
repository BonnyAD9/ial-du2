/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  ht_item_t **tab = (ht_item_t **)table;

  for (int i = 0; i < HT_SIZE; ++i) {
    tab[i] = NULL;
  }
}

// Gets pointer to position for the key in the item
ht_item_t **ht_find(ht_table_t *table, char *key) {
  ht_item_t **tab = (ht_item_t **)table;
  ht_item_t **item = &tab[get_hash(key)];

  for (; *item; item = &(*item)->next) {
    if (strcmp(key, (*item)->key) == 0) {
      return item;
    }
  }

  return item;
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  return *ht_find(table, key);
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // I used ht_find instead of ht_search, because this way I need only one
  // lookup in the table.
  ht_item_t **i = ht_find(table, key);

  ht_item_t *item = *i;

  if (item) {
    item->value = value;
    return;
  }

  item = malloc(sizeof(*item));
  if (!item) {
    return;
  }

  item->key = key;
  item->value = value;
  item->next = NULL;

  *i = item;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *i = ht_search(table, key);
  return i ? &i->value : NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t **i = ht_find(table, key);
  ht_item_t *item = *i;

  if (item) {
    *i = item->next;
    free(item);
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t **tab = (ht_item_t **)table;

  for (int i = 0; i < HT_SIZE; ++i) {
    ht_item_t *item = tab[i];
    while (item) {
      ht_item_t *to_free = item;
      item = item->next;
      free(to_free);
    }
    tab[i] = NULL;
  }
}
