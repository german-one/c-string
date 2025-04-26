#ifndef HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_2
#define HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_2
/**
 * @copyright Copyright (c) 2025 Steffen Illhardt,
 *            Licensed under the MIT license
 *            ( https://opensource.org/license/mit/ ).
 * @brief     cstring - Heap implemented using C library allocation functions.
 * @file      cstring.h
 * @version   1.2
 * @details
 *   The c-string library is based on the c-vector library,
 *   Copyright (c) 2015 Evan Teran,
 *   refer to: https://github.com/eteran/c-vector
 */

/* ======================== */
/* === PUBLIC INTERFACE === */
/* ======================== */

/**
 * @defgroup cstring_api The cstring API
 * @{
 */

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
 * @param str   - The cstring. Can be a NULL string. <br>
 *                If `str` refers to an existing cstring, the old content is
 *                overwritten.
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
 * @details
 * For clarity, this is like: <br>
 * `((min(PTRDIFF_MAX, (SIZE_MAX / 2)) - sizeof(metadata)) / sizeof(type) - 1)`,
 * with -1 for the string terminator that is not counted. Also, (SIZE_MAX / 2)
 * because any `array + SIZE_MAX` would be bogus. <br>
 * PTRDIFF_MAX and SIZE_MAX may not be defined in ancient C libraries. Hence the
 * calculation in the macro. However, the value of the macro is a constant
 * expression. It is supposed to be calculated at compile time.
 * @note The resulting value is technically possible. However, typically
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

/* -------------- */
/* --- search --- */

/**
 * @brief cstring_find - Find the first occurrence of the given substring.
 * @details Implements the Rabin-Karp algorithm.
 * @param str        - The cstring.
 * @param pos        - Position at which to start the search, i.e. the found
 *                     substring must not begin in a position preceding `pos`.
 *                     Zero means that the whole `str` is searched.
 * @param ptr        - Pointer to the first character of the string to search
 *                     for.
 * @param count      - Length of the string to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first character of the found substring or -1 if no
 *                     such substring is found.
 * @return void
 */
#define cstring_find(str, pos, ptr, count, ret_offset)                                                                                \
    do {                                                                                                                              \
        const void *const tmp_ptr___ = (const void *)(ptr);                                                                           \
        const ptrdiff_t sub_size__ = (ptrdiff_t)(count), str_size__ = (ptrdiff_t)cstring_size(str) - (ptrdiff_t)(pos);                \
        if (!tmp_ptr___ || (ptrdiff_t)(pos) < 0 || sub_size__ > str_size__ || str_size__ <= 0 || sub_size__ <= 0) {                   \
            (ret_offset) = (ptrdiff_t)-1;                                                                                             \
        } else if (sub_size__ == str_size__) {                                                                                        \
            int eq__;                                                                                                                 \
            str_n_eq_((str) + (ptrdiff_t)(pos), (ptr), sub_size__, eq__);                                                             \
            (ret_offset) = eq__ ? (ptrdiff_t)(pos) : (ptrdiff_t)-1;                                                                   \
        } else if (sub_size__ == 1) {                                                                                                 \
            find_first_char_(0, (str), (pos), (ptr), (ret_offset));                                                                   \
        } else {                                                                                                                      \
            ptrdiff_t cs_off__;                                                                                                       \
            const ptrdiff_t end__ = sub_size__ - 1, cs_diff__ = (ptrdiff_t)cstring_size(str) - sub_size__;                            \
            size_t str_hash__ = 0, sub_hash__ = 0, hash_factor__ = 1;                                                                 \
            const size_t mask__ = (sizeof(size_t) <= sizeof(*(str))) ? (size_t)-1 : (((size_t)1 << (sizeof(*(str)) * CHAR_BIT)) - 1); \
            (ret_offset)        = (ptrdiff_t)-1;                                                                                      \
            for (cs_off__ = 0; cs_off__ < end__; ++cs_off__) {                                                                        \
                str_hash__ = (str_hash__ << 1) + ((size_t)(str)[cs_off__ + (ptrdiff_t)(pos)] & mask__);                               \
                sub_hash__ = (sub_hash__ << 1) + ((size_t)(ptr)[cs_off__] & mask__);                                                  \
                hash_factor__ <<= 1;                                                                                                  \
            }                                                                                                                         \
            str_hash__ = (str_hash__ << 1) + ((size_t)(str)[cs_off__ + (ptrdiff_t)(pos)] & mask__);                                   \
            sub_hash__ = (sub_hash__ << 1) + ((size_t)(ptr)[cs_off__] & mask__);                                                      \
            cs_off__   = (ptrdiff_t)(pos);                                                                                            \
            do {                                                                                                                      \
                if (sub_hash__ == str_hash__) {                                                                                       \
                    int eq__;                                                                                                         \
                    str_n_eq_((str) + cs_off__, (ptr), sub_size__, eq__);                                                             \
                    if (eq__) {                                                                                                       \
                        (ret_offset) = cs_off__;                                                                                      \
                        break;                                                                                                        \
                    }                                                                                                                 \
                }                                                                                                                     \
                str_hash__ =                                                                                                          \
                    ((str_hash__ - hash_factor__ * ((size_t)(str)[cs_off__] & mask__)) << 1) +                                        \
                    ((size_t)(str)[cs_off__ + sub_size__] & mask__);                                                                  \
            } while (cs_off__++ < cs_diff__);                                                                                         \
        }                                                                                                                             \
    } while (0)

