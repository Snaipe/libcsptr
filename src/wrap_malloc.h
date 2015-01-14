#ifndef WRAP_MALLOC_H_
# define WRAP_MALLOC_H_

#ifdef SMALLOC_WRAP_MALLOC
extern void *(*real_malloc)(size_t);
extern void (*real_free)(void *);
#endif /* !SMALLOC_WRAP_MALLOC */

#endif /* !WRAP_MALLOC_H_ */
