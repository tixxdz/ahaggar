/*
 * The 'Ahaggar framework'
 *
 * Copyright (C) 2012-2013  Djalal Harouni <tixxdz@opendz.org>
 * Copyright (C) 2012-2013  LIRE Laboratory.
 * University Constantine 2, Algeria.
 *
 *   Author(s): Djalal Harouni <tixxdz@opendz.org>
 *              Pr. Mohamed Benmohammed
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Some parts were taken from GCC tree-dump.h and tree-dump.c files.
 *
 */

#ifndef _CACHE_NODES_H
#define _CACHE_NODES_H

#include "splay-tree.h"
#include "pointer-set.h"

struct sp_tree_queue {
	splay_tree_node node;
	struct sp_tree_queue *next;
};

struct sp_tree_nodes {
	/* nodes index */
	unsigned int index;

	const_tree node;

	struct sp_tree_queue *queue;
	struct sp_tree_queue *queue_end;

	/* struct splay_tree_queue *free_list;*/

	/* The tree nodes, keys are the addresses of the nodes. */
	splay_tree nodes;
};

struct pmap_queue {
	/* the queued node */
	const void *node;
	struct pmap_queue *next;
};

struct pmap_nodes {
	/* nodes index */
	unsigned int index;

	struct pmap_queue *queue;
	struct pmap_queue *queue_end;

	/*struct pointer_map_queue *free_list;*/

	/* The pointer map */
	struct pointer_map_t *pmap;

	/* Optional field that can be used to track
	 * multiple pointer_map_nodes... */
	/*struct pointer_map_nodes *next;*/
};

struct sp_tree_nodes *sptree_nodes_init(void);
void sptree_nodes_finish(struct sp_tree_nodes *sp);

static inline void sptree_nodes_set_root(struct sp_tree_nodes *sp,
					 const_tree t)
{
	sp->node = t;
}

static inline bool sptree_nodes_is_root(struct sp_tree_nodes *sp,
					const_tree t)
{
	return sp->node == t;
}

static inline unsigned int sptree_nodes_next_index(struct sp_tree_nodes *sp)
{
	return ++sp->index;
}

static inline unsigned int sptree_nodes_index(struct sp_tree_nodes *sp)
{
	return sp->index;
}

unsigned int sptree_nodes_insert(struct sp_tree_nodes *sp,
				 const_tree t);
void *sptree_nodes_contains(struct sp_tree_nodes *sp, const_tree t);
unsigned int sptree_nodes_index_by_key(struct sp_tree_nodes *sp,
				       const_tree t);


struct pmap_nodes *pmap_nodes_init(void);
void pmap_nodes_finish(struct pmap_nodes *pn);

static inline unsigned int pmap_nodes_next_index(struct pmap_nodes *pn)
{
	return ++pn->index;
}

static inline unsigned int pmap_nodes_index(struct pmap_nodes *pn)
{
	return pn->index;
}

unsigned int pmap_nodes_insert(struct pmap_nodes *pn, const_tree t);
void **pmap_nodes_contains(struct pmap_nodes *pn, const_tree t);

unsigned int pmap_nodes_index_by_key(struct pmap_nodes *pn,
				     const_tree t);

tree pmap_nodes_next_node(struct pmap_nodes *pn,
			  struct pmap_queue **idx);

#define PMAP_QUEUE_FIRST(pn) \
	((struct pmap_queue *)((pn)->queue))
#define PMAP_QUEUE_END(pn) \
	((struct pmap_queue *)((pn)->queue_end))

#define PMAP_NODES_FIRST_NODE(pn) \
	((tree)(((struct pmap_nodes *)(pn))->queue->node))
#define PMAP_NODES_LAST_NODE(pn) \
	((tree)(((struct pmap_nodes *)(pn))->queue_end->node))
#define PMAP_NODES_NEXT_NODE(pn, q) pmap_nodes_next_node(pn, q)

#endif /* _TREE_NODES_H */