/**
 * @brief cstring_rfind - Find the last occurrence of the given substring.
 * @details Implements the Rabin-Karp algorithm.
 * @param str        - The cstring.
 * @param pos        - Position at which to start the search, proceeded from
 *                     right to left. Hence the found substring cannot begin in
 *                     a position following `pos`. -1 means that the whole `str`
 *                     is searched.
 * @param ptr        - Pointer to the first character of the string to search
 *                     for.
 * @param count      - Length of the string to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first character of the found substring or -1 if no
 *                     such substring is found.
 * @return void
 */
#define cstring_rfind(str, pos, ptr, count, ret_offset)                                                                                \
    do {                                                                                                                               \
        const void *const tmp_ptr____ = (const void *)(ptr);                                                                           \
        const ptrdiff_t sub_size___   = (ptrdiff_t)(count);                                                                            \
        const ptrdiff_t str_size___   = ((ptrdiff_t)(pos) == -1 || (ptrdiff_t)(pos) + sub_size___ > (ptrdiff_t)cstring_size(str))      \
                                            ? (ptrdiff_t)cstring_size(str)                                                             \
                                            : ((ptrdiff_t)(pos) + sub_size___);                                                        \
        if (!tmp_ptr____ || (ptrdiff_t)(pos) < -1 || sub_size___ > str_size___ || !str_size___ || sub_size___ <= 0) {                  \
            (ret_offset) = (ptrdiff_t)-1;                                                                                              \
        } else if (sub_size___ == str_size___) {                                                                                       \
            int eq___;                                                                                                                 \
            str_n_eq_((str), (ptr), sub_size___, eq___);                                                                               \
            (ret_offset) = eq___ ? (ptrdiff_t)0 : (ptrdiff_t)-1;                                                                       \
        } else if (sub_size___ == 1) {                                                                                                 \
            find_last_char_(0, (str), (pos), (ptr), (ret_offset));                                                                     \
        } else {                                                                                                                       \
            ptrdiff_t cs_off___;                                                                                                       \
            const ptrdiff_t cs_diff___ = str_size___ - sub_size___;                                                                    \
            size_t str_hash___ = 0, sub_hash___ = 0, hash_factor___ = 1;                                                               \
            const size_t mask___ = (sizeof(size_t) <= sizeof(*(str))) ? (size_t)-1 : (((size_t)1 << (sizeof(*(str)) * CHAR_BIT)) - 1); \
            (ret_offset)         = (ptrdiff_t)-1;                                                                                      \
            for (cs_off___ = sub_size___ - 1; cs_off___; --cs_off___) {                                                                \
                str_hash___ = (str_hash___ << 1) + ((size_t)(str)[cs_off___ + cs_diff___] & mask___);                                  \
                sub_hash___ = (sub_hash___ << 1) + ((size_t)(ptr)[cs_off___] & mask___);                                               \
                hash_factor___ <<= 1;                                                                                                  \
            }                                                                                                                          \
            str_hash___ = (str_hash___ << 1) + ((size_t)(str)[cs_off___ + cs_diff___] & mask___);                                      \
            sub_hash___ = (sub_hash___ << 1) + ((size_t)(ptr)[cs_off___] & mask___);                                                   \
            cs_off___   = cs_diff___;                                                                                                  \
            do {                                                                                                                       \
                if (sub_hash___ == str_hash___) {                                                                                      \
                    int eq___;                                                                                                         \
                    str_n_eq_((str) + cs_off___, (ptr), sub_size___, eq___);                                                           \
                    if (eq___) {                                                                                                       \
                        (ret_offset) = cs_off___;                                                                                      \
                        break;                                                                                                         \
                    }                                                                                                                  \
                }                                                                                                                      \
                --cs_off___;                                                                                                           \
                str_hash___ =                                                                                                          \
                    ((str_hash___ - hash_factor___ * ((size_t)(str)[cs_off___ + sub_size___] & mask___)) << 1) +                       \
                    ((size_t)(str)[cs_off___] & mask___);                                                                              \
            } while (cs_off___);                                                                                                       \
        }                                                                                                                              \
    } while (0)

/**
 * @brief cstring_find_first_of - Find the first character equal to one of the
 *        characters in the given character sequence.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define cstring_find_first_of(str, pos, ptr, count, ret_offset) \
    find_first_of_(0, str, pos, ptr, count, ret_offset)

/**
 * @brief cstring_find_first_not_of - Find the first character equal to none of
 *        the characters in the given character sequence.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define cstring_find_first_not_of(str, pos, ptr, count, ret_offset) \
    find_first_of_(1, str, pos, ptr, count, ret_offset)

/**
 * @brief cstring_find_last_of - Find the last character equal to one of the
 *        characters in the given character sequence.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching. -1 means that the
 *                     whole `str` is searched.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define cstring_find_last_of(str, pos, ptr, count, ret_offset) \
    find_last_of_(0, str, pos, ptr, count, ret_offset)

/**
 * @brief cstring_find_last_not_of - Find the last character equal to none of
 *        the characters in the given character sequence.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching. -1 means that the
 *                     whole `str` is searched.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define cstring_find_last_not_of(str, pos, ptr, count, ret_offset) \
    find_last_of_(1, str, pos, ptr, count, ret_offset)

/* ------------------ */
/* --- operations --- */

