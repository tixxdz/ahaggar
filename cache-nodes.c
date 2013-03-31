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


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "splay-tree.h"
#include "pointer-set.h"

#include "cache-nodes.h"

struct sp_tree_nodes *sptree_nodes_init(void)
{
	struct sp_tree_nodes *sp;

	sp = (struct sp_tree_nodes *)xmalloc(sizeof(struct sp_tree_nodes));
	memset(sp, 0, sizeof(struct sp_tree_nodes));
	sp->nodes = splay_tree_new(splay_tree_compare_pointers, 0,
				   (splay_tree_delete_value_fn)&free);
	return sp;
}

void sptree_nodes_finish(struct sp_tree_nodes *sp)
{
	struct sp_tree_queue *q;
	struct sp_tree_queue *q_next;

	for (q = sp->queue; q; q = q_next) {
		q_next = q->next;
		free(q);
	}

	splay_tree_delete(sp->nodes);
	free(sp);
	sp = NULL;
}

static unsigned int __sptree_nodes_insert(struct sp_tree_nodes *sp,
					  const_tree t)
{
	unsigned int index;
	struct sp_tree_queue *q;

	q = (struct sp_tree_queue *)xmalloc(sizeof(struct sp_tree_queue));
	memset(q, 0, sizeof(struct sp_tree_queue));
	index = sptree_nodes_next_index(sp);

	q->node = splay_tree_insert(sp->nodes,
				    (splay_tree_key)t,
				    (splay_tree_value)index);

	q->next = NULL;
	if (!sp->queue_end)
		sp->queue = q;
	else
		sp->queue_end->next = q;

	sp->queue_end = q;
	return index;
}

unsigned int sptree_nodes_insert(struct sp_tree_nodes *sp,
				 const_tree t)
{
	splay_tree_node slot;
	unsigned int index = 0;

	if (!t)
		return index;

	slot = splay_tree_lookup(sp->nodes, (splay_tree_key)t);
	if (slot)
		index = (unsigned int)slot->value;
	else
		index = __sptree_nodes_insert(sp, t);

	return index;
}

void *sptree_nodes_contains(struct sp_tree_nodes *sp, const_tree t)
{
	 return splay_tree_lookup(sp->nodes, (splay_tree_key)t);
}

unsigned int sptree_nodes_index_by_key(struct sp_tree_nodes *sp,
				       const_tree t)
{
	splay_tree_node slot;
	unsigned int index = 0;

	if (!t)
		return index;

	slot = splay_tree_lookup(sp->nodes, (splay_tree_key)t);
	if (slot)
		index = (unsigned int)slot->value;

	return index;
}

struct pmap_nodes *pmap_nodes_init(void)
{
	struct pmap_nodes *pn;

	pn = (struct pmap_nodes *)xmalloc(sizeof(struct pmap_nodes));
	memset(pn, 0, sizeof(struct pmap_nodes));
	pn->pmap = pointer_map_create();
	return pn;
}

void pmap_nodes_finish(struct pmap_nodes *pn)
{
	struct pmap_queue *q;
	struct pmap_queue *q_next;

	for (q = pn->queue; q; q = q_next) {
		q_next = q->next;
		free(q);
	}

	pointer_map_destroy(pn->pmap);
	free(pn);
	pn = NULL;
}

static unsigned int __pmap_nodes_insert(struct pmap_nodes *pn,
					const_tree t)
{
	unsigned int index;
	struct pmap_queue *q;

	q = (struct pmap_queue *)xmalloc(sizeof(struct pmap_queue));
	memset(q, 0, sizeof(struct pmap_queue));
	index = pmap_nodes_next_index(pn);

	q->node = t;
	*pointer_map_insert(pn->pmap, t) = (void *)(intptr_t)index;

	/* Add it to the end of the queue */
	q->next = NULL;
	if (!pn->queue_end)
		pn->queue = q;
	else
		pn->queue_end->next = q;

	pn->queue_end = q;
	return index;
}

unsigned int pmap_nodes_insert(struct pmap_nodes *pn, const_tree t)
{
	void **slot;
	unsigned int index = 0;

	if (!t)
		return index;

	slot = pointer_map_contains(pn->pmap, t);
	if (slot)
		index = (unsigned int)(intptr_t)*slot;
	else
		index = __pmap_nodes_insert(pn, t);

	return index;
}

void **pmap_nodes_contains(struct pmap_nodes *pn, const_tree t)
{
	return pointer_map_contains(pn->pmap, t);
}

unsigned int pmap_nodes_index_by_key(struct pmap_nodes *pn,
				     const_tree t)
{
	void **slot;
	unsigned int index = 0;

	if (!t)
		return index;

	slot = pointer_map_contains(pn->pmap, t);

	if (slot)
		index = (unsigned int)(intptr_t)*slot;

	return index;
}

/*
 * Iterate over nodes and return NULL_TREE if at the end.
 *
 * If idx points to NULL then we set it to the first entry in the queue,
 * otherwise we set it to the next entry and in this case idx must point to
 * the right queue entry, use PMAP_QUEUE_FIRST() to intialize it.
 */
tree pmap_nodes_next_node(struct pmap_nodes *pn,
			  struct pmap_queue **idx)
{
	tree t = NULL_TREE;

	/* set to next or to the start of the queue if it's NULL */
	*idx = (*idx) ? (*idx)->next : pn->queue;

	if (*idx)
		t = (tree)(*idx)->node;

	return t;
}
