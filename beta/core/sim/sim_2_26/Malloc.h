/*	This file is part of the memory management and leak detector MALLOC.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: Malloc.h,v 1.2 2008/04/21 14:35:43 dick Exp $
*/

#include	<stdio.h>

/*****
The files Malloc.[ch] provide several functionalities:

- checking for "out of memory":        to simplify programming
- allocating memory using new(type)    "     "        " "
- detecting memory leaks:              to obtain cleaner programs
- clobbering freshly allocated memory: to obtain safer programs

The module defines several sets of routines:

1.  void *Malloc(size_t s)
    void *Calloc(int n, size_t s)
    void *Realloc(void *p, size_t s)
    void *Free(void *p)

2.  void *TryMalloc(size_t s)
    void *TryCalloc(int n, size_t s)
    void *TryRealloc(void *p, size_t s)

3.  T *new(T)
    char *new_string(const char *s)

4.  void ReportMemoryLeaks(FILE *f)

* The members of the first set act like their Unix counterparts, except that
  they never return NULL; upon out-of-memory an error message is given on
  standard error, showing the file name and the line number of the call. Since
  in almost all cases there is nothing more intelligent to do, this is almost
  always adequate, and makes for simpler and safer programming.

  In those rare cases that the program *can* continue when out of memory, the
  routines in the second set can be used; they act exactly like their Unix
  counterparts.

  Note that automatic out-of-memory detection is active, regardless of the
  -DMEM... flags described below.

* A call of new(T), with T any type, yields a pointer of type T* to a block
  of type T, allocated using Malloc().
  A call of new_string(s), with s a string, yields a pointer to a copy of s,
  allocated using Malloc(); it is equivalent to strdup() except that it uses
  Malloc().

* Normally, a call of ReportMemoryLeaks() does nothing, but when Malloc.c is
  compiled with -DMEMLEAK, it produces a compacted list of allocated but not
  yet freed blocks on the stream f, with information about where they were
  allocated.
  This is useful to get insight into memory use and abuse.

* When Malloc.c is compiled with -DMEMCLOBBER, it clobbers all newly allocated
  memory from Malloc() and Realloc() just after allocation, and all freed
  memory just before freeing it.  Clobbering means overwriting it with the bit
  pattern 1010101001010101. This is done in the hope that improper use of
  memory will cause some evident error somewhere.

* Compiled with any of the -DMEM... flags, Malloc will also produce run-time
  error messages for multiple Free()s of the same block, and Realloc()s on
  not-allocated blocks. It then allows the program to continue.

* The system consumes hardly any time and is fast enough to be kept active all
  the time.
*****/

#define	Malloc(s)	(_leak_malloc(1, (size_t)(s), __FILE__, __LINE__))
#define	Calloc(n,s)	(_leak_calloc(1, (n), (size_t)(s), __FILE__, __LINE__))
#define	Realloc(p,s)	(_leak_realloc(1, (void *)(p), (size_t)(s), __FILE__, __LINE__))
#define	TryMalloc(s)	(_leak_malloc(0, (size_t)(s), __FILE__, __LINE__))
#define	TryCalloc(n,s)	(_leak_calloc(0, (n), (size_t)(s), __FILE__, __LINE__))
#define	TryRealloc(p,s)	(_leak_realloc(0, (void *)(p), (size_t)(s), __FILE__, __LINE__))
#define	Free(p)		(_leak_free((void *)(p), __FILE__, __LINE__))

#define	new(type)	((type *)Malloc(sizeof (type)))
#define	new_string(s)	(_new_string((s), __FILE__, __LINE__))

extern void *_leak_malloc(int chk, size_t size, const char *fname, int l_nmb);
extern void *_leak_calloc(int chk, int n, size_t size, const char *fname, int l_nmb);
extern void *_leak_realloc(int chk, void *addr, size_t size, const char *fname, int l_nmb);
extern void _leak_free(void *addr, const char *fname, int l_nmb);

extern void ReportMemoryLeaks(FILE *f);

extern char *_new_string(const char *s, const char *fname, int l_nmb);
