/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  // find relevant node
  while (tree && tree->key != key) {
    tree = tree->key > key
      ? tree->left
      : tree->right;
  }

  // set value if the node exists
  return tree && (*value = tree->value, true);
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // find relevant node
  while (*tree && (*tree)->key != key) {
    tree = (*tree)->key > key
      ? &(*tree)->left
      : &(*tree)->right;
  }

  bst_node_t *t = *tree;

  // found node to edit
  if (t) {
    t->value = value;
    return;
  }

  // found place for new node
  bst_node_t *n = malloc(sizeof(*n));
  if (!n) {
    return;
  }
  n->left = NULL;
  n->right = NULL;
  n->key = key;
  n->value = value;
  *tree = n;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // find rightmost node
  while ((*tree)->right) {
    tree = &(*tree)->right;
  }

  bst_node_t *t = *tree;

  target->key = t->key;
  target->value = t->value;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  // find relevant node
  while (*tree && (*tree)->key != key) {
    tree = (*tree)->key > key
      ? &(*tree)->left
      : &(*tree)->right;
  }

  bst_node_t *t = *tree;

  // key is no in the tree
  if (!t) {
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
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree);

  while (!stack_bst_empty(&stack)) {
    bst_node_t *n = stack_bst_pop(&stack);
    if (!n) {
      continue;
    }
    stack_bst_push(&stack, n->left);
    stack_bst_push(&stack, n->right);
    free(n);
  }

  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  // I don't understand why would I need this, and so I don't quite understand
  // what it is supposed to do.
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, tree);

  while (!stack_bst_empty(&stack)) {
    bst_node_t *n = stack_bst_pop(&stack);
    if (!n) {
      continue;
    }

    // push the left node last, so that it is popped first
    stack_bst_push(&stack, n->right);
    stack_bst_push(&stack, n->left);

    bst_add_node_to_items(n, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  for (; tree; tree = tree->left) {
    stack_bst_push(to_visit, tree);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  // init
  stack_bst_t nodes;
  stack_bst_init(&nodes);

  // push the left arm
  bst_leftmost_inorder(tree, &nodes);

  while (!stack_bst_empty(&nodes)) {
    tree = stack_bst_pop(&nodes);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &nodes);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  for (; tree; tree = tree->left) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  // init
  stack_bst_t nodes;
  stack_bool_t first_visit;
  stack_bst_init(&nodes);
  stack_bool_init(&first_visit);

  // add the first left arm to the stack
  bst_leftmost_postorder(tree, &nodes, &first_visit);

  while (!stack_bst_empty(&nodes)) {
    tree = stack_bst_pop(&nodes);

    // if this is the second visit, add it to the list
    if (!stack_bool_pop(&first_visit)) {
      bst_add_node_to_items(tree, items);
      continue;
    }

    // if this is the first visit, push it back and push the left arm of the
    // right node
    stack_bool_push(&first_visit, false);
    stack_bst_push(&nodes, tree);
    bst_leftmost_postorder(tree->right, &nodes, &first_visit);
  }
}
