#include "cstring.h"
#include "utest/utest.h"
#include <wchar.h>

static const char literal[]     = "abcde";
static const wchar_t wliteral[] = L"abcde";

#define strlen_of(lit) \
    (sizeof(lit) / sizeof((lit)[0]) - 1)

#define wcseq(s1, s2) \
    (wcscmp((s1), (s2)) == 0)

UTEST(test, cstring_literal) {
    cstring_literal(lit, char, "abc");
    ASSERT_EQ(cstring_size(lit), 3U);
    ASSERT_EQ(cstring_capacity(lit), 3U);
    ASSERT_STREQ(lit, "abc");

    /* -- wide string -- */

    cstring_literal(wlit, wchar_t, L"abc");
    ASSERT_EQ(cstring_size(wlit), 3U);
    ASSERT_EQ(cstring_capacity(wlit), 3U);
    ASSERT_TRUE(wcseq(wlit, L"abc"));
}

UTEST(test, cstring_init) {
    cstring_init(str, char);

    ASSERT_NE(str, NULL);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_NE(cstring_capacity(str), 0U);

    cstring_free(str);

    /* -- wide string -- */

    cstring_init(wstr, wchar_t);

    ASSERT_NE(wstr, NULL);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_NE(cstring_capacity(wstr), 0U);

    cstring_free(wstr);
}

UTEST(test, cstring_assign_clear) {
    cstring_string_type(char) str = NULL;

    cstring_assign(str, literal + 1, 3);
    ASSERT_EQ(cstring_size(str), 3U);
    ASSERT_EQ(cstring_capacity(str), 3U);
    ASSERT_STREQ(str, "bcd");

    cstring_assign(str, literal, strlen_of(literal));
    ASSERT_EQ(cstring_size(str), strlen_of(literal));
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, literal);

    cstring_assign(str, literal + 1, 3);
    ASSERT_EQ(cstring_size(str), 3U);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, "bcd");

    cstring_clear(str);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_EQ(str[0], '\0');

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;

    cstring_assign(wstr, wliteral + 1, 3);
    ASSERT_EQ(cstring_size(wstr), 3U);
    ASSERT_EQ(cstring_capacity(wstr), 3U);
    ASSERT_TRUE(wcseq(wstr, L"bcd"));

    cstring_assign(wstr, wliteral, strlen_of(wliteral));
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral));
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, wliteral));

    cstring_assign(wstr, wliteral + 1, 3);
    ASSERT_EQ(cstring_size(wstr), 3U);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L"bcd"));

    cstring_clear(wstr);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_EQ(wstr[0], L'\0');

    cstring_free(wstr);
}

UTEST(test, cstring_at) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    ASSERT_EQ(*cstring_at(str, 0), 'a');
    ASSERT_EQ(*cstring_at(str, 4), 'e');
    ASSERT_EQ(cstring_at(str, 5), NULL);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    ASSERT_EQ(*cstring_at(wstr, 0), L'a');
    ASSERT_EQ(*cstring_at(wstr, 4), L'e');
    ASSERT_EQ(cstring_at(wstr, 5), NULL);

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    ASSERT_EQ(cstring_at(nullstr, 0), NULL);

    cstring_init(zerolenstr, char);
    ASSERT_EQ(cstring_at(zerolenstr, 0), NULL);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_front_back) {
    /* see cstring_insert test for the typical use */

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    ASSERT_EQ(cstring_front(nullstr), NULL);
    ASSERT_EQ(cstring_back(nullstr), NULL);

    cstring_init(zerolenstr, char);
    ASSERT_EQ(cstring_front(zerolenstr), NULL);
    ASSERT_EQ(cstring_back(zerolenstr), NULL);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_begin_end) {
    cstring_string_type(char) str = NULL;

    cstring_assign(str, literal, strlen_of(literal));
    cstring_iterator(char) it  = cstring_begin(str);
    cstring_iterator(char) end = cstring_end(str);
    int i                      = 0;
    while (it < end) {
        ASSERT_EQ(*it, (char)('a' + i));
        ++it;
        ++i;
    }

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;

    cstring_assign(wstr, wliteral, strlen_of(wliteral));
    cstring_iterator(wchar_t) wit  = cstring_begin(wstr);
    cstring_iterator(wchar_t) wend = cstring_end(wstr);
    i                              = 0;
    while (wit < wend) {
        ASSERT_EQ(*wit, (wchar_t)(L'a' + i));
        ++wit;
        ++i;
    }

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    ASSERT_EQ(cstring_begin(nullstr), NULL);
    ASSERT_EQ(cstring_end(nullstr), NULL);

    cstring_init(zerolenstr, char);
    ASSERT_EQ(*cstring_begin(zerolenstr), '\0');
    ASSERT_EQ(*cstring_end(zerolenstr), '\0');
    cstring_free(zerolenstr);
}

UTEST(test, cstring_empty) {
    /* see cstring_erase test for the typical use */

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    ASSERT_TRUE(cstring_empty(nullstr));
}

UTEST(test, cstring_size_length_capacity) {
    /* the typical use is found in more places, note that cstring_length is the same as cstring_size */

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    ASSERT_EQ(cstring_size(nullstr), 0U);
    ASSERT_EQ(cstring_length(nullstr), 0U);
    ASSERT_EQ(cstring_capacity(nullstr), 0U);

    cstring_string_type(char) zerolenstr = NULL;
    cstring_reserve(zerolenstr, 0);
    ASSERT_NE(zerolenstr, NULL);
    ASSERT_EQ(cstring_size(zerolenstr), 0U);
    ASSERT_EQ(cstring_length(zerolenstr), 0U);
    ASSERT_EQ(cstring_capacity(zerolenstr), 0U);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_max_size) {
    ASSERT_TRUE(cstring_max_size(char) && (cstring_max_size(char) & 1) == 0);

    /* -- wide string -- */

    ASSERT_TRUE(cstring_max_size(wchar_t) && (cstring_max_size(wchar_t) & 1) == 0);
}

