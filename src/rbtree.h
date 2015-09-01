#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <stdint.h>
#include <stddef.h>  // for NULL

typedef uint64_t rbtree_key_t;

#define rbtree_red(_node)           ((_node)->color = 1)
#define rbtree_black(_node)         ((_node)->color = 0)
#define rbtree_is_red(_node)        ((_node)->color)
#define rbtree_is_black(_node)      (!rbtree_is_red(_node))
#define rbtree_copy_color(_n1, _n2) ((_n1)->color = (_n2)->color)

typedef struct rbtree_node_s rbtree_node_t;

struct rbtree_node_s {
    rbtree_node_t *left;     /* left link */
    rbtree_node_t *right;    /* right link */
    rbtree_node_t *parent;   /* parent link */
    rbtree_key_t   key;      /* key for ordering */
    unsigned char  color;    /* red | black */
};

typedef struct rbtree_s {
    rbtree_node_t *root;     /* root node */
    rbtree_node_t *sentinel; /* nil node */
} rbtree_t;

#define rbtree_empty(tree)           \
    (tree)->root == (tree)->sentinel

/* color is left uninitialized */
#define rbtree_node_init(_node)      \
do {                                 \
    (_node)->left   = NULL;          \
    (_node)->right  = NULL;          \
    (_node)->parent = NULL;          \
    (_node)->key    = 0ULL;          \
} while(0)

void rbtree_init(rbtree_t *tree, rbtree_node_t *sentinel);
rbtree_node_t *rbtree_min(rbtree_t *tree);
rbtree_node_t *rbtree_max(rbtree_t *tree);
void rbtree_insert(rbtree_t *tree, rbtree_node_t *node);
void rbtree_delete(rbtree_t *tree, rbtree_node_t *node);

#define rbtree_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif
