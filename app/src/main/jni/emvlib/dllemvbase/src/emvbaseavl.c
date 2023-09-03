/* Produced by texiweb from libavl.w. */

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

#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "dllemvbase.h"
#include "emvbaseavl.h"


static EMVBaseallocator gEMVBaseallocator;


void emvbase_allocator_init(EMVBaseallocator *allocator)
{
	gEMVBaseallocator.EMVBallocator_malloc = allocator->EMVBallocator_malloc;
	gEMVBaseallocator.EMVBallocator_free   = allocator->EMVBallocator_free;
}


void *emvbase_malloc (unsigned int size)
{

	if(size > 0)
	{
		return gEMVBaseallocator.EMVBallocator_malloc(size);
	}
	else
	{
		return NULL;
	}

}

void emvbase_free (void *block)
{

	if(block != NULL)
	{
		gEMVBaseallocator.EMVBallocator_free(block);
		block = NULL;
	}

}


void *emvbase_realloc(void *block, unsigned int size)
{
	return NULL;
}



/* Creates and returns a new table
   with comparison function |compare| using parameter |param|
   and memory allocator |allocator|.
   Returns |NULL| if memory allocation failed. */
struct emvbase_avl_table *emvbase_avl_create (emvbase_avl_comparison_func *compare, void *param, struct emvbase_libavl_allocator *allocator)
{
	struct emvbase_avl_table *tree;

	if (allocator == NULL)
	{
		EMVBase_Trace("allocator is NULL!\r\n");
		allocator = &emvbase_allocator_default;
	}
	tree = allocator->libavl_malloc(allocator, sizeof *tree);

	if (tree == NULL)
	{
	  return NULL;
	}

	tree->avl_root = NULL;
	tree->avl_compare = compare;
	tree->avl_param = param;
	tree->avl_alloc = allocator;
	tree->avl_count = 0;
	tree->avl_generation = 0;

	return tree;
}

/* Search |tree| for an item matching |item|, and return it if found.
   Otherwise return |NULL|. */
void *
emvbase_avl_find (const struct emvbase_avl_table *tree, const void *item)
{
  const struct emvbase_avl_node *p;

  for (p = tree->avl_root; p != NULL; )
    {
      int cmp = tree->avl_compare (item, p->avl_data, tree->avl_param);

      if (cmp < 0)
        p = p->avl_link[0];
      else if (cmp > 0)
        p = p->avl_link[1];
      else /* |cmp == 0| */
        return p->avl_data;
    }

  return NULL;
}

/* Inserts |item| into |tree| and returns a pointer to |item|'s address.
   If a duplicate item is found in the tree,
   returns a pointer to the duplicate without inserting |item|.
   Returns |NULL| in case of memory allocation failure. */