/**
 * @brief cstring_compare - Lexicographically compare two strings.
 * @param str1      - The first cstring.
 * @param str2      - The second cstring.
 * @param ret_order - Variable of type `int` that receives the result of the
 *                    comparison. <br>
 *                     -1 if `str1` appears before `str2` <br>
 *                      0 if both strings compare equivalent <br>
 *                      1 if `str1` appears after `str2`
 * @return void
 */
#define cstring_compare(str1, str2, ret_order)                                                                               \
    do {                                                                                                                     \
        if ((str1) && (str2)) {                                                                                              \
            const size_t min__ = cstring_size(str1) < cstring_size(str2) ? cstring_size(str1) : cstring_size(str2);          \
            size_t idx__       = 0;                                                                                          \
            while (idx__ < min__ && (str1)[idx__] == (str2)[idx__]) {                                                        \
                ++idx__;                                                                                                     \
            }                                                                                                                \
            if ((str1)[idx__] == (str2)[idx__]) {                                                                            \
                (ret_order) = 0;                                                                                             \
            } else {                                                                                                         \
                const size_t tp_mask__ = (sizeof(size_t) <= sizeof(*(str1)))                                                 \
                                             ? (size_t)-1                                                                    \
                                             : (((size_t)1 << (sizeof(*(str1)) * CHAR_BIT)) - 1);                            \
                (ret_order)            = ((size_t)(str1)[idx__] & tp_mask__) < ((size_t)(str2)[idx__] & tp_mask__) ? -1 : 1; \
            }                                                                                                                \
        }                                                                                                                    \
    } while (0)

/**
 * @brief cstring_starts_with - Check if the string begins with the given prefix.
 * @param str       - The cstring.
 * @param ptr       - Pointer to the first character of the prefix.
 * @param count     - Length of the prefix.
 * @param ret_found - Variable of type `int` that receives the result of the
 *                    check. <br>
 *                      0 if the prefix was not found <br>
 *                      1 if the prefix was found
 * @return void
 */
#define cstring_starts_with(str, ptr, count, ret_found)                            \
    do {                                                                           \
        const void *const tmp_p__ = (const void *)(ptr);                           \
        const ptrdiff_t sub_sz__  = (ptrdiff_t)(count);                            \
        (ret_found)               = 0;                                             \
        if (tmp_p__ && sub_sz__ > 0 && sub_sz__ <= (ptrdiff_t)cstring_size(str)) { \
            str_n_eq_((str), (ptr), sub_sz__, (ret_found));                        \
        }                                                                          \
    } while (0)

/**
 * @brief cstring_ends_with - Check if the string ends with the given suffix.
 * @param str       - The cstring.
 * @param ptr       - Pointer to the first character of the suffix.
 * @param count     - Length of the suffix.
 * @param ret_found - Variable of type `int` that receives the result of the
 *                    check. <br>
 *                      0 if the suffix was not found <br>
 *                      1 if the suffix was found
 * @return void
 */
#define cstring_ends_with(str, ptr, count, ret_found)                                                   \
    do {                                                                                                \
        const void *const tmp_p___ = (const void *)(ptr);                                               \
        const ptrdiff_t sub_sz___  = (ptrdiff_t)(count);                                                \
        (ret_found)                = 0;                                                                 \
        if (tmp_p___ && sub_sz___ > 0 && sub_sz___ <= (ptrdiff_t)cstring_size(str)) {                   \
            str_n_eq_((str) + (ptrdiff_t)cstring_size(str) - sub_sz___, (ptr), sub_sz___, (ret_found)); \
        }                                                                                               \
    } while (0)

/**
 * @brief cstring_ends_with - Check if the string contains the given substring.
 * @param str       - The cstring.
 * @param ptr       - Pointer to the first character of the substring.
 * @param count     - Length of the substring.
 * @param ret_found - Variable of type `int` that receives the result of the
 *                    check. <br>
 *                      0 if the substring was not found <br>
 *                      1 if the substring was found
 * @return void
 */
#define cstring_contains(str, ptr, count, ret_found)   \
    do {                                               \
        ptrdiff_t off__;                               \
        cstring_find((str), 0, (ptr), (count), off__); \
        (ret_found) = off__ >= 0;                      \
    } while (0)

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

/** @} */

/* ========================== */
/* === EXTENDED INTERFACE === */
/* ========================== */

/**
 * @defgroup cstring_array_api The cstring_array API
 * @{
 */

/* ------------------------- */
/* --- vector of cstring --- */

