#ifndef HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_0_1
#define HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_0_1
/**
 * @copyright Copyright (c) 2025 Steffen Illhardt,
 *            Licensed under the MIT license
 *            ( https://opensource.org/license/mit/ ).
 * @brief     cstring - Heap implemented using C library allocation functions.
 * @file      cstring.h
 * @version   0.1
 * @details
 *   The c-string library is based on the c-vector library,
 *   Copyright (c) 2015 Evan Teran,
 *   refer to: https://github.com/eteran/c-vector
 */

/* ======================== */
/* === PUBLIC INTERFACE === */
/* ======================== */

/* ------------- */
/* --- types --- */

/**
 * @brief cstring_string_type - The string type used in this library.
 * @param type - The type of string to act on.
 */
#define cstring_string_type(type) type *

/**
 * @brief cstring - Syntactic sugar to retrieve a string type.
 * @param type - The type of string to act on.
 */
#define cstring(type) cstring_string_type(type)

/**
 * @brief cstring_iterator - The iterator type used for a string.
 * @param type - The type of iterator to act on.
 */
#define cstring_iterator(type) cstring_string_type(type)

/* --------------------------------- */
/* --- construction, destruction --- */

/**
 * @brief cstring_init - Allocate a new cstring with zero length.
 * @details Also see `cstring_assign()` and `cstring_reserve()`.
 * @param name - A not yet used variable name for the cstring variable to be
 *               declared and initialized.
 * @param type - The type of string to act on.
 * @return void
 */
#define cstring_init(name, type)                                                   \
    cstring_string_type(type) name = NULL;                                         \
    do {                                                                           \
        cstring_grow_((name), ((64 - sizeof(cstring_metadata_t)) / sizeof(type))); \
        (name)[0] = 0;                                                             \
        cstring_set_ttl_siz_((name), 1);                                           \
    } while (0)

/**
 * @brief cstring_assign - Assign a string to a cstring.
 * @details Also see `cstring_init()` and `cstring_reserve()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param ptr   - Pointer to the first character assigned to the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_assign(str, ptr, count)                                  \
    do {                                                                 \
        if ((str) && cstring_ttl_cap_(str) < (count) + 1) {              \
            cstring_free(str);                                           \
        }                                                                \
        if (!(str)) {                                                    \
            cstring_grow_((str), (count) + 1);                           \
        }                                                                \
        if ((ptr) != NULL && (count)) {                                  \
            cstring_clib_memcpy((str), (ptr), (count) * sizeof(*(ptr))); \
            cstring_set_ttl_siz_((str), (count) + 1);                    \
            (str)[count] = 0;                                            \
        }                                                                \
    } while (0)

/**
 * @brief cstring_free - Free all memory associated with the cstring and set it
 *        to NULL.
 * @param str - The cstring. Can be a NULL string.
 * @return void
 */
#define cstring_free(str)                                    \
    do {                                                     \
        if (str) {                                           \
            cstring_clib_free(cstring_string_to_base_(str)); \
            (str) = NULL;                                    \
        }                                                    \
    } while (0)

/* ---------------------- */
/* --- element access --- */

/**
 * @brief cstring_at - Return a reference to the character at position `pos` in
 *        the cstring.
 * @param str - The cstring.
 * @param pos - Position of a character in the string.
 * @return A reference to the character at the specified position in the string.
 */
#define cstring_at(str, pos) \
    ((size_t)(pos) < cstring_size(str) ? (str) + (pos) : NULL)

/**
 * @brief cstring_front - Return a reference to the first character in the
 *        cstring.
 * @details Unlike member cstring_begin, which returns an iterator to this same
 *          character, this function returns a direct reference.
 * @param str - The cstring.
 * @return A reference to the first character in the cstring.
 */
#define cstring_front(str) \
    (cstring_size(str) ? (str) : NULL)

/**
 * @brief cstring_back - Return a reference to the last character in the
 *        cstring.
 * @details Unlike member cstring_end, which returns an iterator just past this
 *          character, this function returns a direct reference.
 * @param str - The cstring.
 * @return A reference to the last character in the cstring.
 */
#define cstring_back(str) \
    (cstring_size(str) ? (str) + cstring_size(str) - 1 : NULL)

/* ----------------- */
/* --- iterators --- */

/**
 * @brief cstring_begin - Return an iterator to first character of the string.
 * @param str - The cstring.
 * @return A pointer to the first character (or NULL).
 */
#define cstring_begin(str) \
    (str)

/**
 * @brief cstring_end - Return an iterator to one past the last character of the
 *        string.
 * @param str - The cstring.
 * @return A pointer to one past the last character (or NULL).
 */
#define cstring_end(str) \
    ((str) ? (str) + cstring_size(str) : NULL)

/* ---------------- */
/* --- capacity --- */

