#ifndef HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_3
#define HEADER_CSTRING_8C60CBA8_34A6_4EA3_94B1_A69444BA0B9C_1_3
/**
 * @copyright Copyright (c) 2025 Steffen Illhardt,
 *            Licensed under the MIT license
 *            ( https://opensource.org/license/mit/ ).
 * @brief     cstring - Heap implemented using C library allocation functions.
 * @file      cstring.h
 * @version   1.3
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
 * @param type - The character type of the string to act on.
 */
#define cstring_string_type(type) \
    type *

/**
 * @brief cstring - Syntactic sugar to retrieve a string type.
 * @param type - The character type of the string to act on.
 */
#define cstring(type) \
    cstring_string_type(type)

/**
 * @brief cstring_iterator - The iterator type used for a string.
 * @param type - The character type of the iterator to act on.
 */
#define cstring_iterator(type) \
    cstring_string_type(type)

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
#define cstring_init(name, type)                                         \
    cstring_string_type(type) name = NULL;                               \
    do {                                                                 \
        pvt_grow_(name, ((64 - sizeof(pvt_metadata_t)) / sizeof(type))); \
        (name)[0] = 0;                                                   \
        pvt_set_ttl_siz_(name, 1);                                       \
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
#define cstring_assign(str, ptr, count)                                      \
    do {                                                                     \
        const void *const chk_s00__ = (const void *)(ptr);                   \
        if ((str) && pvt_ttl_cap_(str) < (size_t)(count) + 1) {              \
            cstring_free(str);                                               \
        }                                                                    \
        if (!(str)) {                                                        \
            pvt_grow_((str), (size_t)(count) + 1);                           \
        }                                                                    \
        if (chk_s00__ && (count)) {                                          \
            pvt_clib_memcpy((str), (ptr), (size_t)(count) * sizeof(*(ptr))); \
            pvt_set_ttl_siz_((str), (size_t)(count) + 1);                    \
            (str)[(size_t)(count)] = 0;                                      \
        } else {                                                             \
            pvt_set_ttl_siz_((str), 1);                                      \
            (str)[0] = 0;                                                    \
        }                                                                    \
    } while (0)

/**
 * @brief cstring_free - Free all memory associated with the cstring and set it
 *                       to NULL.
 * @param str - The cstring. Can be a NULL string.
 * @return void
 */
#define cstring_free(str)                         \
    do {                                          \
        if (str) {                                \
            pvt_clib_free(pvt_dat_to_base_(str)); \
            (str) = NULL;                         \
        }                                         \
    } while (0)

/* ---------------------- */
/* --- element access --- */

/**
 * @brief cstring_at - Return the character at position `pos` in the cstring.
 * @param str - The cstring.
 * @param pos - Position of a character in the string.
 * @return The ASCII value (as `int`) of the character at the specified position
 *         in the string. If the macro fails, -1 is returned.
 */
#define cstring_at(str, pos) \
    (((size_t)(pos) < cstring_size(str)) ? (int)((size_t)(str)[(size_t)(pos)] & pvt_get_typemask_(str)) : -1)

/**
 * @brief cstring_front - Return the first character in the cstring.
 * @param str - The cstring.
 * @return The ASCII value (as `int`) of the first character in the string. If
 *         the macro fails, -1 is returned.
 */
#define cstring_front(str) \
    (cstring_size(str) ? (int)((size_t)(str)[0] & pvt_get_typemask_(str)) : -1)

/**
 * @brief cstring_back - Return the last character in the cstring.
 * @param str - The cstring.
 * @return The ASCII value (as `int`) of the last character in the string. If
 *         the macro fails, -1 is returned.
 */
#define cstring_back(str) \
    (cstring_size(str) ? (int)((size_t)(str)[cstring_size(str) - 1] & pvt_get_typemask_(str)) : -1)

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
 *                      string.
 * @param str - The cstring.
 * @return A pointer to one past the last character (or NULL).
 */
#define cstring_end(str) \
    ((str) ? (str) + cstring_size(str) : NULL)

/* ---------------- */
/* --- capacity --- */

/**
 * @brief cstring_empty - Return 1 if the string is empty.
 * @param str - The cstring. Can be a NULL string.
 * @return 1 if `str` is NULL or empty, 0 if non-empty.
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
    (pvt_ttl_siz_(str) ? pvt_ttl_siz_(str) - 1 : (size_t)0)

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
 *                           specified character type is able to hold.
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
    (((((size_t)1) << ((sizeof(ptrdiff_t) < sizeof(size_t) ? sizeof(ptrdiff_t) : sizeof(size_t)) * CHAR_BIT - 1)) - 1 - sizeof(pvt_metadata_t)) / sizeof(type) - 1)

/**
 * @brief cstring_reserve - Request that the string capacity be at least enough
 *                          to contain `n` characters.
 * @details If `n` is greater than the current string capacity, the function
 *          causes the container to reallocate its storage increasing its
 *          capacity to `n` (or greater). <br>
 *          Also see `cstring_init()`, `cstring_assign()`,
 *          `cstring_push_back()`, `cstring_append()`, `cstring_resize()`.
 * @param str - The cstring. Can be a NULL string.
 * @param n   - Minimum capacity for the string.
 * @return void
 */
#define cstring_reserve(str, n)                                   \
    do {                                                          \
        const int isnew_s01__ = ((str) == NULL);                  \
        if (isnew_s01__ || pvt_ttl_cap_(str) < (size_t)(n) + 1) { \
            pvt_grow_((str), (size_t)(n) + 1);                    \
        }                                                         \
        if (isnew_s01__) {                                        \
            pvt_set_ttl_siz_((str), 1);                           \
            (str)[0] = 0;                                         \
        }                                                         \
    } while (0)

/**
 * @brief cstring_capacity - Get the current capacity of the string.
 * @param str - The cstring. Can be a NULL string.
 * @return The capacity as a `size_t`. Zero if `str` is NULL.
 */
#define cstring_capacity(str) \
    (pvt_ttl_cap_(str) ? pvt_ttl_cap_(str) - 1 : (size_t)0)

/**
 * @brief cstring_shrink_to_fit - Request the container to reduce its capacity
 *                                to fit its size.
 * @param str - The cstring.
 * @return void
 */
#define cstring_shrink_to_fit(str)                      \
    do {                                                \
        if (str) {                                      \
            const size_t ttl_s02__ = pvt_ttl_siz_(str); \
            pvt_grow_((str), ttl_s02__);                \
        }                                               \
    } while (0)

/**
 * @brief cstring_unsafe_set_size - Set the size property to the specified
 *                                  value and add the string terminator
 *                                  accordingly.
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
#define cstring_unsafe_set_size(str, size)                                                                                 \
    do {                                                                                                                   \
        if (str) {                                                                                                         \
            const size_t newsiz_s03__ = (cstring_capacity(str) < (size_t)(size)) ? cstring_capacity(str) : (size_t)(size); \
            pvt_set_ttl_siz_((str), newsiz_s03__ + 1);                                                                     \
            (str)[newsiz_s03__] = 0;                                                                                       \
        }                                                                                                                  \
    } while (0)

/* ----------------- */
/* --- modifiers --- */

/**
 * @brief cstring_clear - Erase all of the characters in the string.
 * @param str - The cstring.
 * @return void
 */
#define cstring_clear(str)              \
    do {                                \
        if (str) {                      \
            pvt_set_ttl_siz_((str), 1); \
            (str)[0] = 0;               \
        }                               \
    } while (0)

/**
 * @brief cstring_insert - Insert a string at position `pos` into the cstring.
 * @param str   - The cstring.
 * @param pos   - Position in the string where the new characters are inserted.
 * @param ptr   - Pointer to the first character inserted into the cstring.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_insert(str, pos, ptr, count)                                                                                                           \
    do {                                                                                                                                               \
        const size_t oldttl_s04__ = pvt_ttl_siz_(str);                                                                                                 \
        if ((size_t)(pos) + 1 <= oldttl_s04__) {                                                                                                       \
            const size_t newttl_s04__ = oldttl_s04__ + (size_t)(count);                                                                                \
            if (pvt_ttl_cap_(str) < newttl_s04__) {                                                                                                    \
                pvt_grow_((str), newttl_s04__);                                                                                                        \
            }                                                                                                                                          \
            if ((size_t)(pos) < oldttl_s04__ - 1) {                                                                                                    \
                pvt_clib_memmove((str) + (size_t)(pos) + (size_t)(count), (str) + (size_t)(pos), sizeof(*(str)) * (oldttl_s04__ - 1 - (size_t)(pos))); \
            }                                                                                                                                          \
            pvt_clib_memcpy((str) + (size_t)(pos), (ptr), (size_t)(count) * sizeof(*(ptr)));                                                           \
            pvt_set_ttl_siz_((str), newttl_s04__);                                                                                                     \
            (str)[newttl_s04__ - 1] = 0;                                                                                                               \
        }                                                                                                                                              \
    } while (0)

/**
 * @brief cstring_erase - Remove the characters beginning at offset `pos` from
 *                        the cstring.
 * @param str - The cstring.
 * @param pos - Offset of the first character erased from the cstring.
 * @param n   - Number of consecutive characters to be erased.
 * @return void
 */
#define cstring_erase(str, pos, n)                                                                                                       \
    do {                                                                                                                                 \
        size_t ttl_s05__ = pvt_ttl_siz_(str);                                                                                            \
        if ((size_t)(pos) + 1 <= ttl_s05__) {                                                                                            \
            const size_t n_s05__ = ((size_t)(pos) + (size_t)(n) >= cstring_size(str)) ? cstring_size(str) - (size_t)(pos) : (size_t)(n); \
            ttl_s05__ -= n_s05__;                                                                                                        \
            pvt_set_ttl_siz_((str), ttl_s05__);                                                                                          \
            pvt_clib_memmove((str) + (size_t)(pos), (str) + (size_t)(pos) + n_s05__, sizeof(*(str)) * (ttl_s05__ - (size_t)(pos)));      \
        }                                                                                                                                \
    } while (0)

/**
 * @brief cstring_push_back - Add a character to the end of the string.
 * @details Also see `cstring_init()`, `cstring_assign()`, `cstring_reserve()`,
 *          `cstring_append()`, `cstring_resize()`.
 * @param str   - The cstring. Can be a NULL string.
 * @param value - The character to add.
 * @return void
 */
#define cstring_push_back(str, value)           \
    do {                                        \
        size_t newttl_s06__;                    \
        if (!(str)) {                           \
            pvt_grow_((str), 2);                \
            pvt_set_ttl_siz_((str), 1);         \
        }                                       \
        newttl_s06__ = pvt_ttl_siz_(str) + 1;   \
        if (pvt_ttl_cap_(str) < newttl_s06__) { \
            pvt_grow_((str), newttl_s06__);     \
        }                                       \
        pvt_set_ttl_siz_((str), newttl_s06__);  \
        (str)[newttl_s06__ - 2] = (value);      \
        (str)[newttl_s06__ - 1] = 0;            \
    } while (0)

/**
 * @brief cstring_pop_back - Remove the last character from the cstring.
 * @param str - The cstring.
 * @return void
 */
#define cstring_pop_back(str)                       \
    do {                                            \
        const size_t siz_s07__ = cstring_size(str); \
        if (siz_s07__) {                            \
            pvt_set_ttl_siz_((str), siz_s07__);     \
            (str)[siz_s07__ - 1] = 0;               \
        }                                           \
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
 *                          another string.
 * @param str   - The cstring.
 * @param pos   - Offset of the first character replaced in the cstring.
 * @param n     - Number of consecutive characters to be replaced.
 * @param ptr   - Pointer to the first replacement character.
 * @param count - Number of consecutive replacement characters to be used.
 * @return void
 */
#define cstring_replace(str, pos, n, ptr, count)                                                                                                                \
    do {                                                                                                                                                        \
        if ((ptrdiff_t)(count) >= 0 && (size_t)(pos) + 1 <= pvt_ttl_siz_(str)) {                                                                                \
            const size_t siz_s08__     = cstring_size(str);                                                                                                     \
            const size_t n_s08__       = ((size_t)(pos) + (size_t)(n) >= siz_s08__) ? siz_s08__ - (size_t)(pos) : (size_t)(n);                                  \
            const ptrdiff_t diff_s08__ = (ptrdiff_t)(count) - (ptrdiff_t)n_s08__;                                                                               \
            const size_t newttl_s08__  = (size_t)((ptrdiff_t)siz_s08__ + diff_s08__) + 1;                                                                       \
            if (newttl_s08__ > pvt_ttl_cap_(str)) {                                                                                                             \
                pvt_grow_((str), newttl_s08__);                                                                                                                 \
            }                                                                                                                                                   \
            if (diff_s08__) {                                                                                                                                   \
                pvt_clib_memmove((str) + (size_t)(pos) + (count), (str) + (size_t)(pos) + n_s08__, (siz_s08__ - n_s08__ - (size_t)(pos) + 1) * sizeof(*(str))); \
            }                                                                                                                                                   \
            pvt_clib_memcpy((str) + (size_t)(pos), (ptr), (size_t)(count) * sizeof(*(ptr)));                                                                    \
            pvt_set_ttl_siz_((str), newttl_s08__);                                                                                                              \
            (str)[newttl_s08__ - 1] = 0;                                                                                                                        \
        }                                                                                                                                                       \
    } while (0)

/**
 * @brief cstring_copy - Copy a cstring.
 * @param from - The original cstring.
 * @param to   - Destination to which the cstring is copied. Can be a NULL
 *               string. <br>
 *               If `to` refers to an existing cstring, the old content is
 *               overwritten.
 * @return void
 */
#define cstring_copy(from, to)                                              \
    do {                                                                    \
        if (from) {                                                         \
            const size_t fromttl_s09__ = pvt_ttl_siz_(from);                \
            if ((to) && pvt_ttl_cap_(to) < fromttl_s09__) {                 \
                cstring_free(to);                                           \
            }                                                               \
            if (!(to)) {                                                    \
                pvt_grow_((to), fromttl_s09__);                             \
            }                                                               \
            pvt_clib_memcpy((to), (from), fromttl_s09__ * sizeof(*(from))); \
            pvt_set_ttl_siz_((to), fromttl_s09__);                          \
        } else {                                                            \
            cstring_clear(to);                                              \
        }                                                                   \
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
#define cstring_resize(str, count, value)             \
    do {                                              \
        const size_t cnt_s10__ = (size_t)(count) + 1; \
        size_t siz_s10__       = cstring_size(str);   \
        if (cnt_s10__ > siz_s10__ + 1) {              \
            pvt_grow_((str), cnt_s10__);              \
            do {                                      \
                (str)[siz_s10__++] = (value);         \
            } while (siz_s10__ < cnt_s10__);          \
        }                                             \
        pvt_set_ttl_siz_((str), cnt_s10__);           \
        (str)[(size_t)(count)] = 0;                   \
    } while (0)

/**
 * @brief cstring_swap - Exchange the content of the cstring by the content of
 *                       another cstring of the same type.
 * @param str   - The original cstring. Can be a NULL string.
 * @param other - The other cstring to swap content with. Can be a NULL string.
 * @return void
 */
#define cstring_swap(str, other)         \
    do {                                 \
        void *tmp_s11__ = (void *)(str); \
        (str)           = (other);       \
        (other)         = tmp_s11__;     \
    } while (0)

/**
 * @brief cstring_trim - Remove contiguous occurrences of the specified
 *                       character from the begin and/or the end of a cstring.
 * @param str   - The cstring.
 * @param value - The character to be removed.
 * @param mode  - Flags specifying where the characters are removed. <br>
 *                  1 to remove leading characters <br>
 *                  2 to remove trailing characters <br>
 *                Their combination (1|2) results in trimming on both sides of
 *                the string.
 * @return void
 */
#define cstring_trim(str, value, mode)                                                     \
    do {                                                                                   \
        size_t end_s12__ = cstring_size(str);                                              \
        if (end_s12__) {                                                                   \
            size_t newsiz_s12__;                                                           \
            const int dohead_s12__ = (int)((mode) & 1);                                    \
            const int dotail_s12__ = (int)((mode) & 2);                                    \
            size_t beg_s12__       = 0;                                                    \
            if (dotail_s12__) {                                                            \
                while (end_s12__ > beg_s12__) {                                            \
                    if ((str)[--end_s12__] != (value)) {                                   \
                        ++end_s12__;                                                       \
                        break;                                                             \
                    }                                                                      \
                }                                                                          \
            }                                                                              \
            if (dohead_s12__) {                                                            \
                while (beg_s12__ < end_s12__) {                                            \
                    if ((str)[beg_s12__] != (value)) {                                     \
                        break;                                                             \
                    }                                                                      \
                    ++beg_s12__;                                                           \
                }                                                                          \
            }                                                                              \
            newsiz_s12__ = end_s12__ - beg_s12__;                                          \
            if (newsiz_s12__ && beg_s12__) {                                               \
                pvt_clib_memmove((str), (str) + beg_s12__, sizeof(*(str)) * newsiz_s12__); \
            }                                                                              \
            pvt_set_ttl_siz_((str), newsiz_s12__ + 1);                                     \
            (str)[newsiz_s12__] = 0;                                                       \
        }                                                                                  \
    } while (0)

/**
 * @brief cstring_fix - Update the cstring to a fixed length by either padding
 *                      or shortening.
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
#define cstring_fix(str, length, value, mode)                                                                                               \
    do {                                                                                                                                    \
        if ((str) && (ptrdiff_t)(length) >= 0) {                                                                                            \
            const size_t siz_s13__     = cstring_size(str);                                                                                 \
            const ptrdiff_t diff_s13__ = (ptrdiff_t)(length) - (ptrdiff_t)siz_s13__;                                                        \
            if (diff_s13__) {                                                                                                               \
                const int dohead_s13__        = (int)((mode) & 1);                                                                          \
                const int dotail_s13__        = (int)((mode) & 2);                                                                          \
                const ptrdiff_t headsiz_s13__ = dohead_s13__ && dotail_s13__ ? diff_s13__ / 2 : (dohead_s13__ ? diff_s13__ : (ptrdiff_t)0); \
                if (diff_s13__ < 0) {                                                                                                       \
                    pvt_clib_memmove((str), (str) - (headsiz_s13__), (size_t)(length) * sizeof(*(str)));                                    \
                } else {                                                                                                                    \
                    if ((size_t)(length) + 1 > pvt_ttl_cap_(str)) {                                                                         \
                        pvt_grow_((str), (size_t)(length) + 1);                                                                             \
                    }                                                                                                                       \
                    if (dohead_s13__ && headsiz_s13__) {                                                                                    \
                        ptrdiff_t i_s13__ = 0;                                                                                              \
                        pvt_clib_memmove((str) + headsiz_s13__, (str), siz_s13__ * sizeof(*(str)));                                         \
                        while (i_s13__ < headsiz_s13__) {                                                                                   \
                            (str)[i_s13__++] = (value);                                                                                     \
                        }                                                                                                                   \
                    }                                                                                                                       \
                    if (dotail_s13__) {                                                                                                     \
                        ptrdiff_t i_s13__         = headsiz_s13__ + (ptrdiff_t)siz_s13__;                                                   \
                        const ptrdiff_t end_s13__ = (ptrdiff_t)(length);                                                                    \
                        while (i_s13__ < end_s13__) {                                                                                       \
                            (str)[i_s13__++] = (value);                                                                                     \
                        }                                                                                                                   \
                    }                                                                                                                       \
                }                                                                                                                           \
                pvt_set_ttl_siz_((str), (size_t)(length) + 1);                                                                              \
                (str)[(size_t)(length)] = L'\0';                                                                                            \
            }                                                                                                                               \
        }                                                                                                                                   \
    } while (0)

/**
 * @brief cstring_reverse - Reverse the character order in the cstring.
 * @param str - The cstring.
 * @return void
 */
#define cstring_reverse(str)                             \
    do {                                                 \
        const size_t tmp_s14__ = cstring_size(str);      \
        if (tmp_s14__ > 1) {                             \
            size_t end_s14__ = tmp_s14__;                \
            size_t beg_s14__ = 0;                        \
            while (end_s14__ > beg_s14__) {              \
                (str)[tmp_s14__]   = (str)[--end_s14__]; \
                (str)[end_s14__]   = (str)[beg_s14__];   \
                (str)[beg_s14__++] = (str)[tmp_s14__];   \
            }                                            \
            (str)[tmp_s14__] = 0;                        \
        }                                                \
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
#define cstring_find(str, pos, ptr, count, ret_offset)                    \
    do {                                                                  \
        const void *const chk_s15__ = (const void *)(ptr);                \
        size_t hs_s15__             = 0;                                  \
        if (chk_s15__ && (ptrdiff_t)(count) > 1) {                        \
            pvt_make_find_sub_hash_((ptr), (ptrdiff_t)(count), hs_s15__); \
        }                                                                 \
        pvt_find_((str), (pos), (ptr), (count), hs_s15__, (ret_offset));  \
    } while (0);

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
#define cstring_rfind(str, pos, ptr, count, ret_offset)                                                                       \
    do {                                                                                                                      \
        const void *const chk_s16__ = (const void *)(ptr);                                                                    \
        const ptrdiff_t cnt_s16__   = (ptrdiff_t)(count);                                                                     \
        const ptrdiff_t siz_s16__   = ((ptrdiff_t)(pos) == -1 || (ptrdiff_t)(pos) + cnt_s16__ > (ptrdiff_t)cstring_size(str)) \
                                          ? (ptrdiff_t)cstring_size(str)                                                      \
                                          : ((ptrdiff_t)(pos) + cnt_s16__);                                                   \
        if (!chk_s16__ || (ptrdiff_t)(pos) < -1 || cnt_s16__ > siz_s16__ || !siz_s16__ || cnt_s16__ <= 0) {                   \
            (ret_offset) = (ptrdiff_t)-1;                                                                                     \
        } else if (cnt_s16__ == siz_s16__) {                                                                                  \
            int eq_s16__;                                                                                                     \
            pvt_str_n_eq_((str), (ptr), cnt_s16__, eq_s16__);                                                                 \
            (ret_offset) = eq_s16__ ? (ptrdiff_t)0 : (ptrdiff_t)-1;                                                           \
        } else if (cnt_s16__ == 1) {                                                                                          \
            pvt_find_last_char_(0, (str), (pos), (ptr), (ret_offset));                                                        \
        } else {                                                                                                              \
            ptrdiff_t off_s16__;                                                                                              \
            const ptrdiff_t diff_s16__ = siz_s16__ - cnt_s16__;                                                               \
            size_t strhs_s16__ = 0, subhs_s16__ = 0, hsfactor_s16__ = 1;                                                      \
            static const size_t tpmsk_s16__ = pvt_get_typemask_(str);                                                         \
            (ret_offset)                    = (ptrdiff_t)-1;                                                                  \
            for (off_s16__ = cnt_s16__ - 1; off_s16__; --off_s16__) {                                                         \
                strhs_s16__ = (strhs_s16__ << 1) + ((size_t)(str)[off_s16__ + diff_s16__] & tpmsk_s16__);                     \
                subhs_s16__ = (subhs_s16__ << 1) + ((size_t)(ptr)[off_s16__] & tpmsk_s16__);                                  \
                hsfactor_s16__ <<= 1;                                                                                         \
            }                                                                                                                 \
            strhs_s16__ = (strhs_s16__ << 1) + ((size_t)(str)[off_s16__ + diff_s16__] & tpmsk_s16__);                         \
            subhs_s16__ = (subhs_s16__ << 1) + ((size_t)(ptr)[off_s16__] & tpmsk_s16__);                                      \
            off_s16__   = diff_s16__;                                                                                         \
            do {                                                                                                              \
                if (subhs_s16__ == strhs_s16__) {                                                                             \
                    int eq_s16__;                                                                                             \
                    pvt_str_n_eq_((str) + off_s16__, (ptr), cnt_s16__, eq_s16__);                                             \
                    if (eq_s16__) {                                                                                           \
                        (ret_offset) = off_s16__;                                                                             \
                        break;                                                                                                \
                    }                                                                                                         \
                }                                                                                                             \
                --off_s16__;                                                                                                  \
                strhs_s16__ =                                                                                                 \
                    ((strhs_s16__ - hsfactor_s16__ * ((size_t)(str)[off_s16__ + cnt_s16__] & tpmsk_s16__)) << 1) +            \
                    ((size_t)(str)[off_s16__] & tpmsk_s16__);                                                                 \
            } while (off_s16__);                                                                                              \
        }                                                                                                                     \
    } while (0)

/**
 * @brief cstring_find_first_of - Find the first character equal to one of the
 *                                characters in the given character sequence.
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
    pvt_find_first_of_(0, (str), (pos), (ptr), (count), (ret_offset))

/**
 * @brief cstring_find_first_not_of - Find the first character equal to none of
 *                                    the characters in the given character
 *                                    sequence.
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
    pvt_find_first_of_(1, (str), (pos), (ptr), (count), (ret_offset))

/**
 * @brief cstring_find_last_of - Find the last character equal to one of the
 *                               characters in the given character sequence.
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
    pvt_find_last_of_(0, (str), (pos), (ptr), (count), (ret_offset))

/**
 * @brief cstring_find_last_not_of - Find the last character equal to none of
 *                                   the characters in the given character
 *                                   sequence.
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
    pvt_find_last_of_(1, (str), (pos), (ptr), (count), (ret_offset))

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
#define cstring_compare(str1, str2, ret_order)                                                                                                  \
    do {                                                                                                                                        \
        if ((str1) && (str2)) {                                                                                                                 \
            const size_t siz_s17__ = (cstring_size(str1) < cstring_size(str2)) ? cstring_size(str1) : cstring_size(str2);                       \
            size_t i_s17__         = 0;                                                                                                         \
            while (i_s17__ < siz_s17__ && (str1)[i_s17__] == (str2)[i_s17__]) {                                                                 \
                ++i_s17__;                                                                                                                      \
            }                                                                                                                                   \
            if ((str1)[i_s17__] == (str2)[i_s17__]) {                                                                                           \
                (ret_order) = 0;                                                                                                                \
            } else {                                                                                                                            \
                static const size_t tpmsk_s17__ = pvt_get_typemask_(str1);                                                                      \
                (ret_order)                     = (((size_t)(str1)[i_s17__] & tpmsk_s17__) < ((size_t)(str2)[i_s17__] & tpmsk_s17__)) ? -1 : 1; \
            }                                                                                                                                   \
        }                                                                                                                                       \
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
#define cstring_starts_with(str, ptr, count, ret_found)                                      \
    do {                                                                                     \
        const void *const chk_s18__  = (const void *)(ptr);                                  \
        const ptrdiff_t subsiz_s18__ = (ptrdiff_t)(count);                                   \
        (ret_found)                  = 0;                                                    \
        if (chk_s18__ && subsiz_s18__ > 0 && subsiz_s18__ <= (ptrdiff_t)cstring_size(str)) { \
            pvt_str_n_eq_((str), (ptr), subsiz_s18__, (ret_found));                          \
        }                                                                                    \
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
#define cstring_ends_with(str, ptr, count, ret_found)                                                             \
    do {                                                                                                          \
        const void *const chk_s19__  = (const void *)(ptr);                                                       \
        const ptrdiff_t subsiz_s19__ = (ptrdiff_t)(count);                                                        \
        (ret_found)                  = 0;                                                                         \
        if (chk_s19__ && subsiz_s19__ > 0 && subsiz_s19__ <= (ptrdiff_t)cstring_size(str)) {                      \
            pvt_str_n_eq_((str) + (ptrdiff_t)cstring_size(str) - subsiz_s19__, (ptr), subsiz_s19__, (ret_found)); \
        }                                                                                                         \
    } while (0)

/**
 * @brief cstring_contains - Check if the string contains the given substring.
 * @param str       - The cstring.
 * @param ptr       - Pointer to the first character of the substring.
 * @param count     - Length of the substring.
 * @param ret_found - Variable of type `int` that receives the result of the
 *                    check. <br>
 *                      0 if the substring was not found <br>
 *                      1 if the substring was found
 * @return void
 */
#define cstring_contains(str, ptr, count, ret_found)       \
    do {                                                   \
        ptrdiff_t off_s20__;                               \
        cstring_find((str), 0, (ptr), (count), off_s20__); \
        (ret_found) = (off_s20__ >= 0);                    \
    } while (0)

/**
 * @brief cstring_substring - Copy a part of a string.
 * @param from - The source cstring.
 * @param pos  - Position in the source cstring where the substring begins.
 * @param n    - Number of consecutive characters copied to the substring.
 * @param to   - Destination to which the substring is copied. Can be a NULL
 *               string. <br>
 *               If `to` refers to an existing cstring, the old content is
 *               overwritten.
 * @return void
 */
#define cstring_substring(from, pos, n, to)                                                                                                     \
    do {                                                                                                                                        \
        if ((from) && pvt_ttl_siz_(from) > (size_t)(pos)) {                                                                                     \
            const size_t subsiz_s21__ = ((size_t)(pos) + (size_t)(n) >= cstring_size(from)) ? cstring_size(from) - (size_t)(pos) : (size_t)(n); \
            if ((to) && pvt_ttl_cap_(to) < subsiz_s21__ + 1) {                                                                                  \
                cstring_free(to);                                                                                                               \
            }                                                                                                                                   \
            if (!(to)) {                                                                                                                        \
                pvt_grow_((to), subsiz_s21__ + 1);                                                                                              \
            }                                                                                                                                   \
            pvt_clib_memcpy((to), (from) + (size_t)(pos), subsiz_s21__ * sizeof(*(from)));                                                      \
            pvt_set_ttl_siz_((to), subsiz_s21__ + 1);                                                                                           \
            (to)[subsiz_s21__] = 0;                                                                                                             \
        } else {                                                                                                                                \
            cstring_clear(to);                                                                                                                  \
        }                                                                                                                                       \
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
 * @brief cstring_array_type - The type of the data in the vector of strings.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array_type(type) \
    type **

/**
 * @brief cstring_array - Syntactic sugar to retrieve a vector type.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array(type) \
    cstring_array_type(type)

/**
 * @brief cstring_array_iterator - The iterator type used for a vector.
 * @param type - The character type of the strings in the vector.
 */
#define cstring_array_iterator(type) \
    cstring_array_type(type)

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
#define cstring_split(str, max_tok, ptr, count, ret_array)                                                  \
    do {                                                                                                    \
        const void *const chk_a00__ = (const void *)(ptr);                                                  \
        const ptrdiff_t cnt_a00__   = (ptrdiff_t)(count);                                                   \
        cstring_array_clear(ret_array);                                                                     \
        if ((str) && (max_tok) && (ptrdiff_t)(max_tok) > -2 && chk_a00__ && cnt_a00__ > 0) {                \
            ptrdiff_t off_a00__;                                                                            \
            ptrdiff_t beg_a00__       = 0;                                                                  \
            size_t i_a00__            = 0;                                                                  \
            size_t subhs_a00__        = 0;                                                                  \
            const size_t delcnt_a00__ = (size_t)(max_tok) - (size_t)1;                                      \
            cstring_array_reserve((ret_array), 63);                                                         \
            pvt_set_ttl_siz_((ret_array), 1);                                                               \
            if (cnt_a00__ > 1) {                                                                            \
                pvt_make_find_sub_hash_((ptr), cnt_a00__, subhs_a00__);                                     \
            }                                                                                               \
            pvt_find_((str), beg_a00__, (ptr), cnt_a00__, subhs_a00__, off_a00__);                          \
            for (; off_a00__ != -1 && i_a00__ < delcnt_a00__; ++i_a00__) {                                  \
                cstring_push_back((ret_array), NULL);                                                       \
                cstring_assign((ret_array)[i_a00__], (str) + beg_a00__, off_a00__ - beg_a00__);             \
                beg_a00__ = off_a00__ + cnt_a00__;                                                          \
                pvt_find_((str), beg_a00__, (ptr), cnt_a00__, subhs_a00__, off_a00__);                      \
            }                                                                                               \
            cstring_push_back((ret_array), NULL);                                                           \
            cstring_assign((ret_array)[i_a00__], (str) + beg_a00__, cstring_size(str) - (size_t)beg_a00__); \
            (ret_array)[i_a00__ + 1] = NULL;                                                                \
        }                                                                                                   \
    } while (0)

/**
 * @brief cstring_array_free - Recursively free all memory associated with the
 *                             cstring_array and set it to NULL.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return void
 */
#define cstring_array_free(arr)                              \
    do {                                                     \
        const size_t siz_a01__ = cstring_size(arr);          \
        size_t i_a01__         = 0;                          \
        for (; i_a01__ < siz_a01__; ++i_a01__) {             \
            pvt_clib_free(pvt_dat_to_base_((arr)[i_a01__])); \
        }                                                    \
        cstring_free(arr);                                   \
    } while (0)

/**
 * @brief cstring_array_at - Return the pointer to the first character of the
 *                           string at position `pos` in the cstring_array.
 * @param arr - The cstring_array.
 * @param pos - Position of a string in the vector.
 * @return A string pointer at the specified position in the vector or NULL.
 */
#define cstring_array_at(arr, pos) \
    ((size_t)(pos) < cstring_size(arr) ? (arr)[(size_t)(pos)] : NULL)

/**
 * @brief cstring_array_front - Return the pointer to the first character of the
 *                              first string in the cstring_array.
 * @details Unlike member cstring_array_begin, which returns an iterator to this
 *          same string, this function returns a direct string pointer.
 * @param arr - The cstring_array.
 * @return A string pointer to the first string in the vector or NULL.
 */
#define cstring_array_front(arr) \
    (cstring_size(arr) ? *(arr) : NULL)

/**
 * @brief cstring_array_back - Return the pointer to the first character of the
 *                             last string in the cstring_array.
 * @details Unlike member cstring_array_begin, which returns an iterator just
 *          past this string, this function returns a direct string pointer.
 * @param arr - The cstring_array.
 * @return A string pointer to the last string in the vector or NULL.
 */
#define cstring_array_back(arr) \
    (cstring_size(arr) ? (arr)[cstring_size(arr) - 1] : NULL)

/**
 * @brief cstring_array_begin - Return an iterator to first string of the
 *                              vector.
 * @param arr - The cstring_array.
 * @return An iterator to the first string (or NULL).
 */
#define cstring_array_begin(arr) \
    cstring_begin(arr)

/**
 * @brief cstring_array_end - Return an iterator to one past the last string of
 *                            the vector.
 * @param arr - The cstring_array.
 * @return An iterator to one past the last string (or NULL).
 */
#define cstring_array_end(arr) \
    cstring_end(arr)

/**
 * @brief cstring_array_empty - Return 1 if the vector is empty.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return 1 if `arr` is NULL or empty, 0 if non-empty.
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
 *                                 of strings of the specified character type is
 *                                 able to hold.
 * @note The resulting value is technically possible. However, typically
 *       allocations of such a big size will fail.
 * @param type - The character type of strings in the vector to act on.
 * @return The maximum number of elements the vector is able to hold.
 */
#define cstring_array_max_size(type) \
    cstring_max_size(type *)

/**
 * @brief cstring_array_reserve - Request that the vector capacity be at least
 *                                enough to contain `n` strings.
 * @details If `n` is greater than the current vector capacity, the function
 *          causes the container to reallocate its storage increasing its
 *          capacity to `n` (or greater).
 * @param arr - The cstring_array. Can be a NULL vector.
 * @param n   - Minimum capacity for the vector.
 * @return void
 */
#define cstring_array_reserve(arr, n) \
    cstring_reserve((arr), (n))

/**
 * @brief cstring_array_capacity - Get the current capacity of the vector.
 * @param arr - The cstring_array. Can be a NULL vector.
 * @return The capacity as a `size_t`. Zero if `arr` is NULL.
 */
#define cstring_array_capacity(arr) \
    cstring_capacity(arr)

/**
 * @brief cstring_array_shrink_to_fit - Request the container to reduce its
 *                                      capacity to fit its size.
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
#define cstring_array_clear(arr)                                 \
    do {                                                         \
        if (arr) {                                               \
            const size_t siz_a02__ = cstring_size(arr);          \
            size_t i_a02__         = 0;                          \
            for (; i_a02__ < siz_a02__; ++i_a02__) {             \
                pvt_clib_free(pvt_dat_to_base_((arr)[i_a02__])); \
            }                                                    \
            pvt_set_ttl_siz_((arr), 1);                          \
            (arr)[0] = NULL;                                     \
        }                                                        \
    } while (0)

/**
 * @brief cstring_array_insert - Insert a string at position `pos` into the
 *                               vector.
 * @param arr   - The cstring_array.
 * @param pos   - Position in the vector where the new string is inserted.
 * @param ptr   - Pointer to the first character of the string inserted into the
 *                cstring_array.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_insert(arr, pos, ptr, count)                                                                                       \
    do {                                                                                                                                 \
        if ((size_t)(pos) + 1 <= pvt_ttl_siz_(arr)) {                                                                                    \
            const size_t newttl_a03__ = pvt_ttl_siz_(arr) + 1;                                                                           \
            if (pvt_ttl_cap_(arr) < newttl_a03__) {                                                                                      \
                pvt_grow_((arr), newttl_a03__);                                                                                          \
            }                                                                                                                            \
            if ((size_t)(pos) < newttl_a03__ - 2) {                                                                                      \
                pvt_clib_memmove((arr) + (size_t)(pos) + 1, (arr) + (size_t)(pos), sizeof(*(arr)) * (newttl_a03__ - 2 - (size_t)(pos))); \
            }                                                                                                                            \
            (arr)[(pos)] = NULL;                                                                                                         \
            cstring_assign((arr)[(pos)], (ptr), (count));                                                                                \
            pvt_set_ttl_siz_((arr), newttl_a03__);                                                                                       \
            (arr)[newttl_a03__ - 1] = NULL;                                                                                              \
        }                                                                                                                                \
    } while (0)

/**
 * @brief cstring_array_erase - Remove the strings beginning at offset `pos`
 *                              from the cstring_array.
 * @param arr - The cstring_array.
 * @param pos - Offset of the first string erased from the cstring_array.
 * @param n   - Number of consecutive strings to be erased.
 * @return void
 */
#define cstring_array_erase(arr, pos, n)                                                                                                 \
    do {                                                                                                                                 \
        size_t ttl_a04__ = pvt_ttl_siz_(arr);                                                                                            \
        if ((size_t)(pos) + 1 <= ttl_a04__) {                                                                                            \
            const size_t n_a04__ = ((size_t)(pos) + (size_t)(n) >= cstring_size(arr)) ? cstring_size(arr) - (size_t)(pos) : (size_t)(n); \
            size_t i_a04__       = (size_t)(pos);                                                                                        \
            for (; i_a04__ < (size_t)(pos) + n_a04__; ++i_a04__) {                                                                       \
                pvt_clib_free(pvt_dat_to_base_((arr)[i_a04__]));                                                                         \
            }                                                                                                                            \
            ttl_a04__ -= n_a04__;                                                                                                        \
            pvt_set_ttl_siz_((arr), ttl_a04__);                                                                                          \
            pvt_clib_memmove((arr) + (size_t)(pos), (arr) + (size_t)(pos) + n_a04__, sizeof(*(arr)) * (ttl_a04__ - (size_t)(pos)));      \
        }                                                                                                                                \
    } while (0)

/**
 * @brief cstring_array_push_back - Add a string to the end of the vector.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param ptr   - Pointer to the first character of the string added to the
 *                cstring_array.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_push_back(arr, ptr, count)                 \
    do {                                                         \
        size_t newttl_a05__;                                     \
        if (!(arr)) {                                            \
            pvt_grow_((arr), 2);                                 \
            pvt_set_ttl_siz_((arr), 1);                          \
        }                                                        \
        newttl_a05__ = pvt_ttl_siz_(arr) + 1;                    \
        if (pvt_ttl_cap_(arr) < newttl_a05__) {                  \
            pvt_grow_((arr), newttl_a05__);                      \
        }                                                        \
        pvt_set_ttl_siz_((arr), newttl_a05__);                   \
        (arr)[newttl_a05__ - 2] = NULL;                          \
        cstring_assign((arr)[newttl_a05__ - 2], (ptr), (count)); \
        (arr)[newttl_a05__ - 1] = NULL;                          \
    } while (0)

/**
 * @brief cstring_array_pop_back - Remove the last string from the
 *                                 cstring_array.
 * @param arr - The cstring_array.
 * @return void
 */
#define cstring_array_pop_back(arr)                 \
    do {                                            \
        const size_t siz_a06__ = cstring_size(arr); \
        if (siz_a06__) {                            \
            pvt_set_ttl_siz_((arr), siz_a06__);     \
            cstring_free((arr)[siz_a06__ - 1]);     \
        }                                           \
    } while (0)

/**
 * @brief cstring_array_copy - Copy a cstring_array.
 * @param from - The original cstring_array.
 * @param to   - Destination to which the cstring_array is copied. Can be a NULL
 *               vector. <br>
 *               If `to` refers to an existing vector, the old content is
 *               overwritten.
 * @return void
 */
#define cstring_array_copy(from, to)                          \
    do {                                                      \
        cstring_array_clear(to);                              \
        if (from) {                                           \
            const size_t fromttl_a07__ = pvt_ttl_siz_(from);  \
            size_t i_a07__             = 0;                   \
            cstring_free(to);                                 \
            pvt_grow_((to), fromttl_a07__);                   \
            for (; i_a07__ < fromttl_a07__ - 1; ++i_a07__) {  \
                (to)[i_a07__] = NULL;                         \
                cstring_copy((from)[i_a07__], (to)[i_a07__]); \
            }                                                 \
            (to)[i_a07__] = NULL;                             \
            pvt_set_ttl_siz_((to), fromttl_a07__);            \
        }                                                     \
    } while (0)

/**
 * @brief cstring_array_resize - Resize the container to contain `count`
 *                               strings.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param n     - New size of the cstring_array.
 * @param ptr   - Pointer to the first character of the strings added if the
 *                container grows.
 * @param count - Number of consecutive characters to be used.
 * @return void
 */
#define cstring_array_resize(arr, n, ptr, count)                   \
    do {                                                           \
        const size_t n_a08__ = (size_t)(n);                        \
        size_t siz_a08__     = cstring_size(arr);                  \
        if (n_a08__ > siz_a08__) {                                 \
            pvt_grow_((arr), n_a08__ + 1);                         \
            do {                                                   \
                (arr)[siz_a08__] = NULL;                           \
                cstring_assign((arr)[siz_a08__], (ptr), (count));  \
            } while (++siz_a08__ < n_a08__);                       \
        } else {                                                   \
            while (siz_a08__-- > n_a08__) {                        \
                pvt_clib_free(pvt_dat_to_base_((arr)[siz_a08__])); \
            }                                                      \
        }                                                          \
        (arr)[n_a08__] = NULL;                                     \
        pvt_set_ttl_siz_((arr), n_a08__ + 1);                      \
    } while (0)

/**
 * @brief cstring_array_swap - Exchange the content of the cstring_array by the
 *                             content of another cstring_array of the same
 *                             type.
 * @param arr   - The cstring_array. Can be a NULL vector.
 * @param other - The other cstring_array to swap content with. Can be a NULL
 *                vector.
 * @return void
 */
#define cstring_array_swap(arr, other) \
    cstring_swap((arr), (other))

/**
 * @brief cstring_array_slice - Copy a part of a vector.
 * @param from - The source vector.
 * @param pos  - Position in the source vector where the part begins.
 * @param n    - Number of consecutive strings to copy.
 * @param to   - Destination to which the part is copied. Can be a NULL
 *               vector. <br>
 *               If `to` refers to an existing vector, the old content is
 *               overwritten.
 * @return void
 */
#define cstring_array_slice(from, pos, n, to)                                                                                      \
    do {                                                                                                                           \
        const size_t pos_a09__ = (size_t)(pos);                                                                                    \
        cstring_array_clear(to);                                                                                                   \
        if (pvt_ttl_siz_(from) > pos_a09__) {                                                                                      \
            const size_t n_a09__ = (pos_a09__ + (size_t)(n) >= cstring_size(from)) ? cstring_size(from) - pos_a09__ : (size_t)(n); \
            size_t i_a09__       = 0;                                                                                              \
            if ((to) && pvt_ttl_cap_(to) < n_a09__ + 1) {                                                                          \
                cstring_free(to);                                                                                                  \
            }                                                                                                                      \
            if (!(to)) {                                                                                                           \
                pvt_grow_((to), n_a09__ + 1);                                                                                      \
                pvt_set_ttl_siz_((to), 1);                                                                                         \
            }                                                                                                                      \
            for (; i_a09__ < n_a09__; ++i_a09__) {                                                                                 \
                (to)[i_a09__] = NULL;                                                                                              \
                cstring_assign((to)[i_a09__], (from)[pos_a09__ + i_a09__], cstring_size((from)[pos_a09__ + i_a09__]));             \
            }                                                                                                                      \
            pvt_set_ttl_siz_((to), n_a09__ + 1);                                                                                   \
            (to)[n_a09__] = NULL;                                                                                                  \
        }                                                                                                                          \
    } while (0)

/**
 * @brief cstring_array_join - Concatenate the strings of a vector using the
 *                             specified joiner.
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
#define cstring_array_join(arr, ptr, count, ret_str)                                                    \
    do {                                                                                                \
        const void *const chk_a10__ = (const void *)(ptr);                                              \
        const size_t siz_a10__      = cstring_size(arr);                                                \
        if (siz_a10__) {                                                                                \
            size_t i_a10__, elsiz_a10__, rssiz_a10__;                                                   \
            const size_t cnt_a10__ = chk_a10__ ? (size_t)(count) : (size_t)0;                           \
            size_t strsiz_a10__    = (siz_a10__ - 1) * cnt_a10__;                                       \
            for (i_a10__ = 0; i_a10__ < siz_a10__; ++i_a10__) {                                         \
                strsiz_a10__ += cstring_size((arr)[i_a10__]);                                           \
            }                                                                                           \
            if (strsiz_a10__ > cstring_capacity(ret_str)) {                                             \
                cstring_free(ret_str);                                                                  \
                pvt_grow_((ret_str), strsiz_a10__ + 1);                                                 \
            }                                                                                           \
            rssiz_a10__ = elsiz_a10__ = cstring_size((arr)[0]);                                         \
            pvt_clib_memcpy((ret_str), (arr)[0], elsiz_a10__ * sizeof(*(ptr)));                         \
            for (i_a10__ = 1; i_a10__ < siz_a10__; ++i_a10__) {                                         \
                if (cnt_a10__) {                                                                        \
                    pvt_clib_memcpy((ret_str) + rssiz_a10__, (ptr), cnt_a10__ * sizeof(*(ptr)));        \
                    rssiz_a10__ += cnt_a10__;                                                           \
                }                                                                                       \
                elsiz_a10__ = cstring_size((arr)[i_a10__]);                                             \
                pvt_clib_memcpy((ret_str) + rssiz_a10__, (arr)[i_a10__], elsiz_a10__ * sizeof(*(ptr))); \
                rssiz_a10__ += elsiz_a10__;                                                             \
            }                                                                                           \
            pvt_set_ttl_siz_((ret_str), strsiz_a10__ + 1);                                              \
            (ret_str)[strsiz_a10__] = 0;                                                                \
        } else {                                                                                        \
            cstring_clear(ret_str);                                                                     \
        }                                                                                               \
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
#if !defined(pvt_clib_free) || !defined(pvt_clib_malloc) || !defined(pvt_clib_realloc)
#ifdef pvt_clib_free
#undef pvt_clib_free
#endif
#ifdef pvt_clib_malloc
#undef pvt_clib_malloc
#endif
#ifdef pvt_clib_realloc
#undef pvt_clib_realloc
#endif
#include <stdlib.h>
#define pvt_clib_free free
#define pvt_clib_malloc malloc
#define pvt_clib_realloc realloc
#endif
/* functions independent of memory allocation */
#ifndef pvt_clib_assert
#include <assert.h>
#define pvt_clib_assert assert
#endif
#ifndef pvt_clib_memcpy
#include <string.h>
#define pvt_clib_memcpy memcpy
#endif
#ifndef pvt_clib_memmove
#include <string.h>
#define pvt_clib_memmove memmove
#endif

/**
 * @brief pvt_metadata_t - Header type that prefixes a cstring or cstring_array.
 */
typedef struct pvt_metadata_ {
    size_t size;            /*!< The size incl. terminator. */
    size_t capacity;        /*!< The capacity incl. space for the string terminator. */
    void (*unused)(void *); /*!< This maintains binary compatibility with `cvector_metadata_t` of the c-vector library. */
} pvt_metadata_t;

/**
 * @brief pvt_dat_to_base_ - For internal use, convert a buffer pointer to a
 *                           metadata pointer.
 * @param dat - The data pointer.
 * @return The metadata pointer of the cstring.
 */
#define pvt_dat_to_base_(dat) \
    (((pvt_metadata_t *)(void *)(dat)) - 1)

/**
 * @brief pvt_dat_to_const_base_ - For internal use, convert a buffer pointer to
 *                                 a pointer to constant metadata.
 * @param dat - The data pointer.
 * @return The pointer to constant metadata of the cstring.
 */
#define pvt_dat_to_const_base_(dat) \
    (((const pvt_metadata_t *)(const void *)(dat)) - 1)

/**
 * @brief pvt_ttl_cap_ - For internal use, get the current capacity of the
 *                       buffer incl. terminator.
 * @param dat - The data pointer.
 * @return The capacity as a `size_t`.
 */
#define pvt_ttl_cap_(dat) \
    ((dat) ? pvt_dat_to_const_base_(dat)->capacity : (size_t)0)

/**
 * @brief pvt_ttl_siz_ - For internal use, get the current size of the buffer
 *                       incl. terminator.
 * @param dat - The data pointer.
 * @return The size as a `size_t`.
 */
#define pvt_ttl_siz_(dat) \
    ((dat) ? pvt_dat_to_const_base_(dat)->size : (size_t)0)

/**
 * @brief pvt_set_ttl_siz_ - For internal use, set the size member of the
 *                           metadata.
 * @param dat - The data pointer.
 * @param siz - The new size to set.
 * @return void
 */
#define pvt_set_ttl_siz_(dat, siz)           \
    do {                                     \
        pvt_dat_to_base_(dat)->size = (siz); \
    } while (0)

/**
 * @brief pvt_grow_ - For internal use, ensure that the buffer is at least
 *                    `count` elements big.
 * @param dat   - The data pointer.
 * @param count - The new capacity to set.
 * @return void
 */
#define pvt_grow_(dat, count)                                                               \
    do {                                                                                    \
        pvt_metadata_t *base_p00__;                                                         \
        const size_t siz_p00__ = (size_t)(count) * sizeof(*(dat)) + sizeof(pvt_metadata_t); \
        if (dat) {                                                                          \
            base_p00__ = pvt_clib_realloc(pvt_dat_to_base_(dat), siz_p00__);                \
            pvt_clib_assert(base_p00__);                                                    \
        } else {                                                                            \
            base_p00__ = pvt_clib_malloc(siz_p00__);                                        \
            pvt_clib_assert(base_p00__);                                                    \
            base_p00__->unused = NULL;                                                      \
        }                                                                                   \
        base_p00__->capacity = (size_t)(count);                                             \
        (dat)                = (void *)(base_p00__ + 1);                                    \
    } while (0)

/**
 * @brief pvt_str_n_eq_ - For internal use, check the equality of two string
 *                        sequences.
 * @param s1     - Pointer to the first string sequence.
 * @param s2     - Pointer to the second string sequence.
 * @param n      - The number of characters to compare.
 * @param ret_eq - Variable of type `int` that is set either to 1 if the
 *                 sequences are equal, or to 0 otherwise.
 * @return void
 */
#define pvt_str_n_eq_(s1, s2, n, ret_eq)                                        \
    do {                                                                        \
        ptrdiff_t i_p01__ = (ptrdiff_t)(n) - (ptrdiff_t)1;                      \
        while (((ret_eq) = ((s1)[i_p01__] == (s2)[i_p01__])) && --i_p01__ >= 0) \
            ;                                                                   \
    } while (0)

/**
 * @brief pvt_make_find_sub_hash_ - For internal use, pre-calculate the hash
 *                                  used for pvt_find_.
 * @param ptr      - Pointer to the first character of the string to search for.
 * @param count    - Length of the string to search for.
 * @param ret_hash - Pre-calculated hash of the string to search for.
 * @return void
 */
#define pvt_make_find_sub_hash_(ptr, count, ret_hash)                                  \
    do {                                                                               \
        static const size_t tpmsk_p02__ = pvt_get_typemask_(ptr);                      \
        ptrdiff_t off_p02__             = 0;                                           \
        for (; off_p02__ < (ptrdiff_t)(count); ++off_p02__) {                          \
            (ret_hash) = ((ret_hash) << 1) + ((size_t)(ptr)[off_p02__] & tpmsk_p02__); \
        }                                                                              \
    } while (0)

/**
 * @brief pvt_find_ - For internal use, find the first occurrence of the given
 *                    substring.
 * @param str        - The cstring.
 * @param pos        - Position at which to start the search, i.e. the found
 *                     substring must not begin in a position preceding `pos`.
 *                     Zero means that the whole `str` is searched.
 * @param ptr        - Pointer to the first character of the string to search
 *                     for.
 * @param count      - Length of the string to search for.
 * @param sub_hs     - Pre-calculated hash of the string to search for.
 * @param ret_offset - Variable of type `ptrdiff_t` that receives the position
 *                     of the first character of the found substring or -1 if no
 *                     such substring is found.
 * @return void
 */
#define pvt_find_(str, pos, ptr, count, sub_hs, ret_offset)                                                          \
    do {                                                                                                             \
        const void *const chk_p03__ = (const void *)(ptr);                                                           \
        const ptrdiff_t cnt_p03__ = (ptrdiff_t)(count), siz_p03__ = (ptrdiff_t)cstring_size(str) - (ptrdiff_t)(pos); \
        if (!chk_p03__ || (ptrdiff_t)(pos) < 0 || cnt_p03__ > siz_p03__ || siz_p03__ <= 0 || cnt_p03__ <= 0) {       \
            (ret_offset) = (ptrdiff_t)-1;                                                                            \
        } else if (cnt_p03__ == siz_p03__) {                                                                         \
            int eq_p03__;                                                                                            \
            pvt_str_n_eq_((str) + (ptrdiff_t)(pos), (ptr), cnt_p03__, eq_p03__);                                     \
            (ret_offset) = eq_p03__ ? (ptrdiff_t)(pos) : (ptrdiff_t)-1;                                              \
        } else if (cnt_p03__ == 1) {                                                                                 \
            pvt_find_first_char_(0, (str), (pos), (ptr), (ret_offset));                                              \
        } else {                                                                                                     \
            ptrdiff_t off_p03__;                                                                                     \
            const ptrdiff_t end_p03__ = cnt_p03__ - 1, diff_p03__ = (ptrdiff_t)cstring_size(str) - cnt_p03__;        \
            size_t strhs_p03__ = 0, hash_factor__ = 1;                                                               \
            static const size_t tpmsk_p03__ = pvt_get_typemask_(str);                                                \
            (ret_offset)                    = (ptrdiff_t)-1;                                                         \
            for (off_p03__ = 0; off_p03__ < end_p03__; ++off_p03__) {                                                \
                strhs_p03__ = (strhs_p03__ << 1) + ((size_t)(str)[off_p03__ + (ptrdiff_t)(pos)] & tpmsk_p03__);      \
                hash_factor__ <<= 1;                                                                                 \
            }                                                                                                        \
            strhs_p03__ = (strhs_p03__ << 1) + ((size_t)(str)[off_p03__ + (ptrdiff_t)(pos)] & tpmsk_p03__);          \
            off_p03__   = (ptrdiff_t)(pos);                                                                          \
            do {                                                                                                     \
                if ((sub_hs) == strhs_p03__) {                                                                       \
                    int eq_p03__;                                                                                    \
                    pvt_str_n_eq_((str) + off_p03__, (ptr), cnt_p03__, eq_p03__);                                    \
                    if (eq_p03__) {                                                                                  \
                        (ret_offset) = off_p03__;                                                                    \
                        break;                                                                                       \
                    }                                                                                                \
                }                                                                                                    \
                strhs_p03__ =                                                                                        \
                    ((strhs_p03__ - hash_factor__ * ((size_t)(str)[off_p03__] & tpmsk_p03__)) << 1) +                \
                    ((size_t)(str)[off_p03__ + cnt_p03__] & tpmsk_p03__);                                            \
            } while (off_p03__++ < diff_p03__);                                                                      \
        }                                                                                                            \
    } while (0)

/**
 * @brief pvt_find_first_char_ - For internal use, find the first character
 *                               equal or not equal (depending on `not_eq`) to
 *                               the character pointed to by `pchar`.
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
#define pvt_find_first_char_(not_eq, str, pos, pchar, ret_offset)                       \
    do {                                                                                \
        const ptrdiff_t siz_p04__ = (ptrdiff_t)cstring_size(str);                       \
        for ((ret_offset) = (ptrdiff_t)(pos);                                           \
             (ret_offset) < siz_p04__ && ((str)[(ret_offset)] == *(pchar)) == (not_eq); \
             ++(ret_offset))                                                            \
            ;                                                                           \
        if ((ret_offset) == siz_p04__) {                                                \
            (ret_offset) = (ptrdiff_t)-1;                                               \
        }                                                                               \
    } while (0)

/**
 * @brief pvt_find_last_char_ - For internal use, find the last character equal
 *                              or not equal (depending on `not_eq`) to the
 *                              character pointed to by `pchar`.
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
#define pvt_find_last_char_(not_eq, str, pos, pchar, ret_offset)                            \
    do {                                                                                    \
        for ((ret_offset) = (ptrdiff_t)(pos);                                               \
             (ret_offset) > (ptrdiff_t)-1 && ((str)[(ret_offset)] == *(pchar)) == (not_eq); \
             --(ret_offset))                                                                \
            ;                                                                               \
    } while (0)

/**
 * @brief pvt_get_typemask_ - For internal use, calculate the bit mask with all
 *                            bits of the size of the referenced character set.
 * @details The value is considered a constant expression, calculated at compile
 *          time. The receiving variables are declared as `static const` to make
 *          this clear to the compiler.
 * @param ptr - Pointer to a character.
 * @return The calculated bit mask as value of type `size_t`.
 */
#define pvt_get_typemask_(ptr) \
    ((sizeof(size_t) <= sizeof(*(ptr))) ? (size_t)-1 : (((size_t)1 << (sizeof(*(ptr)) * CHAR_BIT)) - 1))

/**
 * @brief pvt_make_charmask_ - For internal use, create a bit mask that helps to
 *                             avoid the complexity of pvt_find_first_of_ and
 *                             pvt_find_last_of_ being always O(n*m).
 * @param ptr    - Pointer to the first character of the character sequence.
 * @param count  - Length of the character sequence.
 * @param chmask - Variable of type `size_t` that receives the created bit mask.
 * @return void
 */
#define pvt_make_charmask_(ptr, count, chmask)                    \
    do {                                                          \
        static const size_t tpmsk_p05__ = pvt_get_typemask_(ptr); \
        ptrdiff_t i_p05__               = 0;                      \
        (chmask)                        = 0;                      \
        while (i_p05__ < (count)) {                               \
            (chmask) |= (size_t)(ptr)[i_p05__++];                 \
        }                                                         \
        (chmask) = (~(chmask)) & tpmsk_p05__;                     \
    } while (0)

/**
 * @brief pvt_find_first_of_ - For internal use, find the first character equal
 *                             to one or none (depending on `not_of`) of the
 *                             characters in the given character sequence.
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
#define pvt_find_first_of_(not_of, str, pos, ptr, count, ret_offset)                                 \
    do {                                                                                             \
        const void *const chk_p06__ = (const void *)(ptr);                                           \
        const ptrdiff_t cnt_p06__ = (ptrdiff_t)(count), siz_p06__ = (ptrdiff_t)cstring_size(str);    \
        if (!chk_p06__ || (ptrdiff_t)(pos) < 0 || siz_p06__ <= (ptrdiff_t)(pos) || cnt_p06__ <= 0) { \
            (ret_offset) = (ptrdiff_t)-1;                                                            \
        } else if (cnt_p06__ == 1) {                                                                 \
            pvt_find_first_char_((not_of), (str), (pos), (ptr), (ret_offset));                       \
        } else {                                                                                     \
            size_t chmsk_p06__;                                                                      \
            ptrdiff_t suboff_p06__;                                                                  \
            ptrdiff_t stroff_p06__ = (ptrdiff_t)(pos);                                               \
            pvt_make_charmask_((ptr), cnt_p06__, chmsk_p06__);                                       \
            for (; stroff_p06__ < siz_p06__; ++stroff_p06__) {                                       \
                if ((size_t)(str)[stroff_p06__] & chmsk_p06__) {                                     \
                    continue;                                                                        \
                }                                                                                    \
                for (suboff_p06__ = 0;                                                               \
                     suboff_p06__ < cnt_p06__ && (str)[stroff_p06__] != (ptr)[suboff_p06__];         \
                     ++suboff_p06__)                                                                 \
                    ;                                                                                \
                if ((suboff_p06__ == cnt_p06__) == (not_of)) {                                       \
                    break;                                                                           \
                }                                                                                    \
            }                                                                                        \
            (ret_offset) = (stroff_p06__ == siz_p06__) ? (ptrdiff_t)-1 : stroff_p06__;               \
        }                                                                                            \
    } while (0)

/**
 * @brief pvt_find_last_of_ - For internal use, find the last character equal to
 *                            one or none (depending on `not_of`) of the
 *                            characters in the given character sequence.
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
#define pvt_find_last_of_(not_of, str, pos, ptr, count, ret_offset)                                                \
    do {                                                                                                           \
        const void *const chk_p07__ = (const void *)(ptr);                                                         \
        const ptrdiff_t cnt_p07__   = (ptrdiff_t)(count);                                                          \
        ptrdiff_t stroff_p07__      = ((ptrdiff_t)(pos) == -1 || (ptrdiff_t)(pos) >= (ptrdiff_t)cstring_size(str)) \
                                          ? (ptrdiff_t)cstring_size(str) - 1                                       \
                                          : (ptrdiff_t)(pos);                                                      \
        if (!chk_p07__ || (ptrdiff_t)(pos) < -1 || stroff_p07__ < 0 || cnt_p07__ <= 0) {                           \
            (ret_offset) = (ptrdiff_t)-1;                                                                          \
        } else if (cnt_p07__ == 1) {                                                                               \
            pvt_find_last_char_((not_of), (str), (pos), (ptr), (ret_offset));                                      \
        } else {                                                                                                   \
            size_t chmsk_p07__;                                                                                    \
            ptrdiff_t suboff_p07__;                                                                                \
            pvt_make_charmask_((ptr), cnt_p07__, chmsk_p07__);                                                     \
            for (; stroff_p07__ >= 0; --stroff_p07__) {                                                            \
                if ((size_t)(str)[stroff_p07__] & chmsk_p07__) {                                                   \
                    continue;                                                                                      \
                }                                                                                                  \
                for (suboff_p07__ = 0;                                                                             \
                     suboff_p07__ < cnt_p07__ && (str)[stroff_p07__] != (ptr)[suboff_p07__];                       \
                     ++suboff_p07__)                                                                               \
                    ;                                                                                              \
                if ((suboff_p07__ == cnt_p07__) == (not_of)) {                                                     \
                    break;                                                                                         \
                }                                                                                                  \
            }                                                                                                      \
            (ret_offset) = stroff_p07__;                                                                           \
        }                                                                                                          \
    } while (0)

/** @} */

/** @endcond */

#endif /* header guard */