/**
 * @brief cstring_array_type - The vector type of the data provided by
 * `cstring_split()`.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array_type(type) type **

/**
 * @brief cstring_array - Syntactic sugar to retrieve a vector type.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array(type) cstring_array_type(type)

/**
 * @brief cstring_array_iterator - The iterator type used for a vector.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array_iterator(type) cstring_array_type(type)

/**
 * @brief cstring_split - Tokenize a cstring into a cstring_array vector.
 * @param str       - The cstring.
 * @param max_tok   - Maximum number of tokens to be created. -1 specifies that
 *                    all tokens are created.
 * @param ptr       - Pointer to the first character of the delimiter string
 *                    that separates the tokens in `str`.
 * @param count     - Number of consecutive characters to be used as delimiter.
 * @param ret_array - Variable of cstring_array(type) that receives the created
 *                    vector. Can be a NULL vector. <br>
 *                    If `ret_array` refers to an existing vector, the old
 *                    content is overwritten.
 * @return void
 */
#define cstring_split(str, max_tok, ptr, count, ret_array)                                              \
    do {                                                                                                \
        const void *const check__   = (const void *)(ptr);                                              \
        const ptrdiff_t search_sz__ = (ptrdiff_t)(count);                                               \
        if (ret_array) {                                                                                \
            cstring_array_free(ret_array);                                                              \
        }                                                                                               \
        if ((str) && (max_tok) && (ptrdiff_t)(max_tok) > -2 && check__ && search_sz__ > 0) {            \
            ptrdiff_t found__;                                                                          \
            ptrdiff_t begin__            = 0;                                                           \
            size_t s_cnt__               = 0;                                                           \
            const size_t tok_minus_one__ = (size_t)(max_tok) - (size_t)1;                               \
            cstring_grow_((ret_array), 64);                                                             \
            cstring_set_ttl_siz_((ret_array), 1);                                                       \
            cstring_find((str), begin__, (ptr), search_sz__, found__);                                  \
            for (; found__ != -1 && s_cnt__ < tok_minus_one__; ++s_cnt__) {                             \
                cstring_push_back((ret_array), NULL);                                                   \
                cstring_assign((ret_array)[s_cnt__], (str) + begin__, found__ - begin__);               \
                begin__ = found__ + search_sz__;                                                        \
                cstring_find((str), begin__, (ptr), search_sz__, found__);                              \
            }                                                                                           \
            cstring_push_back((ret_array), NULL);                                                       \
            cstring_assign((ret_array)[s_cnt__], (str) + begin__, cstring_size(str) - (size_t)begin__); \
            (ret_array)[s_cnt__ + 1] = NULL;                                                            \
        }                                                                                               \
    } while (0)

/**
 * @brief cstring_array_free - Recursively free all memory associated with the
 *        cstring_array and set it to NULL.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return void
 */
#define cstring_array_free(arr)                         \
    do {                                                \
        const size_t strings___ = cstring_size(arr);    \
        size_t string_i__       = 0;                    \
        for (; string_i__ < strings___; ++string_i__) { \
            cstring_free((arr)[string_i__]);            \
        }                                               \
        cstring_free(arr);                              \
    } while (0)

/**
 * @brief cstring_array_at - Return the string pointer at position `pos` in
 *        the cstring_array.
 * @param arr - The cstring_array.
 * @param pos - Position of a string in the vector.
 * @return A string pointer at the specified position in the vector or NULL.
 */
#define cstring_array_at(arr, pos) \
    (cstring_at((arr), (pos)) ? *cstring_at((arr), (pos)) : NULL)

/**
 * @brief cstring_array_front - Return the string pointer to the first string in
 *        the cstring_array.
 * @details Unlike member cstring_array_begin, which returns an iterator to this
 *          same string, this function returns a direct string pointer.
 * @param arr - The cstring_array.
 * @return A string pointer to the first string in the vector or NULL.
 */
#define cstring_array_front(arr) \
    (cstring_front(arr) ? *cstring_front(arr) : NULL)

/**
 * @brief cstring_array_back - Return the string pointer to the last string in
 *        the cstring_array.
 * @details Unlike member cstring_array_begin, which returns an iterator just
 *          past this string, this function returns a direct string pointer.
 * @param arr - The cstring_array.
 * @return A string pointer to the last string in the vector or NULL.
 */
#define cstring_array_back(arr) \
    (cstring_back(arr) ? *cstring_back(arr) : NULL)

/**
 * @brief cstring_array_begin - Return an iterator to first string of the
 *        vector.
 * @param arr - The cstring_array.
 * @return A iterator to the first string (or NULL).
 */
#define cstring_array_begin(arr) \
    cstring_begin(arr)

/**
 * @brief cstring_array_end - Return an iterator to one past the last string of
 *        the vector.
 * @param arr - The cstring_array.
 * @return A pointer to one past the last string (or NULL).
 */
#define cstring_array_end(arr) \
    cstring_end(arr)

/**
 * @brief cstring_array_empty - Return non-zero if the vector is empty.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return Non-zero if `arr` is NULL or empty, zero if non-empty.
 */
#define cstring_array_empty(arr) \
    cstring_empty(arr)