UTEST(test, cstring_reserve) {
    cstring_string_type(char) str = NULL;

    cstring_reserve(str, 10);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_EQ(cstring_capacity(str), 10U);
    ASSERT_EQ(*str, '\0');

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;

    cstring_reserve(wstr, 10);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_EQ(cstring_capacity(wstr), 10U);
    ASSERT_EQ(*wstr, L'\0');

    cstring_free(wstr);
}

UTEST(test, cstring_shrink_to_fit) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_pop_back(str);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 1);

    cstring_shrink_to_fit(str);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal) - 1);
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 1);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_pop_back(wstr);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 1);

    cstring_shrink_to_fit(wstr);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral) - 1);
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 1);

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_shrink_to_fit(nullstr);
    ASSERT_EQ(nullstr, NULL);
}

static size_t unsafe_buffer_update(char *buffer, size_t buffer_size) {
    static const char data[] = "1234567890";
    const size_t to_copy     = (sizeof(data) - 1) < buffer_size ? (sizeof(data) - 1) : buffer_size;
    memcpy(buffer, data, to_copy);
    return to_copy;
}

static size_t unsafe_wbuffer_update(wchar_t *wbuffer, size_t wbuffer_size) {
    static const wchar_t data[] = L"1234567890";
    const size_t to_copy        = (sizeof(data) / sizeof(wchar_t) - 1) < wbuffer_size ? (sizeof(data) / sizeof(wchar_t) - 1) : wbuffer_size;
    memcpy(wbuffer, data, to_copy * sizeof(wchar_t));
    return to_copy;
}

UTEST(test, cstring_unsafe_set_size) {
    size_t len;
    cstring_string_type(char) str = NULL;

    cstring_reserve(str, 5);
    len = unsafe_buffer_update(str, cstring_capacity(str));
    cstring_unsafe_set_size(str, len);
    ASSERT_EQ(cstring_size(str), 5U);
    ASSERT_STREQ(str, "12345");

    cstring_reserve(str, 30);
    len = unsafe_buffer_update(str, cstring_capacity(str));
    cstring_unsafe_set_size(str, len);
    ASSERT_EQ(cstring_size(str), 10U);
    ASSERT_STREQ(str, "1234567890");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;

    cstring_reserve(wstr, 5);
    len = unsafe_wbuffer_update(wstr, cstring_capacity(wstr));
    cstring_unsafe_set_size(wstr, len);
    ASSERT_EQ(cstring_size(wstr), 5U);
    ASSERT_TRUE(wcseq(wstr, L"12345"));

    cstring_reserve(wstr, 30);
    len = unsafe_wbuffer_update(wstr, cstring_capacity(wstr));
    cstring_unsafe_set_size(wstr, len);
    ASSERT_EQ(cstring_size(wstr), 10U);
    ASSERT_TRUE(wcseq(wstr, L"1234567890"));

    cstring_free(wstr);
}

UTEST(test, cstring_insert) {
    static const char lit1[]      = "0145";
    static const char lit2[]      = "23";
    cstring_string_type(char) str = NULL;

    cstring_assign(str, lit1, strlen_of(lit1));
    cstring_insert(str, 2, lit2, strlen_of(lit2));
    ASSERT_EQ(cstring_size(str), 6U);
    ASSERT_STREQ(str, "012345");

    cstring_insert(str, 6, lit2, strlen_of(lit2));
    ASSERT_EQ(cstring_size(str), 8U);
    ASSERT_STREQ(str, "01234523");

    cstring_insert(str, 0, lit2, strlen_of(lit2));
    ASSERT_EQ(cstring_size(str), 10U);
    ASSERT_EQ(*cstring_front(str), '2');
    ASSERT_EQ(*cstring_back(str), '3');
    ASSERT_STREQ(str, "2301234523");

    cstring_free(str);

    /* -- wide string -- */

    static const wchar_t wlit1[]      = L"0145";
    static const wchar_t wlit2[]      = L"23";
    cstring_string_type(wchar_t) wstr = NULL;

    cstring_assign(wstr, wlit1, strlen_of(wlit1));
    cstring_insert(wstr, 2, wlit2, strlen_of(wlit2));
    ASSERT_EQ(cstring_size(wstr), 6U);
    ASSERT_TRUE(wcseq(wstr, L"012345"));

    cstring_insert(wstr, 6, wlit2, strlen_of(wlit2));
    ASSERT_EQ(cstring_size(wstr), 8U);
    ASSERT_TRUE(wcseq(wstr, L"01234523"));

    cstring_insert(wstr, 0, wlit2, strlen_of(wlit2));
    ASSERT_EQ(cstring_size(wstr), 10U);
    ASSERT_EQ(*cstring_front(wstr), L'2');
    ASSERT_EQ(*cstring_back(wstr), L'3');
    ASSERT_TRUE(wcseq(wstr, L"2301234523"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_insert(nullstr, 0, literal, strlen_of(literal));
    ASSERT_EQ(nullstr, NULL);

    cstring_init(zerolenstr, char);
    cstring_insert(zerolenstr, 1, literal, strlen_of(literal));
    ASSERT_STREQ(zerolenstr, "");

    cstring_insert(zerolenstr, 0, literal, strlen_of(literal));
    ASSERT_STREQ(zerolenstr, literal);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_erase) {
    cstring_string_type(char) str = NULL;

    cstring_assign(str, literal, strlen_of(literal));
    ASSERT_EQ(cstring_size(str), strlen_of(literal));
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, literal);

    cstring_erase(str, 3, 1);
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 1);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, "abce");

    cstring_erase(str, 3, 1);
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 2);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, "abc");

    cstring_erase(str, 0, 2);
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 4);
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, "c");

    cstring_erase(str, 0, 1000);
    ASSERT_TRUE(cstring_empty(str));
    ASSERT_EQ(cstring_capacity(str), strlen_of(literal));
    ASSERT_STREQ(str, "");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;

    cstring_assign(wstr, wliteral, strlen_of(wliteral));
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral));
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, wliteral));

    cstring_erase(wstr, 3, 1);
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 1);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L"abce"));

    cstring_erase(wstr, 3, 1);
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 2);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L"abc"));

    cstring_erase(wstr, 0, 2);
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 4);
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L"c"));

    cstring_erase(wstr, 0, 1000);
    ASSERT_TRUE(cstring_empty(wstr));
    ASSERT_EQ(cstring_capacity(wstr), strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L""));

    cstring_free(wstr);
}

