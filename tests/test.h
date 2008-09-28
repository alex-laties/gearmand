/* Gearman server and library
 * Copyright (C) 2008 Brian Aker
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GEARMAN_TEST_H__
#define __GEARMAN_TEST_H__

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct world_st world_st;
typedef struct collection_st collection_st;
typedef struct test_st test_st;

typedef enum {
  TEST_SUCCESS,
  TEST_FAILURE,
  TEST_MEMORY_ALLOCATION_FAILURE,
  TEST_MAXIMUM_RETURN /* Always add new error code before */
} test_return;

struct test_st {
  char *name;
  uint8_t requires_flush;
  test_return (*function)(void *object);
};

struct collection_st {
  char *name;
  test_return (*flush)(void);
  void *(*create)(void *collection_object);
  void (*destroy)(void *object);
  test_return (*pre)(void *object);
  test_return (*post)(void *object);
  test_st *tests;
};

struct world_st {
  collection_st *collections;
  void *(*create)(void);
  void (*destroy)(void *collection_object);
};

/* How we make all of this work :) */
void get_world(world_st *world);

#ifdef TEST_INTERNAL
#define WATCHPOINT fprintf(stderr, "\nWATCHPOINT %s:%d (%s)\n", __FILE__, __LINE__,__func__);fflush(stdout);
#define WATCHPOINT_STRING(A) fprintf(stderr, "\nWATCHPOINT %s:%d (%s) %s\n", __FILE__, __LINE__,__func__,A);fflush(stdout);
#define WATCHPOINT_STRING_LENGTH(A,B) fprintf(stderr, "\nWATCHPOINT %s:%d (%s) %.*s\n", __FILE__, __LINE__,__func__,(int)B,A);fflush(stdout);
#define WATCHPOINT_NUMBER(A) fprintf(stderr, "\nWATCHPOINT %s:%d (%s) %zu\n", __FILE__, __LINE__,__func__,(size_t)(A));fflush(stdout);
#define WATCHPOINT_ERRNO(A) fprintf(stderr, "\nWATCHPOINT %s:%d (%s) %s\n", __FILE__, __LINE__,__func__, strerror(A));fflush(stdout);
#define WATCHPOINT_ASSERT(A) assert((A));
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GEARMAN_TEST_H__ */