/**
 * @brief cstring_empty - Return non-zero if the string is empty.
 * @param str - The cstring. Can be a NULL string.
 * @return Non-zero if empty, zero if non-empty.
 */
#define cstring_empty(str) \
    (cstring_size(str) == 0)

/**
 * @brief cstring_size - Get the current length of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The length as a `size_t`, terminating null not counted.
 */
#define cstring_size(str) \
    (cstring_ttl_siz_(str) ? cstring_ttl_siz_(str) - 1 : (size_t)0)

/**
 * @brief cstring_length - Gets the current length of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The length as a `size_t`, terminating null not counted.
 */
#define cstring_length(str) \
    cstring_size(str)

/**
 * @brief cstring_reserve - Request that the string capacity be at least enough
 *        to contain `n` characters.
 * @details If `n` is greater than the current string capacity, the function
 *          causes the container to reallocate its storage increasing its
 *          capacity to `n` (or greater). <br>
 *          Also see `cstring_init()` and `cstring_assign()`.
 * @param str - The cstring. Can be a NULL string.
 * @param n   - Minimum capacity for the string.
 * @return void
 */
#define cstring_reserve(str, n)                            \
    do {                                                   \
        const int is_new__ = (str) == NULL;                \
        if (is_new__ || cstring_ttl_cap_(str) < (n) + 1) { \
            cstring_grow_((str), (n) + 1);                 \
        }                                                  \
        if (is_new__) {                                    \
            cstring_set_ttl_siz_((str), 1);                \
            (str)[0] = 0;                                  \
        }                                                  \
    } while (0)

/**
 * @brief cstring_capacity - Get the current capacity of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The capacity as a `size_t`.
 */
#define cstring_capacity(str) \
    (cstring_ttl_cap_(str) ? cstring_ttl_cap_(str) - 1 : (size_t)0)

/**
 * @brief cstring_shrink_to_fit - Request the container to reduce its capacity
 *        to fit its size.
 * @param str - The cstring.
 * @return void
 */
#define cstring_shrink_to_fit(str)                         \
    do {                                                   \
        if (str) {                                         \
            const size_t cs_sz___ = cstring_ttl_siz_(str); \
            cstring_grow_(str, cs_sz___);                  \
        }                                                  \
    } while (0)

/* ----------------- */
/* --- modifiers --- */

/**
 * @brief cstring_clear - Erase all of the characters in the string.
 * @param str - The cstring.
 * @return void
 */
#define cstring_clear(str)                  \
    do {                                    \
        if (str) {                          \
            cstring_set_ttl_siz_((str), 1); \
            (str)[0] = 0;                   \
        }                                   \
    } while (0)

/**
 * @brief cstring_insert - Insert a string at position `pos` into the cstring.
 * @param str   - The cstring.
 * @param pos   - Position in the string where the new characters are inserted.
 * @param ptr   - Pointer to the first character inserted into the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_insert(str, pos, ptr, count)                                     \
    do {                                                                         \
        if (str) {                                                               \
            const size_t new_ttl_sz__ = cstring_ttl_siz_(str) + (count);         \
            if (cstring_ttl_cap_(str) < new_ttl_sz__) {                          \
                cstring_grow_((str), new_ttl_sz__);                              \
            }                                                                    \
            if ((pos) < cstring_size(str)) {                                     \
                cstring_clib_memmove(                                            \
                    (str) + (pos) + (count),                                     \
                    (str) + (pos),                                               \
                    sizeof(*(str)) * ((cstring_size(str)) - (pos)));             \
            }                                                                    \
            cstring_clib_memcpy((str) + (pos), (ptr), (count) * sizeof(*(ptr))); \
            cstring_set_ttl_siz_((str), new_ttl_sz__);                           \
            (str)[cstring_size(str)] = 0;                                        \
        }                                                                        \
    } while (0)

/**
 * @brief cstring_erase - Remove the characters beginning at offset `pos` from
 *        the cstring.
 * @param str - The cstring.
 * @param pos - Offset of the first character erased from the cstring.
 * @param n   - Number of consecutive characters to be erased.
 * @return void
 */
#define cstring_erase(str, pos, n)                                                                                        \
    do {                                                                                                                  \
        if ((str) && (size_t)(pos) < cstring_size(str)) {                                                                 \
            const size_t cs_count__ = (size_t)(pos) + (n) >= cstring_size(str) ? cstring_size(str) - (pos) : (size_t)(n); \
            cstring_set_ttl_siz_((str), cstring_ttl_siz_(str) - cs_count__);                                              \
            cstring_clib_memmove(                                                                                         \
                (str) + (pos),                                                                                            \
                (str) + (pos) + cs_count__,                                                                               \
                sizeof(*(str)) * (cstring_ttl_siz_(str) - (pos)));                                                        \
        }                                                                                                                 \
    } while (0)

