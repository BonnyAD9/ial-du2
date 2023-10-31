/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu)
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v
 * uložen ve stromu.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    for (; *input; ++input) {
        int cnt = 0;
        bst_search(*tree, *input, &cnt);
        bst_insert(tree, *input, cnt + 1);
    }
}

// Inserts `nodes` into `tree` in such order, that if the tree is empty, it
// will be balanced
void _bst_balance(bst_node_t **tree, bst_node_t **nodes, size_t len) {
    // nothing to balance, tree is empty
    if (len == 0) {
        *tree = NULL;
        return;
    }

    // insert the middle node
    size_t p = len / 2;
    *tree = nodes[p];

    // inserts the nodes for the left and right subtrees
    _bst_balance(&(*tree)->left, nodes, p);
    _bst_balance(&(*tree)->right, nodes + p + 1, len - p - 1);
}

/**
 * Vyvážení stromu.
 *
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 *
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // init
    bst_items_t items = { 0 };

    // walk trough the tree and than insert the items in the correct order
    bst_inorder(*tree, &items);
    _bst_balance(tree, items.nodes, items.size);

    free(items.nodes);
}
