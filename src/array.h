#ifndef ARRAY_H_
# define ARRAY_H_

# include "smart_ptr.h"

typedef struct {
    size_t nmemb;
    size_t size;
} s_meta_array;

__attribute__ ((always_inline, pure))
inline size_t array_length(void *ptr) {
    s_meta_array *meta = get_smart_ptr_meta(ptr);
    return meta ? meta->nmemb : 0;
}

__attribute__ ((always_inline, pure))
inline size_t array_type_size(void *ptr) {
    s_meta_array *meta = get_smart_ptr_meta(ptr);
    return meta ? meta->size : 0;
}

__attribute__ ((always_inline, pure))
inline void *array_user_meta(void *ptr) {
    s_meta_array *meta = get_smart_ptr_meta(ptr);
    return meta ? meta + 1 : NULL;
}

#endif /* !ARRAY_H_ */
