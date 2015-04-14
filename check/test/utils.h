#ifndef UTILS_H_
# define UTILS_H_

#include <criterion/criterion.h>
#include <inttypes.h>

__attribute__((always_inline))
inline int is_aligned(void *ptr) {
    uintptr_t off = (uintptr_t) ptr;
    return !(off % sizeof (void *));
}

__attribute__((always_inline))
inline void assert_valid_ptr(void *ptr) {
    assert_neq(ptr, NULL, "Expected unique_ptr to return a non-null pointer.");
    assert(is_aligned(ptr), "Expected unique_ptr to return an aligned pointer.");
}

struct meta {
    int i;
    long l;
    double d;
};

extern const struct meta m;

__attribute__((always_inline))
inline void assert_valid_meta(void *ptr, const struct meta *m, const struct meta *m2) {
    assert_neq(m2, NULL, "Expected metadata to be present");
    assert_neq(m, m2, "Expected metadata to be copied");
    const int intact = m->i == m2->i
        && m->l == m2->l
        && m->d == m2->d;
    assert(intact, "Expected metadata to be intact.");
}

#define lambda(RType, Body) ({ RType __fn__ Body; __fn__; })

#endif /* !UTILS_H_ */