/**
 * @brief cstring_array_size - Get the current length of the vector.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return The length as a `size_t`, terminating NULL not counted. Zero if `arr`
 *         is NULL.
 */
#define cstring_array_size(arr) \
    cstring_size(arr)

/**
 * @brief cstring_array_max_size - Get the maximum number of elements a vector
 *        of strings of the specified character type is able to hold.
 * @note The resulting value is technically possible. However, typically
 *       allocations of such a big size will fail.
 * @param type - The character type of strings in the vector to act on.
 * @return The maximum number of elements the vector is able to hold.
 */
#define cstring_array_max_size(type) \
    cstring_max_size(type *)

/**
 * @brief cstring_array_reserve - Request that the vector capacity be at least
 *        enough to contain `n` strings.
 * @details If `n` is greater than the current vector capacity, the function
 *          causes the container to reallocate its storage increasing its
 *          capacity to `n` (or greater).
 * @param arr - The cstring_array. Can be a NULL vector.
 * @param n   - Minimum capacity for the vector.
 * @return void
 */
#define cstring_array_reserve(arr, n)                               \
    do {                                                            \
        const int is_new___ = ((arr) == NULL);                      \
        if (is_new___ || cstring_ttl_cap_(arr) < (size_t)(n) + 1) { \
            cstring_grow_((arr), (size_t)(n) + 1);                  \
        }                                                           \
        if (is_new___) {                                            \
            cstring_set_ttl_siz_((arr), 1);                         \
            (arr)[0] = NULL;                                        \
        }                                                           \
    } while (0)

/**
 * @brief cstring_array_capacity - Get the current capacity of the vector.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return The capacity as a `size_t`. Zero if `arr` is NULL.
 */
#define cstring_array_capacity(arr) \
    cstring_capacity(arr)

/**
 * @brief cstring_array_shrink_to_fit - Request the container to reduce its
 *        capacity to fit its size.
 * @param arr - The cstring_array.
 * @return void
 */
#define cstring_array_shrink_to_fit(arr) \
    cstring_shrink_to_fit(arr)

/**
 * @brief cstring_array_clear - Erase all of the strings in the vector.
 * @param arr - The cstring_array.
 * @return void
 */
#define cstring_array_clear(arr)                               \
    do {                                                       \
        if (arr) {                                             \
            const size_t strings____ = cstring_size(arr);      \
            size_t string_i___       = 0;                      \
            for (; string_i___ < strings____; ++string_i___) { \
                cstring_free((arr)[string_i___]);              \
            }                                                  \
            cstring_set_ttl_siz_((arr), 1);                    \
            (arr)[0] = NULL;                                   \
        }                                                      \
    } while (0)

/**
 * @brief cstring_array_insert - Insert a string at position `pos` into the
 *        vector.
 * @param arr   - The cstring_array.
 * @param pos   - Position in the vector where the new string is inserted.
 * @param ptr   - Pointer to the first character of the string inserted into the
 *                cstring_array.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_insert(arr, pos, ptr, count)                                                                                              \
    do {                                                                                                                                        \
        if (arr) {                                                                                                                              \
            const size_t new_ttl_sz___ = cstring_ttl_siz_(arr) + 1;                                                                             \
            if (cstring_ttl_cap_(arr) < new_ttl_sz___) {                                                                                        \
                cstring_grow_((arr), new_ttl_sz___);                                                                                            \
            }                                                                                                                                   \
            if ((size_t)(pos) < cstring_size(arr)) {                                                                                            \
                cstring_clib_memmove((arr) + (size_t)(pos) + 1, (arr) + (size_t)(pos), sizeof(*(arr)) * ((cstring_size(arr)) - (size_t)(pos))); \
            }                                                                                                                                   \
            (arr)[(pos)] = NULL;                                                                                                                \
            cstring_assign((arr)[(pos)], ptr, count);                                                                                           \
            cstring_set_ttl_siz_((arr), new_ttl_sz___);                                                                                         \
            (arr)[cstring_size(arr)] = NULL;                                                                                                    \
        }                                                                                                                                       \
    } while (0)

/**
 * @brief cstring_array_erase - Remove the strings beginning at offset `pos`
 *        from the cstring_array.
 * @param arr - The cstring_array.
 * @param pos - Offset of the first string erased from the cstring_array.
 * @param n   - Number of consecutive strings to be erased.
 * @return void
 */
#define cstring_array_erase(arr, pos, n)                                                                                                                 \
    do {                                                                                                                                                 \
        if ((arr) && (size_t)(pos) < cstring_size(arr)) {                                                                                                \
            const size_t cs_count____ = (size_t)(pos) + (size_t)(n) >= cstring_size(arr) ? cstring_size(arr) - (size_t)(pos) : (size_t)(n);              \
            size_t arr_i__            = (size_t)(pos);                                                                                                   \
            for (; arr_i__ < (size_t)(pos) + cs_count____; ++arr_i__) {                                                                                  \
                cstring_free((arr)[arr_i__]);                                                                                                            \
            }                                                                                                                                            \
            cstring_set_ttl_siz_((arr), cstring_ttl_siz_(arr) - cs_count____);                                                                           \
            cstring_clib_memmove((arr) + (size_t)(pos), (arr) + (size_t)(pos) + cs_count____, sizeof(*(arr)) * (cstring_ttl_siz_(arr) - (size_t)(pos))); \
        }                                                                                                                                                \
    } while (0)

