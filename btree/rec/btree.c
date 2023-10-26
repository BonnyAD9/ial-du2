/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  // not found
  if (!tree) {
    return false;
  }

  // found
  if (tree->key == key) {
    *value = tree->value;
    return true;
  }

  // go left/right
  return tree->key > key
    ? bst_search(tree->left, key, value)
    : bst_search(tree->right, key, value);
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *t = *tree;

  // create new
  if (!t) {
    bst_node_t *n = malloc(sizeof(*n));
    if (!n) {
      return;
    }
    n->left = NULL;
    n->right  = NULL;
    n->value = value;
    n->key = key;
    *tree = n;
    return;
  }

  // edit
  if (t->key == key) {
    t->value = value;
    return;
  }

  // go left/right
  t->key > key
    ? bst_insert(&t->left, key, value)
    : bst_insert(&t->right, key, value);
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *t = *tree;
  // continue to right
  if (t->right) {
    bst_replace_by_rightmost(target, &t->right);
    return;
  }

  // edit target
  target->key = t->key;
  target->value = t->value;

  // when I already have the node that should be removed, remove it so that
  // I don't have to find it again
  bst_delete(tree, t->key);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *t = *tree;

  // key is no in the tree
  if (!t) {
    return;
  }

  // go left/right
  if (t->key > key) {
    bst_delete(&t->left, key);
    return;
  }
  if (t->key < key) {
    bst_delete(&t->right, key);
    return;
  }

  // node found, remove it

  // chain the right node
  if (!t->left) {
    *tree = t->right;
    free(t);
    return;
  }

  // chain the left node
  if (!t->right) {
    *tree = t->left;
    free(t);
    return;
  }

  // childern on both sides, replace self by the rightmost node in the left
  // subtree and remove it
  bst_replace_by_rightmost(t, &t->left);
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *t = *tree;

  // tree is empty
  if (!t) {
    return;
  }

  // remove the childern and free self
  bst_dispose(&t->left);
  bst_dispose(&t->right);
  free(t);
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  // nothing to add
  if (!tree) {
    return;
  }

  // add the nodes to the list in the correct order
  bst_add_node_to_items(tree, items);
  bst_preorder(tree->left, items);
  bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  // nothing to add
  if (!tree) {
    return;
  }

  // add the nodes to the list in the correct order
  bst_inorder(tree->left, items);
  bst_add_node_to_items(tree, items);
  bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  // nothing to add
  if (!tree) {
    return;
  }

  // add the nodes to the list in the correct order
  bst_postorder(tree->left, items);
  bst_postorder(tree->right, items);
  bst_add_node_to_items(tree, items);
}