/**
 * @brief cstring_push_back - Add a character to the end of the string.
 * @param str   - The cstring.
 * @param value - The character to add.
 * @return void
 */
#define cstring_push_back(str, value)                                \
    do {                                                             \
        if (str) {                                                   \
            const size_t new_ttl_siz___ = cstring_ttl_siz_(str) + 1; \
            if (cstring_ttl_cap_(str) < new_ttl_siz___) {            \
                cstring_grow_((str), new_ttl_siz___);                \
            }                                                        \
            cstring_set_ttl_siz_((str), new_ttl_siz___);             \
            (str)[new_ttl_siz___ - 2] = (value);                     \
            (str)[new_ttl_siz___ - 1] = 0;                           \
        }                                                            \
    } while (0)

/**
 * @brief cstring_pop_back - Remove the last character from the cstring.
 * @param str - The cstring.
 * @return void
 */
#define cstring_pop_back(str)                               \
    do {                                                    \
        if (cstring_size(str)) {                            \
            cstring_set_ttl_siz_((str), cstring_size(str)); \
            (str)[cstring_size(str)] = 0;                   \
        }                                                   \
    } while (0)

/**
 * @brief cstring_append - Append a string at the end of the cstring.
 * @param str   - The cstring.
 * @param ptr   - Pointer to the first character appended to the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_append(str, ptr, count) \
    cstring_insert((str), cstring_size(str), (ptr), (count))

/**
 * @brief cstring_replace - Replace a substring beginning at position `pos` with
 *        another string.
 * @param str   - The cstring.
 * @param pos   - Offset of the first character replaced in the cstring.
 * @param n     - Number of consecutive characters to be replaced.
 * @param ptr   - Pointer to the first replacement character.
 * @param count - Number of consecutive replacement characters to be used.
 * @return void
 */
#define cstring_replace(str, pos, n, ptr, count)      \
    do {                                              \
        cstring_erase((str), (pos), (n));             \
        cstring_insert((str), (pos), (ptr), (count)); \
    } while (0)

/**
 * @brief cstring_copy - Copy a cstring.
 * @param from - The original cstring.
 * @param to   - Destination to which the cstring is copied.
 * @return void
 */
#define cstring_copy(from, to)                                                  \
    do {                                                                        \
        if (from) {                                                             \
            const size_t from_ttl_sz__ = cstring_ttl_siz_(from);                \
            cstring_grow_((to), from_ttl_sz__);                                 \
            cstring_clib_memcpy((to), (from), from_ttl_sz__ * sizeof(*(from))); \
            cstring_set_ttl_siz_((to), from_ttl_sz__);                          \
        }                                                                       \
    } while (0)

/**
 * @brief cstring_resize - Resize the container to contain `count` characters.
 * @param str   - The cstring.
 * @param count - New size of the cstring.
 * @param value - The value to initialize new characters with.
 * @return void
 */
#define cstring_resize(str, count, value)                       \
    do {                                                        \
        if (str) {                                              \
            const size_t cs_sz_count__ = (size_t)((count) + 1); \
            size_t cs_sz__             = cstring_size(str);     \
            if (cs_sz_count__ > cs_sz__) {                      \
                cstring_reserve((str), (count));                \
                do {                                            \
                    (str)[cs_sz__++] = (value);                 \
                } while (cs_sz__ < cs_sz_count__);              \
            }                                                   \
            cstring_set_ttl_siz_((str), cs_sz_count__);         \
            (str)[count] = 0;                                   \
        }                                                       \
    } while (0)

/**
 * @brief cstring_swap - Exchange the content of the cstring by the content of
 *        another cstring of the same type.
 * @param str   - The original cstring.
 * @param other - The other cstring to swap content with.
 * @return void
 */
#define cstring_swap(str, other)           \
    do {                                   \
        if ((str) && (other)) {            \
            void *cs_swap__ = (void *)str; \
            str             = other;       \
            other           = cs_swap__;   \
        }                                  \
    } while (0)

/* ------------------ */
/* --- operations --- */

/**
 * @brief cstring_substring - Copy part of a string.
 * @param from - The original cstring.
 * @param pos  - Position in the original cstring where the substring begins.
 * @param n    - Number of consecutive characters in the substring.
 * @param to   - Destination to which the substring is copied.
 * @return void
 */
#define cstring_substring(from, pos, n, to)                                                                                  \
    do {                                                                                                                     \
        if ((from) && cstring_ttl_siz_(from) > (size_t)(pos)) {                                                              \
            const size_t cs_count___ = (size_t)(pos) + (n) >= cstring_size(from) ? cstring_size(from) - (pos) : (size_t)(n); \
            cstring_grow_((to), cs_count___ + 1);                                                                            \
            cstring_clib_memcpy((to), (from) + (pos), cs_count___ * sizeof(*(from)));                                        \
            cstring_set_ttl_siz_((to), cs_count___ + 1);                                                                     \
            (to)[cs_count___] = 0;                                                                                           \
        }                                                                                                                    \
    } while (0)