/**
 * @brief cstring_array_push_back - Add a string to the end of the vector.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param ptr   - Pointer to the first character of the string added to the
 *                cstring_array.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_push_back(arr, ptr, count)                    \
    do {                                                            \
        size_t new_ttl_siz____;                                     \
        if (!(arr)) {                                               \
            cstring_grow_((arr), 2);                                \
            cstring_set_ttl_siz_((arr), 1);                         \
        }                                                           \
        new_ttl_siz____ = cstring_ttl_siz_(arr) + 1;                \
        if (cstring_ttl_cap_(arr) < new_ttl_siz____) {              \
            cstring_grow_((arr), new_ttl_siz____);                  \
        }                                                           \
        cstring_set_ttl_siz_((arr), new_ttl_siz____);               \
        (arr)[new_ttl_siz____ - 2] = NULL;                          \
        cstring_assign((arr)[new_ttl_siz____ - 2], (ptr), (count)); \
        (arr)[new_ttl_siz____ - 1] = NULL;                          \
    } while (0)

/**
 * @brief cstring_array_pop_back - Remove the last string from the
 *        cstring_array.
 * @param arr - The cstring_array.
 * @return void
 */
#define cstring_array_pop_back(arr)                         \
    do {                                                    \
        if (cstring_size(arr)) {                            \
            cstring_set_ttl_siz_((arr), cstring_size(arr)); \
            cstring_free((arr)[cstring_size(arr)]);         \
        }                                                   \
    } while (0)

/**
 * @brief cstring_array_copy - Copy a cstring_array.
 * @param from - The original cstring_array.
 * @param to   - Destination to which the cstring_array is copied. Can be a NULL
 *               vector.
 * @return void
 */
#define cstring_array_copy(from, to)                                    \
    do {                                                                \
        if (from) {                                                     \
            const size_t from_ttl_sz___ = cstring_ttl_siz_(from);       \
            size_t string_i____         = 0;                            \
            if (to) {                                                   \
                cstring_array_free(to);                                 \
            }                                                           \
            cstring_grow_((to), from_ttl_sz___);                        \
            for (; string_i____ < from_ttl_sz___ - 1; ++string_i____) { \
                (to)[string_i____] = NULL;                              \
                cstring_copy((from)[string_i____], (to)[string_i____]); \
            }                                                           \
            (to)[string_i____] = NULL;                                  \
            cstring_set_ttl_siz_((to), from_ttl_sz___);                 \
        }                                                               \
    } while (0)

/**
 * @brief cstring_array_resize - Resize the container to contain `count`
 *          strings.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param n     - New size of the cstring_array.
 * @param ptr   - Pointer to the first character of the strings added if the
 *                container grows.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_resize(arr, n, ptr, count)                  \
    do {                                                          \
        const size_t cs_sz_count___ = (size_t)(n);                \
        size_t cs_sz____            = cstring_size(arr);          \
        if (cs_sz_count___ > cs_sz____) {                         \
            cstring_grow_((arr), cs_sz_count___ + 1);             \
            do {                                                  \
                (arr)[cs_sz____] = NULL;                          \
                cstring_assign((arr)[cs_sz____], (ptr), (count)); \
            } while (++cs_sz____ < cs_sz_count___);               \
            (arr)[cs_sz____] = NULL;                              \
        } else if (cs_sz_count___ < cs_sz____) {                  \
            while (cs_sz____-- > cs_sz_count___) {                \
                cstring_free(arr[cs_sz____]);                     \
            }                                                     \
        }                                                         \
        cstring_set_ttl_siz_((arr), cs_sz_count___ + 1);          \
    } while (0)

/**
 * @brief cstring_array_swap - Exchange the content of the cstring_array by the
 *        content of another cstring_array of the same type.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param other - The other cstring_array to swap content with. Can be a NULL
 *                vector.
 * @return void
 */
#define cstring_array_swap(arr, other) \
    cstring_swap((arr), (other))

/**
 * @brief cstring_array_join - Concatenate the strings of a vector using the
 *        specified joiner.
 * @param arr     - The cstring_array.
 * @param ptr     - Pointer to the first character of the string joining the
 *                  elements of the cstring_array. Can be NULL.
 * @param count   - Number of consecutive characters to be used. Can be zero.
 * @param ret_str - A cstring variable of the same character type as `arr` that
 *                  receives the joined string. <br>
 *                  If `ret_str` refers to an existing cstring, the old content
 *                  is overwritten.
 * @return void
 */
