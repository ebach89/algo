#include <stdio.h>

#define MAX_INT     2147483647
#define MIN_INT     (-MAX_INT - 1)

#define MAX(a, b)   (a) > (b) ? (a) : (b)
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(a[0]))

#define PRINT_FIND(expr, array)                             \
    do {                                                    \
        int i = (expr);                                     \
        printf(#expr" : array[%d]=%d\n", i, array[i].d);    \
    } while (0)

/* BST on array implementation is started here */
#define BST_MAX_EL  100

/* Binary Search Tree (BST) node */
typedef struct {
    int d;  /**< data */
    int li; /**< left index, pointing to the left child */
    int ri; /**< right index */
} bst_t;

/* BST on array data structure*/
typedef struct {
    int off;
    /**< offset in array at which position the last
     * 'bst_t' item has been added */
    bst_t t[BST_MAX_EL];
} tree_t;

/**
 * Creates new BST node
 * n - node
 * d - data
 */
static inline void bst(bst_t *n, int d)
{
    n->d = d;
    n->li = n->ri = -1;
}

/**
 * Inserts node 'n' (on 'n_idx' in array) as a left child of parent 'p'
 */
static inline void insert_l(bst_t *n, int n_idx, bst_t *p, int d)
{
    p->li = n_idx;
    bst(n, d);
}

/**
 * Inserts node 'n' (on 'n_idx' in array) as a right child of parent 'p'
 */
static inline void insert_r(bst_t *n, int n_idx, bst_t *p, int d)
{
    p->ri = n_idx;
    bst(n, d);
}

/**
 * Inserts value 'd' as a new node in the tree with the root 't'
 *
 * t - root of tree's array of bst_t items, i.e tree_t :: t[]
 * t_off - pointer to tree offset pointing to the last filled item in array
 */
void insert(bst_t *t, int *t_off, int d)
{
    int i = 0;
    int off;

    /* Auto increment tree offset, because we can insert new item only by
     * means of appending it into the end of the array */
    off = ++(*t_off);

    while (i <= off) {
        if (d < t[i].d) {
            if (t[i].li == -1) {
                insert_l(&t[off], off, &t[i], d);
                return;
            }
            i = t[i].li;
            continue;

        } else if (d > t[i].d) {
            if (t[i].ri == -1) {
                insert_r(&t[off], off, &t[i], d);
                return;
            }
            i = t[i].ri;
            continue;

        } else { /* d == t[i].d */
            /* Exclude duplicated items*/
            /* Currently, in this implementation by decreasing tree offset
             * we avoid hole in array */
            (*t_off)--;
            return;
        }
    }
}

/**
 * t_off - the last filled item in the array
 * v - value to be searched
 *
 * Returns index of item with searched value 'v' in the tree array,
 * otherwise -1
 */
int find(bst_t *t, int t_off, int v)
{
    int i = 0;

    while (i <= t_off) {
        if (v < t[i].d && t[i].li != -1) {
            i = t[i].li;
            continue;
        } else if (v > t[i].d && t[i].ri != -1) {
            i = t[i].ri;
            continue;
        } else if (v < t[i].d && t[i].li == -1) {
            /* did not find, reach the leaf */
            return -1;
        } else if (v > t[i].d && t[i].ri == -1) {
            /* did not find, reach the leaf */
            return -1;
        }

        if (v == t[i].d) {
            return i;
        }
    }
    return -1;
}

/**
 * The same but recursive version
 */
int find_r(bst_t *t, int i, int v)
{
    if (v < t[i].d && t[i].li != -1) {
        return find_r(t, t[i].li, v);
    } else if (v > t[i].d && t[i].ri != -1) {
        return find_r(t, t[i].ri, v);
    }

    if (v == t[i].d) {
        return i;
    }
    /* reach the leaf node */
    return -1;
}

void create_root(tree_t *tr, int d)
{
    tr->off = 0;
    bst(&tr->t[tr->off], d);
}

static int __is_bst(bst_t *t, int i, int min, int max)
{
    if (t[i].li == -1
     && t[i].ri == -1) {
        return 1;
    }

    /* false if violates BST property */
    if (t[i].d < min || t[i].d > max) {
        return 0;
    }

    /* check left/right sub-trees: allaw only distinct vales of nodes,
     * therefore subtract/add -1/+1 from/to min/max */
    int l = t[i].li == -1 ? 1 : __is_bst(t, t[i].li, min, t[i].d - 1);
    int r = t[i].ri == -1 ? 1 : __is_bst(t, t[i].ri, t[i].d + 1, max);
    return l && r;
}

int is_bst(bst_t *t)
{
    return __is_bst(t, 0, MIN_INT, MAX_INT);
}

int max_depth(bst_t *t, int i)
{
    /* get leaf node - leaf is on Level #0 */
    if (t[i].li == -1
     && t[i].ri == -1) {
        return 0;
    }

    int l = t[i].li == -1 ? 0 : max_depth(t, t[i].li);
    int r = t[i].ri == -1 ? 0 : max_depth(t, t[i].ri);

    return MAX(l, r) + 1;
}

/**
 * Returns index of node with min value in subtree,
 * denoted by inpit 'i' param.
 *
 * t - root of tree's array
 */
int min_in_sub_tree(bst_t *t, int i)
{
    int cur = i;

    while (t[cur].li != -1) {
        cur = t[cur].li;
    }
    return cur;
}

int max_in_sub_tree(bst_t *t, int i)
{
    int cur = i;

    while (t[cur].ri != -1) {
        cur = t[cur].ri;
    }
    return cur;
}

void tree_sort(bst_t *t, int i, int *sorted)
{
    static int cnt;

    if (t[i].li != -1) {
        tree_sort(t, t[i].li, sorted);
    }

    sorted[cnt++] = t[i].d;

    if (t[i].ri != -1) {
        tree_sort(t, t[i].ri, sorted);
    }
}

/**
 * in-order traversal
 */
void in_trav(bst_t *t, int i)
{
    static int once;
    int once_after = 0;

    if (!once) {
        printf("In order: ");
        once = once_after = 1;
    }

    if (t[i].li != -1) {
        in_trav(t, t[i].li);
    }

    printf("%d ", t[i].d);

    if (t[i].ri != -1) {
        in_trav(t, t[i].ri);
    }

    if (once_after) {
        printf("\n");
    }
}

/**
 * pre-order traversal
 */
void pre_trav(bst_t *t, int i)
{
    printf("%d\n", t[i].d);

    if (t[i].li != -1) {
        pre_trav(t, t[i].li);
    }

    if (t[i].ri != -1) {
        pre_trav(t, t[i].ri);
    }
}

/**
 * post-order traversal
 */
void post_trav(bst_t *t, int i)
{
    if (t[i].li != -1) {
        post_trav(t, t[i].li);
    }

    if (t[i].ri != -1) {
        post_trav(t, t[i].ri);
    }

    printf("%d\n", t[i].d);
}

void print_as_array(tree_t *tr)
{
    int i = 0;
    printf("Tree array: ");
    for (; i <= tr->off; ++i) {
        printf("%d ", tr->t[i].d);
    }
    printf("\n");
}

int main(void)
{
    tree_t tree;

    /*
     *               50
     *       ________|________
     *      /                 \
     *     10__               60__
     *         \                  \
     *         25                 92
     *        /  \               /
     *       15  30             67
     */
    create_root(&tree,        50);
    insert(tree.t, &tree.off, 10);
    insert(tree.t, &tree.off, 60);
    insert(tree.t, &tree.off, 25);
    insert(tree.t, &tree.off, 30);
    insert(tree.t, &tree.off, 92);
    insert(tree.t, &tree.off, 15);
    insert(tree.t, &tree.off, 67);
    /* check the case when adding the same item (in real BST
     * it should not be present in tree */
    insert(tree.t, &tree.off, 10);
    insert(tree.t, &tree.off, 25);
    insert(tree.t, &tree.off, 30);

    in_trav(tree.t, 0);
    print_as_array(&tree);
    int tree_height = max_depth(tree.t, 0);
    printf("Tree Height/Depth = %d\n", tree_height);

    printf("Is tree a BST: %s\n", is_bst(tree.t)
           ? "true"
           : "false");

    printf("\nSearching items:\n");
    PRINT_FIND(find(tree.t, tree.off, 92), tree.t);
    PRINT_FIND(find(tree.t, tree.off, 10), tree.t);
    PRINT_FIND(find(tree.t, tree.off, 67), tree.t);
    PRINT_FIND(find(tree.t, tree.off, 50), tree.t);
    PRINT_FIND(find(tree.t, tree.off, 5555), tree.t);
    PRINT_FIND(find(tree.t, tree.off, -55), tree.t);

    printf("\nSearching items recursively:\n");
    PRINT_FIND(find_r(tree.t, 0, 92), tree.t);
    PRINT_FIND(find_r(tree.t, 0, 10), tree.t);
    PRINT_FIND(find_r(tree.t, 0, 67), tree.t);
    PRINT_FIND(find_r(tree.t, 0, 50), tree.t);
    PRINT_FIND(find_r(tree.t, 0, 5555), tree.t);
    PRINT_FIND(find_r(tree.t, 0, -55), tree.t);

    printf("\nFind minimum/max in subtrees by choosing differrent "
           "root nodes\n");
    bst_t *root = tree.t;
    int sub_idx = root[0].ri;
    int min_idx = min_in_sub_tree(root, sub_idx);
    int max_idx = max_in_sub_tree(root, sub_idx);
    printf("Right sub-tree of root(%d), min:%d, max:%d\n",
           root[0].d, root[min_idx].d, root[max_idx].d);

    sub_idx = root[0].li;
    min_idx = min_in_sub_tree(root, sub_idx);
    max_idx = max_in_sub_tree(root, sub_idx);
    printf("Left sub-tree of root(%d), min:%d, max:%d\n",
           root[0].d, root[min_idx].d, root[max_idx].d);

    min_idx = min_in_sub_tree(root, 0);
    max_idx = max_in_sub_tree(root, 0);
    printf("Overall tree with root(%d), min:%d, max:%d\n",
           root[0].d, root[min_idx].d, root[max_idx].d);

    printf("\nTree Sort (just in order traverse)\n");
    int sorted[16] = {0};
    tree_sort(tree.t, 0, sorted);
    for (int i = 0; i < ARRAY_SIZE(sorted); ++i) {
        printf("%d ", sorted[i]);
    }
    printf("\n");
    return 0;
}
