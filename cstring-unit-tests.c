#include "cstring.h"
#include "utest/utest.h"
#include <wchar.h>

static const char literal[]     = "abcde";
static const wchar_t wliteral[] = L"abcde";

#define strlen_of(lit) \
    (sizeof(lit) / sizeof((lit)[0]) - 1)

#define wcseq(s1, s2) \
    (wcscmp((s1), (s2)) == 0)

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
    ASSERT_EQ(nullstr, NULL);
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
    ASSERT_EQ(nullstr, NULL);

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
}

UTEST(test, cstring_resize) {
    cstring_string_type(char) str = NULL;
    cstring_assign(str, literal, strlen_of(literal));

    cstring_resize(str, 10, 'x');
    ASSERT_EQ(cstring_size(str), 10U);
    ASSERT_STREQ(str, "abcdexxxxx");

    cstring_resize(str, 4, 'x');
    ASSERT_EQ(cstring_size(str), 4U);
    ASSERT_STREQ(str, "abcd");

    cstring_free(str);

    /* -- wide string -- */

    cstring_string_type(wchar_t) wstr = NULL;
    cstring_assign(wstr, wliteral, strlen_of(wliteral));

    cstring_resize(wstr, 10, L'x');
    ASSERT_EQ(cstring_size(wstr), 10U);
    ASSERT_TRUE(wcseq(wstr, L"abcdexxxxx"));

    cstring_resize(wstr, 4, L'x');
    ASSERT_EQ(cstring_size(wstr), 4U);
    ASSERT_TRUE(wcseq(wstr, L"abcd"));

    cstring_free(wstr);

    /* -- special cases -- */

    cstring_string_type(char) nullstr = NULL;
    cstring_resize(nullstr, 10, 'x');
    ASSERT_EQ(nullstr, NULL);

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
    cstring_free(substr);
    cstring_free(zerolenstr);
}

UTEST_MAIN()