#define cstring_array_join(arr, ptr, count, ret_str)                                     \
    do {                                                                                 \
        const void *const check___ = (const void *)(ptr);                                \
        const size_t arr_siz__     = cstring_size(arr);                                  \
        if (arr_siz__) {                                                                 \
            size_t arr_idx__;                                                            \
            const size_t joiner_cnt__ = check___ ? (size_t)(count) : (size_t)0;          \
            size_t str_siz__          = (arr_siz__ - 1) * joiner_cnt__;                  \
            for (arr_idx__ = 0; arr_idx__ < arr_siz__; ++arr_idx__) {                    \
                str_siz__ += cstring_size(arr[arr_idx__]);                               \
            }                                                                            \
            if (str_siz__ > cstring_capacity(ret_str)) {                                 \
                cstring_free(ret_str);                                                   \
                cstring_reserve((ret_str), str_siz__);                                   \
            }                                                                            \
            cstring_assign((ret_str), arr[0], cstring_size(arr[0]));                     \
            for (arr_idx__ = 1; arr_idx__ < arr_siz__; ++arr_idx__) {                    \
                if (joiner_cnt__) {                                                      \
                    cstring_append((ret_str), (ptr), joiner_cnt__);                      \
                }                                                                        \
                cstring_append((ret_str), arr[arr_idx__], cstring_size(arr[arr_idx__])); \
            }                                                                            \
        } else {                                                                         \
            cstring_reserve((ret_str), 0);                                               \
            cstring_clear(str);                                                          \
        }                                                                                \
    } while (0)

/** @} */

/* ========================== */
/* === INTERNAL INTERFACE === */
/* ========================== */

/** @cond INTERNAL */

/**
 * @defgroup internal_api The private API, for internal use only
 * @{
 */

#include <limits.h>
#include <stddef.h>

/* in case C library functions need extra protection, allow these defines to be overridden. */
/* functions for allocation and deallocation need to correspond to each other, fall back to C library functions if not all are overridden */
#if !defined(cstring_clib_free) || !defined(cstring_clib_malloc) || !defined(cstring_clib_realloc)
#ifdef cstring_clib_free
#undef cstring_clib_free
#endif
#ifdef cstring_clib_malloc
#undef cstring_clib_malloc
#endif
#ifdef cstring_clib_realloc
#undef cstring_clib_realloc
#endif
#include <stdlib.h>
#define cstring_clib_free free
#define cstring_clib_malloc malloc
#define cstring_clib_realloc realloc
#endif
/* functions independent of memory allocation */
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
 * @brief str_n_eq_ - For internal use, check the equality of two string
 *                    sequences.
 * @param s1     - Pointer to the first string sequence.
 * @param s2     - Pointer to the second string sequence.
 * @param n      - The number of characters to compare.
 * @param ret_eq - Variable of type `int` that is set either to 1 if the
 *                 sequences are equal, or to 0 otherwise.
 * @return void
 */
#define str_n_eq_(s1, s2, n, ret_eq)                                 \
    do {                                                             \
        ptrdiff_t n___ = (ptrdiff_t)(n) - (ptrdiff_t)1;              \
        while (((ret_eq) = (s1)[n___] == (s2)[n___]) && --n___ >= 0) \
            ;                                                        \
    } while (0)

/**
 * @brief find_first_char_ - For internal use, find the first character equal or
 *        not equal (depending on `not_eq`) to the character pointed to by
 *        `pchar`.
 * @param not_eq     - 0 to find the first occurrence that equals the character,
 *                     1 to find the first occurrence that does not equal the
 *                     character
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param pchar      - Pointer to the character to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the found character or -1 if no such character is
 *                     found.
 * @return void
 */
#define find_first_char_(not_eq, str, pos, pchar, ret_offset)                                              \
    do {                                                                                                   \
        for ((ret_offset) = (ptrdiff_t)(pos);                                                              \
             (ret_offset) < (ptrdiff_t)cstring_size(str) && ((str)[(ret_offset)] == *(pchar)) == (not_eq); \
             ++(ret_offset))                                                                               \
            ;                                                                                              \
        if ((ret_offset) == (ptrdiff_t)cstring_size(str)) {                                                \
            (ret_offset) = (ptrdiff_t)-1;                                                                  \
        }                                                                                                  \
    } while (0)

/**
 * @brief find_last_char_ - For internal use, find the last character equal or
 *        not equal (depending on `not_eq`) to the character pointed to by
 *        `pchar`.
 * @param not_eq     - 0 to find the first occurrence that equals the character,
 *                     1 to find the first occurrence that does not equal the
 *                     character
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param pchar      - Pointer to the character to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the found character or -1 if no such character is
 *                     found.
 * @return void
 */
#define find_last_char_(not_eq, str, pos, pchar, ret_offset)                                \
    do {                                                                                    \
        for ((ret_offset) = (ptrdiff_t)(pos);                                               \
             (ret_offset) > (ptrdiff_t)-1 && ((str)[(ret_offset)] == *(pchar)) == (not_eq); \
             --(ret_offset))                                                                \
            ;                                                                               \
    } while (0)

/**
 * @brief make_charmask_ - For internal use, create a bit mask that helps to
 *        avoid the complexity of find_first_of_ and find_last_of_ being always
 *        O(n*m).
 * @param ptr    - Pointer to the first character of the character sequence.
 * @param count  - Length of the character sequence.
 * @param chmask - Variable of type `size_t` that receives the created bit mask.
 * @return void
 */