/* ========================== */
/* === INTERNAL INTERFACE === */
/* ========================== */

/** @cond INTERNAL */

/* in case C library functions need extra protection, allow these defines to be overridden. */
#ifndef cstring_clib_free
#include <stdlib.h>
#define cstring_clib_free free
#endif
#ifndef cstring_clib_malloc
#include <stdlib.h>
#define cstring_clib_malloc malloc
#endif
#ifndef cstring_clib_realloc
#include <stdlib.h>
#define cstring_clib_realloc realloc
#endif
#ifndef cstring_clib_assert
#include <assert.h>
#define cstring_clib_assert assert
#endif
#ifndef cstring_clib_memcpy
#include <string.h>
#define cstring_clib_memcpy memcpy
#endif
#ifndef cstring_clib_memmove
#include <string.h>
#define cstring_clib_memmove memmove
#endif

/**
 * @brief cstring_metadata_t - Header type that prefixes a cstring.
 */
typedef struct cstring_metadata_ {
    size_t size;            /*!< The size incl. terminating null. */
    size_t capacity;        /*!< The capacity incl. space for the string terminator. */
    void (*unused)(void *); /*!< This maintains binary compatibility with `cvector_metadata_t` of the c-vector library. */
} cstring_metadata_t;

/**
 * @brief cstring_string_to_base_ - For internal use, convert a cstring pointer
 *        to a metadata pointer.
 * @param str - The cstring.
 * @return The metadata pointer of the cstring.
 */
#define cstring_string_to_base_(str) \
    (((cstring_metadata_t *)(str)) - 1)

/**
 * @brief cstring_base_to_string_ - For internal use, convert a metadata pointer
 *        to a cstring pointer.
 * @param ptr - Pointer to the metadata.
 * @return The cstring.
 */
#define cstring_base_to_string_(ptr) \
    ((void *)((cstring_metadata_t *)(ptr) + 1))

/**
 * @brief cstring_ttl_cap_ - For internal use, get the current capacity of the
 *        cstring incl. terminating null.
 * @param str - The cstring.
 * @return The capacity as a `size_t`.
 */
#define cstring_ttl_cap_(str) \
    ((str) ? cstring_string_to_base_(str)->capacity : (size_t)0)

/**
 * @brief cstring_ttl_siz_ - For internal use, get the current size of the
 *        cstring incl. terminating null.
 * @param str - The cstring.
 * @return The size as a `size_t`.
 */
#define cstring_ttl_siz_(str) \
    ((str) ? cstring_string_to_base_(str)->size : (size_t)0)

/**
 * @brief cstring_set_ttl_cap_ - For internal use, set the capacity member of
 *        the metadata.
 * @param str - The cstring.
 * @param cap - The new capacity to set.
 * @return void
 */
#define cstring_set_ttl_cap_(str, cap)                      \
    do {                                                    \
        if (str) {                                          \
            cstring_string_to_base_(str)->capacity = (cap); \
        }                                                   \
    } while (0)

/**
 * @brief cstring_set_ttl_siz_ - For internal use, set the size member of the
 *        metadata.
 * @param str - The cstring.
 * @param siz - The new size to set.
 * @return void
 */
#define cstring_set_ttl_siz_(str, siz)                  \
    do {                                                \
        if (str) {                                      \
            cstring_string_to_base_(str)->size = (siz); \
        }                                               \
    } while (0)

/**
 * @brief cstring_grow_ - For internal use, ensure that the cstring is at least
 *        `count` characters big.
 * @param str   - The cstring.
 * @param count - The new capacity to set.
 * @return void
 */
#define cstring_grow_(str, count)                                                              \
    do {                                                                                       \
        const size_t cs_siz__ = (count) * sizeof(*(str)) + sizeof(cstring_metadata_t);         \
        if (str) {                                                                             \
            void *const cs_p__ = cstring_clib_realloc(cstring_string_to_base_(str), cs_siz__); \
            cstring_clib_assert(cs_p__);                                                       \
            (str) = cstring_base_to_string_(cs_p__);                                           \
        } else {                                                                               \
            void *const cs_p__ = cstring_clib_malloc(cs_siz__);                                \
            cstring_clib_assert(cs_p__);                                                       \
            (str) = cstring_base_to_string_(cs_p__);                                           \
            cstring_set_ttl_siz_((str), 0);                                                    \
            cstring_string_to_base_(str)->unused = NULL;                                       \
        }                                                                                      \
        cstring_set_ttl_cap_((str), (count));                                                  \
    } while (0)

/** @endcond */

#endif /* header guard */