UTEST(test, cstring_push_back) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_push_back(str, 'f');
    ASSERT_EQ(cstring_size(str), strlen_of(literal) + 1);
    ASSERT_STREQ(str, "abcdef");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_push_back(wstr, L'f');
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) + 1);
    ASSERT_TRUE(wcseq(wstr, L"abcdef"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_push_back(nullstr, 'x');
    ASSERT_EQ(cstring_size(nullstr), 1U);
    ASSERT_EQ(cstring_capacity(nullstr), 1U);
    ASSERT_STREQ(nullstr, "x");
    cstring_free(nullstr);
}

UTEST(test, cstring_pop_back) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_pop_back(str);
    ASSERT_EQ(cstring_size(str), strlen_of(literal) - 1);
    ASSERT_EQ(str[cstring_size(str)], '\0');

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_pop_back(wstr);
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) - 1);
    ASSERT_EQ(wstr[cstring_size(wstr)], L'\0');

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_pop_back(nullstr);
    ASSERT_EQ(nullstr, NULL);

    cstring_init(zerolenstr, char);
    cstring_pop_back(zerolenstr);
    ASSERT_STREQ(zerolenstr, "");
    cstring_free(zerolenstr);
}

UTEST(test, cstring_append) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_append(str, literal, strlen_of(literal));
    ASSERT_EQ(cstring_size(str), 2 * strlen_of(literal));
    ASSERT_STREQ(str, "abcdeabcde");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_append(wstr, wliteral, strlen_of(wliteral));
    ASSERT_EQ(cstring_size(wstr), 2 * strlen_of(wliteral));
    ASSERT_TRUE(wcseq(wstr, L"abcdeabcde"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_append(nullstr, literal, strlen_of(literal));
    ASSERT_STREQ(nullstr, literal);
    cstring_free(nullstr);

    cstring_init(zerolenstr, char);
    cstring_append(zerolenstr, literal, strlen_of(literal));
    ASSERT_STREQ(zerolenstr, literal);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_replace) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    static const char repl[] = "foo";
    cstring_replace(str, 1, 2, repl, strlen_of(repl));

    ASSERT_EQ(cstring_size(str), strlen_of(literal) + 1);
    ASSERT_STREQ(str, "afoode");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    static const wchar_t wrepl[] = L"foo";
    cstring_replace(wstr, 1, 2, wrepl, strlen_of(wrepl));

    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) + 1);
    ASSERT_TRUE(wcseq(wstr, L"afoode"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_replace(nullstr, 0, 0, repl, strlen_of(repl));
    ASSERT_EQ(nullstr, NULL);

    cstring_init(zerolenstr, char);
    cstring_replace(zerolenstr, 0, 0, repl, strlen_of(repl));
    ASSERT_STREQ(zerolenstr, repl);
    cstring_free(zerolenstr);
}

UTEST(test, cstring_copy) {
    cstring_string_type(char) str   = NULL;
    cstring_string_type(char) other = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_copy(str, other);
    ASSERT_EQ(cstring_size(str), cstring_size(other));
    ASSERT_EQ(other[cstring_size(other)], '\0');
    ASSERT_STREQ(other, literal);

    cstring_free(other);
    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr   = NULL;
    cstring_string_type(wchar_t) wother = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_copy(wstr, wother);
    ASSERT_EQ(cstring_size(wstr), cstring_size(wother));
    ASSERT_EQ(wother[cstring_size(wother)], L'\0');
    ASSERT_TRUE(wcseq(wother, wliteral));

    cstring_free(wother);
    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_copy(nullstr, other);
    ASSERT_EQ(nullstr, NULL);
    ASSERT_EQ(other, NULL);

    cstring_assign(str, literal, strlen_of(literal));
    cstring_assign(other, "a", 1);
    cstring_copy(str, other);
    ASSERT_EQ(cstring_size(other), strlen_of(literal));
    ASSERT_STREQ(other, literal);

    static const char lit2[] = "1234567890";
    cstring_assign(other, lit2, strlen_of(lit2));
    cstring_copy(str, other);
    ASSERT_EQ(cstring_size(other), strlen_of(literal));
    ASSERT_STREQ(other, literal);

    cstring_free(other);
    cstring_free(str);
}

UTEST(test, cstring_resize) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_resize(str, strlen_of(literal) + 1, 'x');
    ASSERT_EQ(cstring_size(str), strlen_of(literal) + 1);
    ASSERT_STREQ(str, "abcdex");

    cstring_resize(str, 4, 'x');
    ASSERT_EQ(cstring_size(str), 4U);
    ASSERT_STREQ(str, "abcd");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_resize(wstr, strlen_of(wliteral) + 1, L'x');
    ASSERT_EQ(cstring_size(wstr), strlen_of(wliteral) + 1);
    ASSERT_TRUE(wcseq(wstr, L"abcdex"));

    cstring_resize(wstr, 4, L'x');
    ASSERT_EQ(cstring_size(wstr), 4U);
    ASSERT_TRUE(wcseq(wstr, L"abcd"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_resize(nullstr, 10, 'x');
    ASSERT_STREQ(nullstr, "xxxxxxxxxx");
    cstring_free(nullstr);

    cstring_init(zerolenstr, char);
    cstring_resize(zerolenstr, 10, 'x');
    ASSERT_STREQ(zerolenstr, "xxxxxxxxxx");
    cstring_free(zerolenstr);
}

UTEST(test, cstring_swap) {
    cstring_string_type(char) str1 = NULL;
    cstring_string_type(char) str2 = NULL;
    cstring_assign(str1, literal, 2);
    cstring_assign(str2, literal + 2, 3);

    ASSERT_EQ(cstring_size(str1), 2U);
    ASSERT_EQ(cstring_size(str2), 3U);
    ASSERT_STREQ(str1, "ab");
    ASSERT_STREQ(str2, "cde");

    cstring_swap(str1, str2);
    ASSERT_EQ(cstring_size(str1), 3U);
    ASSERT_EQ(cstring_size(str2), 2U);
    ASSERT_STREQ(str1, "cde");
    ASSERT_STREQ(str2, "ab");

    cstring_free(str2);
    cstring_free(str1);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr1 = NULL;
    cstring_string_type(wchar_t) wstr2 = NULL;
    cstring_assign(wstr1, wliteral, 2);
    cstring_assign(wstr2, wliteral + 2, 3);

    ASSERT_EQ(cstring_size(wstr1), 2U);
    ASSERT_EQ(cstring_size(wstr2), 3U);
    ASSERT_TRUE(wcseq(wstr1, L"ab"));
    ASSERT_TRUE(wcseq(wstr2, L"cde"));

    cstring_swap(wstr1, wstr2);
    ASSERT_EQ(cstring_size(wstr1), 3U);
    ASSERT_EQ(cstring_size(wstr2), 2U);
    ASSERT_TRUE(wcseq(wstr1, L"cde"));
    ASSERT_TRUE(wcseq(wstr2, L"ab"));

    cstring_free(wstr2);
    cstring_free(wstr1);

    /* -- special cases -- */

    ASSERT_EQ(str1, NULL);
    ASSERT_EQ(str2, NULL);
    cstring_swap(str1, str2);
    ASSERT_EQ(str1, NULL);
    ASSERT_EQ(str2, NULL);

    cstring_assign(str1, literal, strlen_of(literal));
    cstring_swap(str1, str2);
    ASSERT_EQ(str1, NULL);
    ASSERT_STREQ(str2, literal);
    cstring_swap(str1, str2);
    ASSERT_STREQ(str1, literal);
    ASSERT_EQ(str2, NULL);
    cstring_free(str1);
}

UTEST(test, cstring_trim) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "  ab  ", strlen_of("  ab  "));
    cstring_trim(str, ' ', 3);
    ASSERT_EQ(cstring_size(str), 2U);
    ASSERT_STREQ(str, "ab");

    cstring_assign(str, "ab  ", strlen_of("ab  "));
    cstring_trim(str, ' ', 1);
    ASSERT_EQ(cstring_size(str), 4U);
    ASSERT_STREQ(str, "ab  ");
    cstring_trim(str, ' ', 3);
    ASSERT_EQ(cstring_size(str), 2U);
    ASSERT_STREQ(str, "ab");

    cstring_assign(str, "  ab", strlen_of("  ab"));
    cstring_trim(str, ' ', 2);
    ASSERT_EQ(cstring_size(str), 4U);
    ASSERT_STREQ(str, "  ab");
    cstring_trim(str, ' ', 3);
    ASSERT_EQ(cstring_size(str), 2U);
    ASSERT_STREQ(str, "ab");

    cstring_assign(str, "  ", strlen_of("  "));
    cstring_trim(str, ' ', 1);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_STREQ(str, "");
    cstring_assign(str, "  ", strlen_of("  "));
    cstring_trim(str, ' ', 2);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_STREQ(str, "");
    cstring_assign(str, "  ", strlen_of("  "));
    cstring_trim(str, ' ', 3);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_STREQ(str, "");

    cstring_assign(str, "ab", strlen_of("ab"));
    cstring_trim(str, ' ', 3);
    ASSERT_EQ(cstring_size(str), 2U);
    ASSERT_STREQ(str, "ab");
    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"  ab  ", strlen_of(L"  ab  "));
    cstring_trim(wstr, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 2U);
    ASSERT_TRUE(wcseq(wstr, L"ab"));

    cstring_assign(wstr, L"ab  ", strlen_of(L"ab  "));
    cstring_trim(wstr, L' ', 1);
    ASSERT_EQ(cstring_size(wstr), 4U);
    ASSERT_TRUE(wcseq(wstr, L"ab  "));
    cstring_trim(wstr, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 2U);
    ASSERT_TRUE(wcseq(wstr, L"ab"));

    cstring_assign(wstr, L"  ab", strlen_of(L"  ab"));
    cstring_trim(wstr, L' ', 2);
    ASSERT_EQ(cstring_size(wstr), 4U);
    ASSERT_TRUE(wcseq(wstr, L"  ab"));
    cstring_trim(wstr, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 2U);
    ASSERT_TRUE(wcseq(wstr, L"ab"));

    cstring_assign(wstr, L"  ", strlen_of(L"  "));
    cstring_trim(wstr, L' ', 1);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_TRUE(wcseq(wstr, L""));
    cstring_assign(wstr, L"  ", strlen_of(L"  "));
    cstring_trim(wstr, L' ', 2);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_TRUE(wcseq(wstr, L""));
    cstring_assign(wstr, L"  ", strlen_of(L"  "));
    cstring_trim(wstr, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 0U);
    ASSERT_TRUE(wcseq(wstr, L""));

    cstring_assign(wstr, L"ab", strlen_of(L"ab"));
    cstring_trim(wstr, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 2U);
    ASSERT_TRUE(wcseq(wstr, L"ab"));
    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_trim(nullstr, ' ', 3);
    ASSERT_EQ(cstring_size(nullstr), 0U);
    ASSERT_EQ(nullstr, NULL);
}