#define make_charmask_(ptr, count, chmask)                          \
    do {                                                            \
        const size_t type_mask__ =                                  \
            (sizeof(size_t) <= sizeof(*(ptr)))                      \
                ? (size_t)-1                                        \
                : (((size_t)1 << (sizeof(*(ptr)) * CHAR_BIT)) - 1); \
        ptrdiff_t ch_idx__ = 0;                                     \
        (chmask)           = 0;                                     \
        while (ch_idx__ < (count)) {                                \
            (chmask) |= (size_t)(ptr)[ch_idx__++];                  \
        }                                                           \
        (chmask) = (~(chmask)) & type_mask__;                       \
    } while (0)

/**
 * @brief find_first_of_ - For internal use, find the first character equal to
 *        one or none (depending on `not_of`) of the characters in the given
 *        character sequence.
 * @param not_of     - 0 for find_first_of behavior, 1 for find_first_not_of
 *                     behavior.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define find_first_of_(not_of, str, pos, ptr, count, ret_offset)                                     \
    do {                                                                                             \
        const void *const chk__      = (const void *)(ptr);                                          \
        const ptrdiff_t search_siz__ = (ptrdiff_t)(count), str_siz__ = (ptrdiff_t)cstring_size(str); \
        if (!chk__ || (ptrdiff_t)(pos) < 0 || str_siz__ <= (ptrdiff_t)(pos) || search_siz__ <= 0) {  \
            (ret_offset) = (ptrdiff_t)-1;                                                            \
        } else if (search_siz__ == 1) {                                                              \
            find_first_char_((not_of), (str), (pos), (ptr), (ret_offset));                           \
        } else {                                                                                     \
            size_t ch_mask__;                                                                        \
            ptrdiff_t search_off__;                                                                  \
            ptrdiff_t str_off__ = (ptrdiff_t)(pos);                                                  \
            make_charmask_((ptr), search_siz__, ch_mask__);                                          \
            for (; str_off__ < str_siz__; ++str_off__) {                                             \
                if ((size_t)(str)[str_off__] & ch_mask__) {                                          \
                    continue;                                                                        \
                }                                                                                    \
                for (search_off__ = 0;                                                               \
                     search_off__ < search_siz__ && (str)[str_off__] != (ptr)[search_off__];         \
                     ++search_off__)                                                                 \
                    ;                                                                                \
                if ((search_off__ == search_siz__) == (not_of)) {                                    \
                    break;                                                                           \
                }                                                                                    \
            }                                                                                        \
            (ret_offset) = (str_off__ == str_siz__) ? (ptrdiff_t)-1 : str_off__;                     \
        }                                                                                            \
    } while (0)

/**
 * @brief find_last_of_ - For internal use, find the last character equal to
 *        one or none (depending on `not_of`) of the characters in the given
 *        character sequence.
 * @param not_of     - 0 for find_last_of behavior, 1 for find_last_not_of
 *                     behavior.
 * @param str        - The cstring.
 * @param pos        - Position at which to begin searching.
 * @param ptr        - Pointer to the first character of the string identifying
 *                     characters to search for.
 * @param count      - Length of the string of characters to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first found character or -1 if no such character
 *                     is found.
 * @return void
 */
#define find_last_of_(not_of, str, pos, ptr, count, ret_offset)                                                      \
    do {                                                                                                             \
        const void *const chk___      = (const void *)(ptr);                                                         \
        const ptrdiff_t search_siz___ = (ptrdiff_t)(count);                                                          \
        ptrdiff_t str_off___          = ((ptrdiff_t)(pos) == -1 || (ptrdiff_t)(pos) >= (ptrdiff_t)cstring_size(str)) \
                                            ? (ptrdiff_t)cstring_size(str) - 1                                       \
                                            : (ptrdiff_t)(pos);                                                      \
        if (!chk___ || (ptrdiff_t)(pos) < -1 || str_off___ < 0 || search_siz___ <= 0) {                              \
            (ret_offset) = (ptrdiff_t)-1;                                                                            \
        } else if (search_siz___ == 1) {                                                                             \
            find_last_char_((not_of), (str), (pos), (ptr), (ret_offset));                                            \
        } else {                                                                                                     \
            size_t ch_mask___;                                                                                       \
            ptrdiff_t search_off___;                                                                                 \
            make_charmask_((ptr), search_siz___, ch_mask___);                                                        \
            for (; str_off___ >= 0; --str_off___) {                                                                  \
                if ((size_t)(str)[str_off___] & ch_mask___) {                                                        \
                    continue;                                                                                        \
                }                                                                                                    \
                for (search_off___ = 0;                                                                              \
                     search_off___ < search_siz___ && (str)[str_off___] != (ptr)[search_off___];                     \
                     ++search_off___)                                                                                \
                    ;                                                                                                \
                if ((search_off___ == search_siz___) == (not_of)) {                                                  \
                    break;                                                                                           \
                }                                                                                                    \
            }                                                                                                        \
            (ret_offset) = str_off___;                                                                               \
        }                                                                                                            \
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

/** @} */

/** @endcond */

#endif /* header guard */
