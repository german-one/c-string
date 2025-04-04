#ifndef HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_0
#define HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_0
/**
 * @copyright Copyright (c) 2025 Steffen Illhardt,
 *            Licensed under the MIT license
 *            ( https://opensource.org/license/mit/ ).
 * @brief     cstring - Heap implemented using C library allocation functions.
 * @file      cstring.h
 * @version   1.0
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

/* ----------------------- */
/* --- cstring literal --- */

/**
 * @brief cstring_literal - Generate a cstring object with static duration.
 * @note The pointer references static read-only data which is constant at
 *       compile time. DO NOT FREE IT.
 * @param name - A not yet used variable name for the cstring object.
 * @param type - The type of string to act on.
 * @param lit  - A string literal used to create the cstring literal. The
 *               argument passed to this parameter cannot be a pointer!
 */
#define cstring_literal(name, type, lit)                                                                               \
    cstring_string_type(const type) name;                                                                              \
    do {                                                                                                               \
        static const struct _cstring_literal_tag_##name {                                                              \
            const size_t size;                                                                                         \
            const size_t capacity;                                                                                     \
            void (*const unused)(void *);                                                                              \
            const type data[((sizeof(lit) + sizeof(size_t) - 1) / sizeof(size_t)) * sizeof(size_t) / sizeof(type)];    \
        } _cstring_literal_container_##name = {(sizeof(lit) / sizeof(type)), (sizeof(lit) / sizeof(type)), NULL, lit}; \
        name                                = &*_cstring_literal_container_##name.data;                                \
    } while (0)

/* --------------------------------- */
/* --- construction, destruction --- */

/**
 * @brief cstring_init - Allocate a new cstring with zero length.
 * @details Also see `cstring_assign()`, `cstring_reserve()`,
 *          `cstring_push_back()`, `cstring_append()`, `cstring_resize()`.
 * @param name - A not yet used variable name for the cstring variable to be
 *               declared and initialized.
 * @param type - The type of string to act on.
 * @return void
 */
#define cstring_init(name, type)                                                 \
    cstring_string_type(type) name = NULL;                                       \
    do {                                                                         \
        cstring_grow_(name, ((64 - sizeof(cstring_metadata_t)) / sizeof(type))); \
        (name)[0] = 0;                                                           \
        cstring_set_ttl_siz_(name, 1);                                           \
    } while (0)

/**
 * @brief cstring_assign - Assign a string to a cstring.
 * @details Also see `cstring_init()`, `cstring_reserve()`,
 *         `cstring_push_back()`, `cstring_append()`, `cstring_resize()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param ptr   - Pointer to the first character assigned to the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_assign(str, ptr, count)                                          \
    do {                                                                         \
        const void *const tmp_ptr__ = (const void *)(ptr);                       \
        if ((str) && cstring_ttl_cap_(str) < (size_t)(count) + 1) {              \
            cstring_free(str);                                                   \
        }                                                                        \
        if (!(str)) {                                                            \
            cstring_grow_((str), (size_t)(count) + 1);                           \
        }                                                                        \
        if (tmp_ptr__ && (count)) {                                              \
            cstring_clib_memcpy((str), (ptr), (size_t)(count) * sizeof(*(ptr))); \
            cstring_set_ttl_siz_((str), (size_t)(count) + 1);                    \
            (str)[(size_t)(count)] = 0;                                          \
        } else {                                                                 \
            cstring_set_ttl_siz_((str), 1);                                      \
            (str)[0] = 0;                                                        \
        }                                                                        \
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
    ((size_t)(pos) < cstring_size(str) ? (str) + (size_t)(pos) : NULL)

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
 * @return Non-zero if `str` is NULL or empty, zero if non-empty.
 */
#define cstring_empty(str) \
    (cstring_size(str) == 0)

/**
 * @brief cstring_size - Get the current length of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The length as a `size_t`, terminating null not counted. Zero if `str`
 *         is NULL.
 */
#define cstring_size(str) \
    (cstring_ttl_siz_(str) ? cstring_ttl_siz_(str) - 1 : (size_t)0)

/**
 * @brief cstring_length - Get the current length of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The length as a `size_t`, terminating null not counted. Zero if `str`
 *         is NULL.
 */
#define cstring_length(str) \
    cstring_size(str)