UTEST(test, cstring_fix) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abc", 3);
    cstring_fix(str, 5, ' ', 1);
    ASSERT_EQ(cstring_size(str), 5U);
    ASSERT_STREQ(str, "  abc");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 5, ' ', 2);
    ASSERT_EQ(cstring_size(str), 5U);
    ASSERT_STREQ(str, "abc  ");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 5, ' ', 3);
    ASSERT_EQ(cstring_size(str), 5U);
    ASSERT_STREQ(str, " abc ");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 6, ' ', 3);
    ASSERT_EQ(cstring_size(str), 6U);
    ASSERT_STREQ(str, " abc  ");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 1, ' ', 1);
    ASSERT_EQ(cstring_size(str), 1U);
    ASSERT_STREQ(str, "c");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 1, ' ', 2);
    ASSERT_EQ(cstring_size(str), 1U);
    ASSERT_STREQ(str, "a");

    cstring_assign(str, "abc", 3);
    cstring_fix(str, 1, ' ', 3);
    ASSERT_EQ(cstring_size(str), 1U);
    ASSERT_STREQ(str, "b");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 5, L' ', 1);
    ASSERT_EQ(cstring_size(wstr), 5U);
    ASSERT_TRUE(wcseq(wstr, L"  abc"));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 5, L' ', 2);
    ASSERT_EQ(cstring_size(wstr), 5U);
    ASSERT_TRUE(wcseq(wstr, L"abc  "));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 5, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 5U);
    ASSERT_TRUE(wcseq(wstr, L" abc "));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 6, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 6U);
    ASSERT_TRUE(wcseq(wstr, L" abc  "));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 1, L' ', 1);
    ASSERT_EQ(cstring_size(wstr), 1U);
    ASSERT_TRUE(wcseq(wstr, L"c"));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 1, L' ', 2);
    ASSERT_EQ(cstring_size(wstr), 1U);
    ASSERT_TRUE(wcseq(wstr, L"a"));

    cstring_assign(wstr, L"abc", 3);
    cstring_fix(wstr, 1, L' ', 3);
    ASSERT_EQ(cstring_size(wstr), 1U);
    ASSERT_TRUE(wcseq(wstr, L"b"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_assign(str, "", 0);
    cstring_fix(str, 5, ' ', 3);
    ASSERT_EQ(cstring_size(str), 5U);
    ASSERT_STREQ(str, "     ");

    cstring_assign(str, "x", 1);
    cstring_fix(str, 0, ' ', 3);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_STREQ(str, "");
    cstring_free(str);

    cstring_string_type(char) nullstr = NULL;
    cstring_fix(nullstr, 5, ' ', 3);
    ASSERT_EQ(cstring_size(nullstr), 0U);
    ASSERT_EQ(nullstr, NULL);
}

UTEST(test, cstring_reverse) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abc", 3);
    cstring_reverse(str);
    ASSERT_EQ(cstring_size(str), 3U);
    ASSERT_STREQ(str, "cba");

    cstring_assign(str, "ab", 2);
    cstring_reverse(str);
    ASSERT_EQ(cstring_size(str), 2U);
    ASSERT_STREQ(str, "ba");

    cstring_assign(str, "a", 1);
    cstring_reverse(str);
    ASSERT_EQ(cstring_size(str), 1U);
    ASSERT_STREQ(str, "a");
    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abc", 3);
    cstring_reverse(wstr);
    ASSERT_EQ(cstring_size(wstr), 3U);
    ASSERT_TRUE(wcseq(wstr, L"cba"));

    cstring_assign(wstr, L"ab", 2);
    cstring_reverse(wstr);
    ASSERT_EQ(cstring_size(wstr), 2U);
    ASSERT_TRUE(wcseq(wstr, L"ba"));

    cstring_assign(wstr, L"a", 1);
    cstring_reverse(wstr);
    ASSERT_EQ(cstring_size(wstr), 1U);
    ASSERT_TRUE(wcseq(wstr, L"a"));
    cstring_free(wstr);

    /* -- special cases -- */

    cstring_assign(str, "", 0);
    cstring_reverse(str);
    ASSERT_EQ(cstring_size(str), 0U);
    ASSERT_STREQ(str, "");
    cstring_free(str);

    cstring_string_type(char) nullstr = NULL;
    cstring_reverse(nullstr);
    ASSERT_EQ(cstring_size(nullstr), 0U);
    ASSERT_EQ(nullstr, NULL);
}

