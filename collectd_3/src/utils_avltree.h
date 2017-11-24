#ifndef UTILS_AVLTREE_H
#define UTILS_AVLTREE_H 1

#ifdef __cplusplus
extern "C" {
#endif

	struct c_avl_tree_s;
	typedef struct c_avl_tree_s c_avl_tree_t;

	struct c_avl_iterator_s;
	typedef struct c_avl_iterator_s c_avl_iterator_t;

	c_avl_tree_t *c_avl_create(int (*compare)(const void *, const void *));

	void c_avl_destroy(c_avl_tree_t *t);

	int c_avl_insert(c_avl_tree_t *t, void *key, void *value);

	int c_avl_remove(c_avl_tree_t *t, const void *key, void **rkey, void **rvalue);

	int c_avl_get(c_avl_tree_t *t, const void *key, void **value);

	int c_avl_pick(c_avl_tree_t *t, void **key, void **value);

	c_avl_iterator_t *c_avl_get_iterator(c_avl_tree_t *t);
	int c_avl_iterator_next(c_avl_iterator_t *iter, void **key, void **value);
	int c_avl_iterator_prev(c_avl_iterator_t *iter, void **key, void **value);
	void c_avl_iterator_destroy(c_avl_iterator_t *iter);
	void test(c_avl_tree_t *r);
#ifdef __cplusplus
}
#endif

#endif /* UTILS_AVLTREE_H */