/**
 * @brief cstring_max_size - Get the maximum number of elements a string of the
 *        specified character type is able to hold.
 * @note The returned value is technically possible. However, typically
 *       allocations of such a big size will fail.
 * @param type - The type of string to act on.
 * @return The maximum number of elements the string is able to hold.
 */
#define cstring_max_size(type) \
    (((((size_t)1) << ((sizeof(ptrdiff_t) < sizeof(size_t) ? sizeof(ptrdiff_t) : sizeof(size_t)) * CHAR_BIT - 1)) - 1 - sizeof(cstring_metadata_t)) / sizeof(type) - 1)

/**
 * @brief cstring_reserve - Request that the string capacity be at least enough
 *        to contain `n` characters.
 * @details If `n` is greater than the current string capacity, the function
 *          causes the container to reallocate its storage increasing its
 *          capacity to `n` (or greater). <br>
 *          Also see `cstring_init()`, `cstring_assign()`,
 *          `cstring_push_back()`, `cstring_append()`, `cstring_resize()`.
 * @param str - The cstring. Can be a NULL string.
 * @param n   - Minimum capacity for the string.
 * @return void
 */
#define cstring_reserve(str, n)                                    \
    do {                                                           \
        const int is_new__ = ((str) == NULL);                      \
        if (is_new__ || cstring_ttl_cap_(str) < (size_t)(n) + 1) { \
            cstring_grow_((str), (size_t)(n) + 1);                 \
        }                                                          \
        if (is_new__) {                                            \
            cstring_set_ttl_siz_((str), 1);                        \
            (str)[0] = 0;                                          \
        }                                                          \
    } while (0)

/**
 * @brief cstring_capacity - Get the current capacity of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The capacity as a `size_t`. Zero if `str` is NULL.
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

/**
 * @brief cstring_unsafe_set_size - Set the size property to the specified
 *        value and add the string terminator accordingly.
 * @details Providing a cstring with sufficiently large capacity as buffer to
 *          external API is supported. However, the third party API cannot
 *          update the header data of a cstring. This function enables the user
 *          to manually update the size property in order to keep it usable in
 *          the cstring API.
 * @note This function does not examine the string data to evaluate the
 *       credibility of the specified size. Furthermore, this function does not
 *       force the capacity of the cstring to grow. If the required size exceeds
 *       the current capacity, the function sets the size to meet the capacity.
 *       Consider the memory being corrupted by the API that updated the string
 *       data in this case.
 * @param str  - The cstring.
 * @param size - The size to be applied.
 * @return void
 */