UTEST(test, cstring_find_rfind) {
    ptrdiff_t off;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abcdefghabcdefgh", 16);

    cstring_find(str, 0, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(str, 5, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(str, 6, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(str, 7, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find(str, 8, "h", 1, off);
    ASSERT_EQ(off, 15);

    cstring_rfind(str, -1, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(str, 15, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(str, 14, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(str, 13, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_rfind(str, 13, "h", 1, off);
    ASSERT_EQ(off, 7);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abcdefghabcdefgh", 16);

    cstring_find(wstr, 0, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(wstr, 5, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(wstr, 6, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find(wstr, 7, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find(wstr, 8, L"h", 1, off);
    ASSERT_EQ(off, 15);

    cstring_rfind(wstr, -1, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(wstr, 15, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(wstr, 14, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_rfind(wstr, 13, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_rfind(wstr, 13, L"h", 1, off);
    ASSERT_EQ(off, 7);

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_assign(str, "x", 0);
    cstring_find(str, 0, "", 0, off);
    ASSERT_EQ(off, -1);
    cstring_rfind(str, -1, "", 0, off);
    ASSERT_EQ(off, -1);

    cstring_assign(str, "", 0);
    cstring_find(str, 0, "", 0, off);
    ASSERT_EQ(off, -1);
    cstring_rfind(str, -1, "", 0, off);
    ASSERT_EQ(off, -1);
    cstring_free(str);

    cstring_string_type(char) nullstr = NULL;
    cstring_find(nullstr, 0, "", 0, off);
    ASSERT_EQ(off, -1);
    cstring_rfind(nullstr, -1, "", 0, off);
    ASSERT_EQ(off, -1);
}

UTEST(test, cstring_find_first_of) {
    ptrdiff_t off;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abcdefghabcdefgh", 16);

    cstring_find_first_of(str, 0, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find_first_of(str, 6, "gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find_first_of(str, 7, "gh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_first_of(str, 8, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find_first_of(str, 8, "h", 1, off);
    ASSERT_EQ(off, 15);
    cstring_find_first_of(str, 0, "xh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_first_of(str, 0, "xy", 2, off);
    ASSERT_EQ(off, -1);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abcdefghabcdefgh", 16);

    cstring_find_first_of(wstr, 0, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find_first_of(wstr, 6, L"gh", 2, off);
    ASSERT_EQ(off, 6);
    cstring_find_first_of(wstr, 7, L"gh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_first_of(wstr, 8, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find_first_of(wstr, 8, L"h", 1, off);
    ASSERT_EQ(off, 15);
    cstring_find_first_of(wstr, 0, L"xh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_first_of(wstr, 0, L"xy", 2, off);
    ASSERT_EQ(off, -1);

    cstring_free(wstr);
}

UTEST(test, cstring_find_first_not_of) {
    ptrdiff_t off;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abcdefghabcdefgh", 16);

    cstring_find_first_not_of(str, 0, "gh", 2, off);
    ASSERT_EQ(off, 0);
    cstring_find_first_not_of(str, 5, "gh", 2, off);
    ASSERT_EQ(off, 5);
    cstring_find_first_not_of(str, 6, "gh", 2, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(str, 7, "gh", 2, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(str, 7, "h", 1, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(str, 0, "abcdefgh", 8, off);
    ASSERT_EQ(off, -1);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abcdefghabcdefgh", 16);

    cstring_find_first_not_of(wstr, 0, L"gh", 2, off);
    ASSERT_EQ(off, 0);
    cstring_find_first_not_of(wstr, 5, L"gh", 2, off);
    ASSERT_EQ(off, 5);
    cstring_find_first_not_of(wstr, 6, L"gh", 2, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(wstr, 7, L"gh", 2, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(wstr, 7, L"h", 1, off);
    ASSERT_EQ(off, 8);
    cstring_find_first_not_of(wstr, 0, L"abcdefgh", 8, off);
    ASSERT_EQ(off, -1);

    cstring_free(wstr);
}

UTEST(test, cstring_find_last_of) {
    ptrdiff_t off;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abcdefghabcdefgh", 16);

    cstring_find_last_of(str, -1, "gh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(str, 15, "gh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(str, 14, "gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find_last_of(str, 13, "gh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_last_of(str, 13, "h", 1, off);
    ASSERT_EQ(off, 7);
    cstring_find_last_of(str, -1, "xh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(str, -1, "xy", 2, off);
    ASSERT_EQ(off, -1);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abcdefghabcdefgh", 16);

    cstring_find_last_of(wstr, -1, L"gh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(wstr, 15, L"gh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(wstr, 14, L"gh", 2, off);
    ASSERT_EQ(off, 14);
    cstring_find_last_of(wstr, 13, L"gh", 2, off);
    ASSERT_EQ(off, 7);
    cstring_find_last_of(wstr, 13, L"h", 1, off);
    ASSERT_EQ(off, 7);
    cstring_find_last_of(wstr, -1, L"xh", 2, off);
    ASSERT_EQ(off, 15);
    cstring_find_last_of(wstr, -1, L"xy", 2, off);
    ASSERT_EQ(off, -1);

    cstring_free(wstr);
}

UTEST(test, cstring_find_last_not_of) {
    ptrdiff_t off;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, "abcdefghabcdefgh", 16);

    cstring_find_last_not_of(str, -1, "gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(str, 15, "gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(str, 14, "gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(str, 13, "gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(str, 12, "gh", 2, off);
    ASSERT_EQ(off, 12);
    cstring_find_last_not_of(str, 12, "h", 1, off);
    ASSERT_EQ(off, 12);
    cstring_find_last_not_of(str, -1, "abcdefgh", 8, off);
    ASSERT_EQ(off, -1);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, L"abcdefghabcdefgh", 16);

    cstring_find_last_not_of(wstr, -1, L"gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(wstr, 15, L"gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(wstr, 14, L"gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(wstr, 13, L"gh", 2, off);
    ASSERT_EQ(off, 13);
    cstring_find_last_not_of(wstr, 12, L"gh", 2, off);
    ASSERT_EQ(off, 12);
    cstring_find_last_not_of(wstr, 12, L"h", 1, off);
    ASSERT_EQ(off, 12);
    cstring_find_last_not_of(wstr, -1, L"abcdefgh", 8, off);
    ASSERT_EQ(off, -1);

    cstring_free(wstr);
}

UTEST(test, cstring_compare) {
    int res                        = 999;
    cstring_string_type(char) str1 = NULL;
    cstring_string_type(char) str2 = NULL;

    cstring_assign(str1, "abc", 3);
    cstring_assign(str2, "abc", 3);
    cstring_compare(str1, str2, res);
    ASSERT_EQ(res, 0);

    str1[2] = 'x';
    cstring_compare(str1, str2, res);
    ASSERT_EQ(res, 1);

    cstring_pop_back(str1);
    cstring_compare(str1, str2, res);
    ASSERT_EQ(res, -1);

    cstring_assign(str1, "\xFF", 1);
    cstring_compare(str1, str2, res);
    ASSERT_EQ(res, 1);

    cstring_assign(str1, "", 0);
    cstring_assign(str2, "", 0);
    cstring_compare(str1, str2, res);
    ASSERT_EQ(res, 0);

    cstring_free(str2);
    cstring_free(str1);

    /* -- wide string -- */

    res                                = 999;
    cstring_string_type(wchar_t) wstr1 = NULL;
    cstring_string_type(wchar_t) wstr2 = NULL;
    cstring_assign(wstr1, L"abc", 3);
    cstring_assign(wstr2, L"abc", 3);

    cstring_compare(wstr1, wstr2, res);
    ASSERT_EQ(res, 0);

    wstr1[2] = 'x';
    cstring_compare(wstr1, wstr2, res);
    ASSERT_EQ(res, 1);

    cstring_pop_back(wstr1);
    cstring_compare(wstr1, wstr2, res);
    ASSERT_EQ(res, -1);

    cstring_assign(wstr1, L"\xFF", 1);
    cstring_compare(wstr1, wstr2, res);
    ASSERT_EQ(res, 1);

    cstring_assign(wstr1, L"", 0);
    cstring_assign(wstr2, L"", 0);
    cstring_compare(wstr1, wstr2, res);
    ASSERT_EQ(res, 0);

    cstring_free(wstr2);
    cstring_free(wstr1);

    /* -- special cases -- */

    res                                = 999;
    cstring_string_type(char) nullstr1 = NULL;
    cstring_string_type(char) nullstr2 = NULL;
    cstring_compare(nullstr1, nullstr2, res);
    ASSERT_EQ(res, 999);
}

UTEST(test, cstring_starts_ends_with) {
    int res;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_starts_with(str, "a", 1, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(str, "ab", 2, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(str, "abcde", 5, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(str, "b", 1, res);
    ASSERT_EQ(res, 0);

    cstring_ends_with(str, "e", 1, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(str, "de", 2, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(str, "abcde", 5, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(str, "d", 1, res);
    ASSERT_EQ(res, 0);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_starts_with(wstr, L"a", 1, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(wstr, L"ab", 2, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(wstr, L"abcde", 5, res);
    ASSERT_EQ(res, 1);
    cstring_starts_with(wstr, L"b", 1, res);
    ASSERT_EQ(res, 0);

    cstring_ends_with(wstr, L"e", 1, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(wstr, L"de", 2, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(wstr, L"abcde", 5, res);
    ASSERT_EQ(res, 1);
    cstring_ends_with(wstr, L"d", 1, res);
    ASSERT_EQ(res, 0);

    cstring_free(wstr);

    /* -- special cases -- */

    res                               = 999;
    cstring_string_type(char) nullstr = NULL;
    cstring_starts_with(nullstr, "x", 1, res);
    ASSERT_EQ(res, 0);
    cstring_ends_with(nullstr, L"x", 1, res);
    ASSERT_EQ(res, 0);
}

UTEST(test, cstring_contains) {
    int res;
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_contains(str, "de", 2, res);
    ASSERT_EQ(res, 1);
    cstring_contains(str, "ed", 2, res);
    ASSERT_EQ(res, 0);

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_contains(wstr, L"de", 2, res);
    ASSERT_EQ(res, 1);
    cstring_contains(wstr, L"ed", 2, res);
    ASSERT_EQ(res, 0);

    cstring_free(wstr);
}

UTEST(test, cstring_substring) {
    cstring_string_type(char) str    = NULL;
    cstring_string_type(char) substr = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_substring(str, 1, 3, substr);
    ASSERT_EQ(cstring_size(substr), 3U);
    ASSERT_STREQ(substr, "bcd");

    cstring_free(substr);
    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr    = NULL;
    cstring_string_type(wchar_t) wsubstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_substring(wstr, 1, 3, wsubstr);
    ASSERT_EQ(cstring_size(wsubstr), 3U);
    ASSERT_TRUE(wcseq(wsubstr, L"bcd"));

    cstring_free(wsubstr);
    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_substring(nullstr, 0, 0, substr);
    ASSERT_EQ(nullstr, NULL);
    ASSERT_EQ(substr, NULL);

    cstring_init(zerolenstr, char);
    cstring_substring(zerolenstr, 1, 0, substr);
    ASSERT_EQ(substr, NULL);

    cstring_substring(zerolenstr, 0, 0, substr);
    ASSERT_STREQ(substr, "");
    cstring_free(zerolenstr);

    cstring_assign(str, literal, strlen_of(literal));
    cstring_assign(substr, literal, strlen_of(literal));
    cstring_substring(str, 1, 1000, substr);
    ASSERT_EQ(cstring_size(substr), 4U);
    ASSERT_STREQ(substr, "bcde");

    cstring_assign(substr, "a", 1);
    cstring_substring(str, 1, 1000, substr);
    ASSERT_EQ(cstring_size(substr), 4U);
    ASSERT_STREQ(substr, "bcde");

    cstring_free(substr);
    cstring_free(str);
}

UTEST(test, cstring_split) {
    int i;
    cstring_string_type(char) str = NULL;
    cstring_array_type(char) arr  = NULL;

    cstring_assign(str, "a;b;c", 5);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 3U);
    ASSERT_EQ(cstring_capacity(arr), 63U);
    ASSERT_EQ(cstring_size(arr[0]), 1U);
    ASSERT_STREQ(arr[0], "a");
    ASSERT_EQ(cstring_size(arr[1]), 1U);
    ASSERT_STREQ(arr[1], "b");
    ASSERT_EQ(cstring_size(arr[2]), 1U);
    ASSERT_STREQ(arr[2], "c");

    cstring_split(str, 2, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 2U);
    ASSERT_EQ(cstring_size(arr[0]), 1U);
    ASSERT_STREQ(arr[0], "a");
    ASSERT_EQ(cstring_size(arr[1]), 3U);
    ASSERT_STREQ(arr[1], "b;c");

    cstring_split(str, 1000, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 3U);
    ASSERT_EQ(cstring_size(arr[0]), 1U);
    ASSERT_STREQ(arr[0], "a");
    ASSERT_EQ(cstring_size(arr[1]), 1U);
    ASSERT_STREQ(arr[1], "b");
    ASSERT_EQ(cstring_size(arr[2]), 1U);
    ASSERT_STREQ(arr[2], "c");

    cstring_assign(str, ";;;;", 4);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 5U);
    for (i = 0; i < 5; ++i) {
        ASSERT_EQ(cstring_size(arr[i]), 0U);
        ASSERT_STREQ(arr[i], "");
    }

    cstring_assign(str, ";b;", 3);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 3U);
    ASSERT_EQ(cstring_size(arr[0]), 0U);
    ASSERT_STREQ(arr[0], "");
    ASSERT_EQ(cstring_size(arr[1]), 1U);
    ASSERT_STREQ(arr[1], "b");
    ASSERT_EQ(cstring_size(arr[2]), 0U);
    ASSERT_STREQ(arr[2], "");

    cstring_assign(str, "abc", 3);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 1U);
    ASSERT_EQ(cstring_size(arr[0]), 3U);
    ASSERT_STREQ(arr[0], "abc");

    cstring_assign(str, "abc;defg", 8);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 2U);
    ASSERT_EQ(cstring_size(arr[0]), 3U);
    ASSERT_STREQ(arr[0], "abc");
    ASSERT_EQ(cstring_size(arr[1]), 4U);
    ASSERT_STREQ(arr[1], "defg");

    cstring_assign(str, "a;;b;;c", 7);
    cstring_split(str, -1, ";", 1, arr);
    ASSERT_EQ(cstring_size(arr), 5U);
    ASSERT_EQ(cstring_size(arr[0]), 1U);
    ASSERT_STREQ(arr[0], "a");
    ASSERT_EQ(cstring_size(arr[1]), 0U);
    ASSERT_STREQ(arr[1], "");
    ASSERT_EQ(cstring_size(arr[2]), 1U);
    ASSERT_STREQ(arr[2], "b");
    ASSERT_EQ(cstring_size(arr[3]), 0U);
    ASSERT_STREQ(arr[3], "");
    ASSERT_EQ(cstring_size(arr[4]), 1U);
    ASSERT_STREQ(arr[4], "c");

    cstring_split(str, -1, ";;", 2, arr);
    ASSERT_EQ(cstring_size(arr), 3U);
    ASSERT_EQ(cstring_size(arr[0]), 1U);
    ASSERT_STREQ(arr[0], "a");
    ASSERT_EQ(cstring_size(arr[1]), 1U);
    ASSERT_STREQ(arr[1], "b");
    ASSERT_EQ(cstring_size(arr[2]), 1U);
    ASSERT_STREQ(arr[2], "c");

    cstring_array_free(arr);
    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_array_type(wchar_t) warr  = NULL;

    cstring_assign(wstr, L"a;b;c", 5);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 3U);
    ASSERT_EQ(cstring_capacity(warr), 63U);
    ASSERT_EQ(cstring_size(warr[0]), 1U);
    ASSERT_TRUE(wcseq(warr[0], L"a"));
    ASSERT_EQ(cstring_size(warr[1]), 1U);
    ASSERT_TRUE(wcseq(warr[1], L"b"));
    ASSERT_EQ(cstring_size(warr[2]), 1U);
    ASSERT_TRUE(wcseq(warr[2], L"c"));

    cstring_split(wstr, 2, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 2U);
    ASSERT_EQ(cstring_size(warr[0]), 1U);
    ASSERT_TRUE(wcseq(warr[0], L"a"));
    ASSERT_EQ(cstring_size(warr[1]), 3U);
    ASSERT_TRUE(wcseq(warr[1], L"b;c"));

    cstring_split(wstr, 1000, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 3U);
    ASSERT_EQ(cstring_size(warr[0]), 1U);
    ASSERT_TRUE(wcseq(warr[0], L"a"));
    ASSERT_EQ(cstring_size(warr[1]), 1U);
    ASSERT_TRUE(wcseq(warr[1], L"b"));
    ASSERT_EQ(cstring_size(warr[2]), 1U);
    ASSERT_TRUE(wcseq(warr[2], L"c"));

    cstring_assign(wstr, L";;;;", 4);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 5U);
    for (i = 0; i < 5; ++i) {
        ASSERT_EQ(cstring_size(warr[i]), 0U);
        ASSERT_TRUE(wcseq(warr[i], L""));
    }

    cstring_assign(wstr, L";b;", 3);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 3U);
    ASSERT_EQ(cstring_size(warr[0]), 0U);
    ASSERT_TRUE(wcseq(warr[0], L""));
    ASSERT_EQ(cstring_size(warr[1]), 1U);
    ASSERT_TRUE(wcseq(warr[1], L"b"));
    ASSERT_EQ(cstring_size(warr[2]), 0U);
    ASSERT_TRUE(wcseq(warr[2], L""));

    cstring_assign(wstr, L"abc", 3);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 1U);
    ASSERT_EQ(cstring_size(warr[0]), 3U);
    ASSERT_TRUE(wcseq(warr[0], L"abc"));

    cstring_assign(wstr, L"abc;defg", 8);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 2U);
    ASSERT_EQ(cstring_size(warr[0]), 3U);
    ASSERT_TRUE(wcseq(warr[0], L"abc"));
    ASSERT_EQ(cstring_size(warr[1]), 4U);
    ASSERT_TRUE(wcseq(warr[1], L"defg"));

    cstring_assign(wstr, L"a;;b;;c", 7);
    cstring_split(wstr, -1, L";", 1, warr);
    ASSERT_EQ(cstring_size(warr), 5U);
    ASSERT_EQ(cstring_size(warr[0]), 1U);
    ASSERT_TRUE(wcseq(warr[0], L"a"));
    ASSERT_EQ(cstring_size(warr[1]), 0U);
    ASSERT_TRUE(wcseq(warr[1], L""));
    ASSERT_EQ(cstring_size(warr[2]), 1U);
    ASSERT_TRUE(wcseq(warr[2], L"b"));
    ASSERT_EQ(cstring_size(warr[3]), 0U);
    ASSERT_TRUE(wcseq(warr[3], L""));
    ASSERT_EQ(cstring_size(warr[4]), 1U);
    ASSERT_TRUE(wcseq(warr[4], L"c"));

    cstring_split(wstr, -1, L";;", 2, warr);
    ASSERT_EQ(cstring_size(warr), 3U);
    ASSERT_EQ(cstring_size(warr[0]), 1U);
    ASSERT_TRUE(wcseq(warr[0], L"a"));
    ASSERT_EQ(cstring_size(warr[1]), 1U);
    ASSERT_TRUE(wcseq(warr[1], L"b"));
    ASSERT_EQ(cstring_size(warr[2]), 1U);
    ASSERT_TRUE(wcseq(warr[2], L"c"));

    cstring_array_free(warr);
    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_array_type(char) nullarr  = NULL;
    cstring_split(nullstr, -1, L";", 2, nullarr);
    ASSERT_EQ(nullarr, NULL);
}

UTEST_MAIN()
