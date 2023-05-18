
/* libavl - library for manipulation of binary trees.
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2004 Free Software
   Foundation, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.
*/

#ifndef _EMVBASEAVL_H
#define _EMVBASEAVL_H

#include <stddef.h>


/* Function types. */
typedef int emvbase_avl_comparison_func (const void *avl_a, const void *avl_b,void *avl_param);
typedef void emvbase_avl_item_func (void *avl_item, void *avl_param);
typedef void *emvbase_avl_copy_func (void *avl_item, void *avl_param);

/* Memory allocator. */
struct emvbase_libavl_allocator
  {
    void *(*libavl_malloc) (struct emvbase_libavl_allocator *, unsigned int libavl_size);
    void (*libavl_free) (struct emvbase_libavl_allocator *, void *libavl_block);
  };

/* Default memory allocator. */
extern struct emvbase_libavl_allocator emvbase_allocator_default;
void *emvbase_avl_malloc (struct emvbase_libavl_allocator *, unsigned int);
void emvbase_avl_free (struct emvbase_libavl_allocator *, void *);

/* Maximum AVL tree height. */
#ifndef EMVBASEAVL_MAX_HEIGHT
#define EMVBASEAVL_MAX_HEIGHT 92
#endif

/* Tree data structure. */
struct emvbase_avl_table
  {
    struct emvbase_avl_node *avl_root;          /* Tree's root. */
    emvbase_avl_comparison_func *avl_compare;   /* Comparison function. */
    void *avl_param;                    /* Extra argument to |avl_compare|. */
    struct emvbase_libavl_allocator *avl_alloc; /* Memory allocator. */
    unsigned int avl_count;                   /* Number of items in tree. */
    unsigned long avl_generation;       /* Generation number. */
  };

/* An AVL tree node. */
struct emvbase_avl_node
  {
    struct emvbase_avl_node *avl_link[2];  /* Subtrees. */
    void *avl_data;                /* Pointer to data. */
    signed char avl_balance;       /* Balance factor. */
  };

/* AVL traverser structure. */
struct emvbase_avl_traverser
  {
    struct emvbase_avl_table *emvbase_avl_table;        /* Tree being traversed. */
    struct emvbase_avl_node *emvbase_avl_node;          /* Current node in tree. */
    struct emvbase_avl_node *avl_stack[EMVBASEAVL_MAX_HEIGHT];
                                        /* All the nodes above |avl_node|. */
    unsigned int avl_height;                  /* Number of nodes in |avl_parent|. */
    unsigned long avl_generation;       /* Generation number. */
  };

/* Table functions. */
struct emvbase_avl_table *emvbase_avl_create (emvbase_avl_comparison_func *, void *,
                              struct emvbase_libavl_allocator *);
struct emvbase_avl_table *emvbase_avl_copy (const struct emvbase_avl_table *, emvbase_avl_copy_func *,
                            emvbase_avl_item_func *, struct emvbase_libavl_allocator *);
void emvbase_avl_destroy (struct emvbase_avl_table *, emvbase_avl_item_func *);
void **emvbase_avl_probe (struct emvbase_avl_table *, void *);
void *emvbase_avl_insert(struct emvbase_avl_table *, void *);
void *emvbase_avl_replace (struct emvbase_avl_table *, void *);
void *emvbase_avl_delete (struct emvbase_avl_table *, const void *);
void *emvbase_avl_find (const struct emvbase_avl_table *, const void *);






#endif /* avl.h */