#define cstring_unsafe_set_size(str, size)                                                                              \
    do {                                                                                                                \
        if (str) {                                                                                                      \
            const size_t cs_new_sz__ = cstring_capacity(str) < (size_t)(size) ? cstring_capacity(str) : (size_t)(size); \
            cstring_set_ttl_siz_((str), cs_new_sz__ + 1);                                                               \
            (str)[cs_new_sz__] = 0;                                                                                     \
        }                                                                                                               \
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
#define cstring_insert(str, pos, ptr, count)                                                                                                                  \
    do {                                                                                                                                                      \
        if (str) {                                                                                                                                            \
            const size_t new_ttl_sz__ = cstring_ttl_siz_(str) + (size_t)(count);                                                                              \
            if (cstring_ttl_cap_(str) < new_ttl_sz__) {                                                                                                       \
                cstring_grow_((str), new_ttl_sz__);                                                                                                           \
            }                                                                                                                                                 \
            if ((size_t)(pos) < cstring_size(str)) {                                                                                                          \
                cstring_clib_memmove((str) + (size_t)(pos) + (size_t)(count), (str) + (size_t)(pos), sizeof(*(str)) * ((cstring_size(str)) - (size_t)(pos))); \
            }                                                                                                                                                 \
            cstring_clib_memcpy((str) + (size_t)(pos), (ptr), (size_t)(count) * sizeof(*(ptr)));                                                              \
            cstring_set_ttl_siz_((str), new_ttl_sz__);                                                                                                        \
            (str)[cstring_size(str)] = 0;                                                                                                                     \
        }                                                                                                                                                     \
    } while (0)

/**
 * @brief cstring_erase - Remove the characters beginning at offset `pos` from
 *        the cstring.
 * @param str - The cstring.
 * @param pos - Offset of the first character erased from the cstring.
 * @param n   - Number of consecutive characters to be erased.
 * @return void
 */
#define cstring_erase(str, pos, n)                                                                                                                     \
    do {                                                                                                                                               \
        if ((str) && (size_t)(pos) < cstring_size(str)) {                                                                                              \
            const size_t cs_count__ = (size_t)(pos) + (size_t)(n) >= cstring_size(str) ? cstring_size(str) - (size_t)(pos) : (size_t)(n);              \
            cstring_set_ttl_siz_((str), cstring_ttl_siz_(str) - cs_count__);                                                                           \
            cstring_clib_memmove((str) + (size_t)(pos), (str) + (size_t)(pos) + cs_count__, sizeof(*(str)) * (cstring_ttl_siz_(str) - (size_t)(pos))); \
        }                                                                                                                                              \
    } while (0)

/**
 * @brief cstring_push_back - Add a character to the end of the string.
 * @details Also see `cstring_init()`, `cstring_assign()`, `cstring_reserve()`,
 *          `cstring_append()`, `cstring_resize()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param value - The character to add.
 * @return void
 */
#define cstring_push_back(str, value)                 \
    do {                                              \
        size_t new_ttl_siz___;                        \
        if (!(str)) {                                 \
            cstring_grow_((str), 2);                  \
            cstring_set_ttl_siz_((str), 1);           \
        }                                             \
        new_ttl_siz___ = cstring_ttl_siz_(str) + 1;   \
        if (cstring_ttl_cap_(str) < new_ttl_siz___) { \
            cstring_grow_((str), new_ttl_siz___);     \
        }                                             \
        cstring_set_ttl_siz_((str), new_ttl_siz___);  \
        (str)[new_ttl_siz___ - 2] = (value);          \
        (str)[new_ttl_siz___ - 1] = 0;                \
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
 * @details Also see `cstring_init()`, `cstring_assign()`, `cstring_reserve()`,
 *          `cstring_push_back()`, `cstring_resize()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param ptr   - Pointer to the first character appended to the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_append(str, ptr, count)                                       \
    do {                                                                      \
        if (str) {                                                            \
            cstring_insert((str), cstring_size(str), (ptr), (size_t)(count)); \
        } else {                                                              \
            cstring_assign((str), (ptr), (size_t)(count));                    \
        }                                                                     \
    } while (0)

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
#define cstring_replace(str, pos, n, ptr, count)                      \
    do {                                                              \
        cstring_erase((str), (size_t)(pos), (size_t)(n));             \
        cstring_insert((str), (size_t)(pos), (ptr), (size_t)(count)); \
    } while (0)

/**
 * @brief cstring_copy - Copy a cstring.
 * @param from - The original cstring.
 * @param to   - Destination to which the cstring is copied. Can be a NULL
 *               string.
 * @return void
 */
#define cstring_copy(from, to)                                                  \
    do {                                                                        \
        if (from) {                                                             \
            const size_t from_ttl_sz__ = cstring_ttl_siz_(from);                \
            if ((to) && cstring_ttl_cap_(to) < from_ttl_sz__) {                 \
                cstring_free(to);                                               \
            }                                                                   \
            if (!(to)) {                                                        \
                cstring_grow_((to), from_ttl_sz__);                             \
            }                                                                   \
            cstring_clib_memcpy((to), (from), from_ttl_sz__ * sizeof(*(from))); \
            cstring_set_ttl_siz_((to), from_ttl_sz__);                          \
        }                                                                       \
    } while (0)

/**
 * @brief cstring_resize - Resize the container to contain `count` characters.
 * @details Also see `cstring_init()`, `cstring_assign()`, `cstring_reserve()`,
 *          `cstring_push_back()`, `cstring_append()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param count - New size of the cstring.
 * @param value - The value to initialize new characters with.
 * @return void
 */
#define cstring_resize(str, count, value)                 \
    do {                                                  \
        const size_t cs_sz_count__ = (size_t)(count) + 1; \
        size_t cs_sz__             = cstring_size(str);   \
        if (cs_sz_count__ > cs_sz__ + 1) {                \
            cstring_grow_((str), cs_sz_count__);          \
            do {                                          \
                (str)[cs_sz__++] = (value);               \
            } while (cs_sz__ < cs_sz_count__);            \
        }                                                 \
        cstring_set_ttl_siz_((str), cs_sz_count__);       \
        (str)[(size_t)(count)] = 0;                       \
    } while (0)

/**
 * @brief cstring_swap - Exchange the content of the cstring by the content of
 *        another cstring of the same type.
 * @param str   - The original cstring. Can be a NULL string.
 * @param other - The other cstring to swap content with. Can be a NULL string.
 * @return void
 */
#define cstring_swap(str, other)       \
    do {                               \
        void *cs_swap__ = (void *)str; \
        str             = other;       \
        other           = cs_swap__;   \
    } while (0)

/**
 * @brief cstring_trim - Remove contiguous occurrences of the specified
 *        character from the begin and/or the end of a cstring.
 * @param str   - The cstring.
 * @param value - The character to be removed.
 * @param mode  - Flags specifying where the characters are removed. <br>
 *                  1 to remove leading characters <br>
 *                  2 to remove trailing characters <br>
 *                Their combination (1|2) results in trimming on both sides of
 *                the string.
 * @return void
 */
#define cstring_trim(str, value, mode)                                                          \
    do {                                                                                        \
        size_t cs_end_i__ = cstring_size(str);                                                  \
        if (cs_end_i__) {                                                                       \
            size_t cs_new_len__;                                                                \
            const int mode_head__ = (int)((mode) & 1);                                          \
            const int mode_tail__ = (int)((mode) & 2);                                          \
            size_t cs_beg_i__     = 0;                                                          \
            if (mode_tail__) {                                                                  \
                while (cs_end_i__ > cs_beg_i__) {                                               \
                    if ((str)[--cs_end_i__] != (value)) {                                       \
                        ++cs_end_i__;                                                           \
                        break;                                                                  \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
            if (mode_head__) {                                                                  \
                while (cs_beg_i__ < cs_end_i__) {                                               \
                    if ((str)[cs_beg_i__] != (value)) {                                         \
                        break;                                                                  \
                    }                                                                           \
                    ++cs_beg_i__;                                                               \
                }                                                                               \
            }                                                                                   \
            cs_new_len__ = cs_end_i__ - cs_beg_i__;                                             \
            if (cs_new_len__ && cs_beg_i__) {                                                   \
                cstring_clib_memmove((str), (str) + cs_beg_i__, sizeof(*(str)) * cs_new_len__); \
            }                                                                                   \
            cstring_set_ttl_siz_((str), cs_new_len__ + 1);                                      \
            (str)[cs_new_len__] = 0;                                                            \
        }                                                                                       \
    } while (0)

/**
 * @brief cstring_fix - Update the cstring to a fixed length by either padding
 *        or shortening.
 * @param str    - The cstring.
 * @param length - New length of the cstring.
 * @param value  - Character used for the padding.
 * @param mode   - Flags specifying where the cstring is to be padded or
 *                 shortened. <br>
 *                   1 at the begin of the cstring <br>
 *                   2 at the end of the cstring <br>
 *                 Their combination (1|2) leads to a centered alignment.
 * @return void
 */
#define cstring_fix(str, length, value, mode)                                                                                          \
    do {                                                                                                                               \
        if ((str) && (ptrdiff_t)(length) >= 0) {                                                                                       \
            ptrdiff_t cs_diff__ = (ptrdiff_t)((size_t)(length) - (cstring_size(str)));                                                 \
            if (cs_diff__) {                                                                                                           \
                const int mode_head___     = (int)((mode) & 1);                                                                        \
                const int mode_tail___     = (int)((mode) & 2);                                                                        \
                const ptrdiff_t head_len__ = mode_head___ && mode_tail___ ? cs_diff__ / 2 : (mode_head___ ? cs_diff__ : (ptrdiff_t)0); \
                if (cs_diff__ < 0) {                                                                                                   \
                    cstring_clib_memmove((str), (str) - (head_len__), (size_t)(length) * sizeof(*(str)));                              \
                } else {                                                                                                               \
                    if ((size_t)(length) + 1 > cstring_ttl_cap_(str)) {                                                                \
                        cstring_grow_((str), (size_t)(length) + 1);                                                                    \
                    }                                                                                                                  \
                    if (mode_head___ && head_len__) {                                                                                  \
                        ptrdiff_t n__ = 0;                                                                                             \
                        cstring_clib_memmove((str) + head_len__, (str), cstring_size(str) * sizeof(*(str)));                           \
                        while (n__ < head_len__) {                                                                                     \
                            (str)[n__++] = (value);                                                                                    \
                        }                                                                                                              \
                    }                                                                                                                  \
                    if (mode_tail___) {                                                                                                \
                        ptrdiff_t n__       = head_len__ + (ptrdiff_t)cstring_size(str);                                               \
                        const ptrdiff_t e__ = (ptrdiff_t)(length);                                                                     \
                        while (n__ < e__) {                                                                                            \
                            (str)[n__++] = (value);                                                                                    \
                        }                                                                                                              \
                    }                                                                                                                  \
                }                                                                                                                      \
                cstring_set_ttl_siz_((str), (size_t)(length) + 1);                                                                     \
                (str)[(size_t)(length)] = L'\0';                                                                                       \
            }                                                                                                                          \
        }                                                                                                                              \
    } while (0)

/**
 * @brief cstring_reverse - Reverse the character order in the cstring.
 * @param str - The cstring.
 * @return void
 */
#define cstring_reverse(str)                                 \
    do {                                                     \
        const size_t cs_tmp_i__ = cstring_size(str);         \
        if (cs_tmp_i__ > 1) {                                \
            size_t cs_end_i___ = cs_tmp_i__;                 \
            size_t cs_beg_i___ = 0;                          \
            while (cs_end_i___ > cs_beg_i___) {              \
                (str)[cs_tmp_i__]    = (str)[--cs_end_i___]; \
                (str)[cs_end_i___]   = (str)[cs_beg_i___];   \
                (str)[cs_beg_i___++] = (str)[cs_tmp_i__];    \
            }                                                \
            (str)[cs_tmp_i__] = 0;                           \
        }                                                    \
    } while (0)

/* ------------------ */
/* --- operations --- */

/**
 * @brief cstring_substring - Copy a part of a string.
 * @param from - The source cstring.
 * @param pos  - Position in the source cstring where the substring begins.
 * @param n    - Number of consecutive characters copied to the substring.
 * @param to   - Destination to which the substring is copied. Can be a NULL
 *               string.
 * @return void
 */
#define cstring_substring(from, pos, n, to)                                                                                                  \
    do {                                                                                                                                     \
        if ((from) && cstring_ttl_siz_(from) > (size_t)(pos)) {                                                                              \
            const size_t cs_count___ = (size_t)(pos) + (size_t)(n) >= cstring_size(from) ? cstring_size(from) - (size_t)(pos) : (size_t)(n); \
            if ((to) && cstring_ttl_cap_(to) < cs_count___ + 1) {                                                                            \
                cstring_free(to);                                                                                                            \
            }                                                                                                                                \
            if (!(to)) {                                                                                                                     \
                cstring_grow_((to), cs_count___ + 1);                                                                                        \
            }                                                                                                                                \
            cstring_clib_memcpy((to), (from) + (size_t)(pos), cs_count___ * sizeof(*(from)));                                                \
            cstring_set_ttl_siz_((to), cs_count___ + 1);                                                                                     \
            (to)[cs_count___] = 0;                                                                                                           \
        }                                                                                                                                    \
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

#include <limits.h>
#include <stddef.h>

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
    (((cstring_metadata_t *)(void *)(str)) - 1)

/**
 * @brief cstring_string_to_const_base_ - For internal use, convert a cstring
 *        pointer to a pointer to constant metadata.
 * @param str - The cstring.
 * @return The pointer to constant metadata of the cstring.
 */
#define cstring_string_to_const_base_(str) \
    (((const cstring_metadata_t *)(const void *)(str)) - 1)

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
    ((str) ? cstring_string_to_const_base_(str)->capacity : (size_t)0)

/**
 * @brief cstring_ttl_siz_ - For internal use, get the current size of the
 *        cstring incl. terminating null.
 * @param str - The cstring.
 * @return The size as a `size_t`.
 */
#define cstring_ttl_siz_(str) \
    ((str) ? cstring_string_to_const_base_(str)->size : (size_t)0)

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