void **
emvbase_avl_probe (struct emvbase_avl_table *tree, void *item)
{
  struct emvbase_avl_node *y, *z; /* Top node to update balance factor, and parent. */
  struct emvbase_avl_node *p, *q; /* Iterator, and parent. */
  struct emvbase_avl_node *n;     /* Newly inserted node. */
  struct emvbase_avl_node *w;     /* New root of rebalanced subtree. */
  int dir;                /* Direction to descend. */

  unsigned char da[EMVBASEAVL_MAX_HEIGHT]; /* Cached comparison results. */
  int k = 0;              /* Number of cached results. */


  z = (struct emvbase_avl_node *) &tree->avl_root;
  y = tree->avl_root;
  dir = 0;
  for (q = z, p = y; p != NULL; q = p, p = p->avl_link[dir])
    {
      int cmp = tree->avl_compare (item, p->avl_data, tree->avl_param);
      if (cmp == 0)
        return &p->avl_data;

      if (p->avl_balance != 0)
        z = q, y = p, k = 0;
      da[k++] = dir = cmp > 0;
    }

  n = q->avl_link[dir] =
    tree->avl_alloc->libavl_malloc (tree->avl_alloc, sizeof *n);
  if (n == NULL)
    return NULL;

  tree->avl_count++;
  n->avl_data = item;
  n->avl_link[0] = n->avl_link[1] = NULL;
  n->avl_balance = 0;
  if (y == NULL)
    return &n->avl_data;

  for (p = y, k = 0; p != n; p = p->avl_link[da[k]], k++)
    if (da[k] == 0)
      p->avl_balance--;
    else
      p->avl_balance++;

  if (y->avl_balance == -2)
    {
      struct emvbase_avl_node *x = y->avl_link[0];
      if (x->avl_balance == -1)
        {
          w = x;
          y->avl_link[0] = x->avl_link[1];
          x->avl_link[1] = y;
          x->avl_balance = y->avl_balance = 0;
        }
      else
        {
          w = x->avl_link[1];
          x->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = x;
          y->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = y;
          if (w->avl_balance == -1)
            x->avl_balance = 0, y->avl_balance = +1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == +1| */
            x->avl_balance = -1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else if (y->avl_balance == +2)
    {
      struct emvbase_avl_node *x = y->avl_link[1];
      if (x->avl_balance == +1)
        {
          w = x;
          y->avl_link[1] = x->avl_link[0];
          x->avl_link[0] = y;
          x->avl_balance = y->avl_balance = 0;
        }
      else
        {
          w = x->avl_link[0];
          x->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = x;
          y->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = y;
          if (w->avl_balance == +1)
            x->avl_balance = 0, y->avl_balance = -1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == -1| */
            x->avl_balance = +1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else
    return &n->avl_data;
  z->avl_link[y != z->avl_link[0]] = w;

  tree->avl_generation++;
  return &n->avl_data;
}

/* Inserts |item| into |table|.
   Returns |NULL| if |item| was successfully inserted
   or if a memory allocation error occurred.
   Otherwise, returns the duplicate item. */
void *
emvbase_avl_insert (struct emvbase_avl_table *table, void *item)
{
  void **p = emvbase_avl_probe (table, item);
  return p == NULL || *p == item ? NULL : *p;
}

/* Inserts |item| into |table|, replacing any duplicate item.
   Returns |NULL| if |item| was inserted without replacing a duplicate,
   or if a memory allocation error occurred.
   Otherwise, returns the item that was replaced. */
void *
emvbase_avl_replace (struct emvbase_avl_table *table, void *item)
{
  void **p = emvbase_avl_probe (table, item);
  if (p == NULL || *p == item)
    return NULL;
  else
    {
      void *r = *p;
      *p = item;
      return r;
    }
}

/* Deletes from |tree| and returns an item matching |item|.
   Returns a null pointer if no matching item found. */
void *
emvbase_avl_delete (struct emvbase_avl_table *tree, const void *item)
{
  /* Stack of nodes. */
  struct emvbase_avl_node *pa[EMVBASEAVL_MAX_HEIGHT]; /* Nodes. */
  unsigned char da[EMVBASEAVL_MAX_HEIGHT];    /* |avl_link[]| indexes. */
  int k;                               /* Stack pointer. */

  struct emvbase_avl_node *p;   /* Traverses tree to find node to delete. */
  int cmp;              /* Result of comparison between |item| and |p|. */


  k = 0;
  p = (struct emvbase_avl_node *) &tree->avl_root;
  for (cmp = -1; cmp != 0;
       cmp = tree->avl_compare (item, p->avl_data, tree->avl_param))
    {
      int dir = cmp > 0;

      pa[k] = p;
      da[k++] = dir;

      p = p->avl_link[dir];
      if (p == NULL)
        return NULL;
    }
  item = p->avl_data;

  if (p->avl_link[1] == NULL)
    pa[k - 1]->avl_link[da[k - 1]] = p->avl_link[0];
  else
    {
      struct emvbase_avl_node *r = p->avl_link[1];
      if (r->avl_link[0] == NULL)
        {
          r->avl_link[0] = p->avl_link[0];
          r->avl_balance = p->avl_balance;
          pa[k - 1]->avl_link[da[k - 1]] = r;
          da[k] = 1;
          pa[k++] = r;
        }
      else
        {
          struct emvbase_avl_node *s;
          int j = k++;

          for (;;)
            {
              da[k] = 0;
              pa[k++] = r;
              s = r->avl_link[0];
              if (s->avl_link[0] == NULL)
                break;

              r = s;
            }

          s->avl_link[0] = p->avl_link[0];
          r->avl_link[0] = s->avl_link[1];
          s->avl_link[1] = p->avl_link[1];
          s->avl_balance = p->avl_balance;

          pa[j - 1]->avl_link[da[j - 1]] = s;
          da[j] = 1;
          pa[j] = s;
        }
    }

  tree->avl_alloc->libavl_free (tree->avl_alloc, p);

  while (--k > 0)
    {
      struct emvbase_avl_node *y = pa[k];

      if (da[k] == 0)
        {
          y->avl_balance++;
          if (y->avl_balance == +1)
            break;
          else if (y->avl_balance == +2)
            {
              struct emvbase_avl_node *x = y->avl_link[1];
              if (x->avl_balance == -1)
                {
                  struct emvbase_avl_node *w;
                  w = x->avl_link[0];
                  x->avl_link[0] = w->avl_link[1];
                  w->avl_link[1] = x;
                  y->avl_link[1] = w->avl_link[0];
                  w->avl_link[0] = y;
                  if (w->avl_balance == +1)
                    x->avl_balance = 0, y->avl_balance = -1;
                  else if (w->avl_balance == 0)
                    x->avl_balance = y->avl_balance = 0;
                  else /* |w->avl_balance == -1| */
                    x->avl_balance = +1, y->avl_balance = 0;
                  w->avl_balance = 0;
                  pa[k - 1]->avl_link[da[k - 1]] = w;
                }
              else
                {
                  y->avl_link[1] = x->avl_link[0];
                  x->avl_link[0] = y;
                  pa[k - 1]->avl_link[da[k - 1]] = x;
                  if (x->avl_balance == 0)
                    {
                      x->avl_balance = -1;
                      y->avl_balance = +1;
                      break;
                    }
                  else
                    x->avl_balance = y->avl_balance = 0;
                }
            }
        }
      else
        {
          y->avl_balance--;
          if (y->avl_balance == -1)
            break;
          else if (y->avl_balance == -2)
            {
              struct emvbase_avl_node *x = y->avl_link[0];
              if (x->avl_balance == +1)
                {
                  struct emvbase_avl_node *w;
                  w = x->avl_link[1];
                  x->avl_link[1] = w->avl_link[0];
                  w->avl_link[0] = x;
                  y->avl_link[0] = w->avl_link[1];
                  w->avl_link[1] = y;
                  if (w->avl_balance == -1)
                    x->avl_balance = 0, y->avl_balance = +1;
                  else if (w->avl_balance == 0)
                    x->avl_balance = y->avl_balance = 0;
                  else /* |w->avl_balance == +1| */
                    x->avl_balance = -1, y->avl_balance = 0;
                  w->avl_balance = 0;
                  pa[k - 1]->avl_link[da[k - 1]] = w;
                }
              else
                {
                  y->avl_link[0] = x->avl_link[1];
                  x->avl_link[1] = y;
                  pa[k - 1]->avl_link[da[k - 1]] = x;
                  if (x->avl_balance == 0)
                    {
                      x->avl_balance = +1;
                      y->avl_balance = -1;
                      break;
                    }
                  else
                    x->avl_balance = y->avl_balance = 0;
                }
            }
        }
    }

  tree->avl_count--;
  tree->avl_generation++;
  return (void *) item;
}


/* Frees storage allocated for |tree|.
   If |destroy != NULL|, applies it to each data item in inorder. */
void
emvbase_avl_destroy (struct emvbase_avl_table *tree, emvbase_avl_item_func *destroy)
{
  struct emvbase_avl_node *p, *q;

  for (p = tree->avl_root; p != NULL; p = q)
    if (p->avl_link[0] == NULL)
      {
        q = p->avl_link[1];
        if (destroy != NULL && p->avl_data != NULL)
          destroy (p->avl_data, tree->avl_param);
        tree->avl_alloc->libavl_free (tree->avl_alloc, p);
      }
    else
      {
        q = p->avl_link[0];
        p->avl_link[0] = q->avl_link[1];
        q->avl_link[1] = p;
      }

  tree->avl_alloc->libavl_free (tree->avl_alloc, tree);
}

/* Allocates |size| bytes of space using |malloc()|.
   Returns a null pointer if allocation fails. */
void *emvbase_avl_malloc (struct emvbase_libavl_allocator *allocator, unsigned int size)
{
  void *pmalloc;

  pmalloc =  gEMVBaseallocator.EMVBallocator_malloc(size);

  if(pmalloc == NULL)
  {
		while(1)
		{

		}
  }
  return pmalloc;
}

/* Frees |block|. */
void
emvbase_avl_free (struct emvbase_libavl_allocator *allocator, void *block)
{
  gEMVBaseallocator.EMVBallocator_free(block);
}

/* Default memory allocator that uses |malloc()| and |free()|. */
struct emvbase_libavl_allocator emvbase_allocator_default =
{
	emvbase_avl_malloc,
	emvbase_avl_free
};


