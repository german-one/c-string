This is a type-safe, header-only implementation of a `std::basic_string`-like string in plain C code.  
It can be considered an offshoot of the [c-vector](https://github.com/eteran/c-vector) library and is intended to be binary compatible with it. Many sequences of the `c-string` library essentially correspond to the `c-vector` library code. All credits go to [eteran](https://github.com/eteran) and contributors.  
While the `c-vector` library implements macros to emulate methods of a `std::vector`, the `c-string` library is specialized for null-terminated strings of characters. Its macros emulate the subset of `std::basic_string` methods that can't be trivially mimicked using functions of the C string library. A few additional features are implemented that don't have an equivalent for a `std::basic_string`.  
  
Just like the `cvector`, a `cstring` is prefixed with metadata, in the tradition of a length-prefixed string implementation.  
The members of the `cstring` metadata are found at the same offset as those of a `cvector`. They count all characters (incl. the terminating null) which makes a `cstring` _interchangeable_ with a `cvector` of the same set of consecutive characters. Unlike the `cvector` macros (and as is usual for strings) the `cstring` macros, which return size and capacity, do not count the string terminator.  
  
The user is given a pointer to the first character of the actual string. This way a `cstring` can be used with the C string and I/O library functions that don't modify its content, as well as whith those of the platform API.  

Example:  

```c
#include "cstring.h"
#include <stdio.h>

int main(void) {
    // Declaration of a null-string. The initialization with NULL is critical.
    cstring_string_type(char) str = NULL;

    // Assign string "abc".
    cstring_assign(str, "abc", 3);
    printf("1)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Append string "hi".
    cstring_append(str, "hi", 2);
    printf("2)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Insert string "defg" at offset 3 (between 'c' and 'h').
    cstring_insert(str, 3, "defg", 4);
    printf("3)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Erase 4 characters at offset 1.
    cstring_erase(str, 1, 4);
    printf("4)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Add an 'x' at the end of the string.
    cstring_push_back(str, 'x');
    printf("5)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Replace two characters at offset 3 with three 'z' characters.
    cstring_replace(str, 3, 2, "zzz", 3);
    printf("6)\nString:   %s\nLength:   %zu\nCapacity: %zu\n\n", str, cstring_length(str), cstring_capacity(str));

    // Copy a 4-character substring from offset 2.
    cstring_string_type(char) str2 = NULL;
    cstring_substring(str, 2, 4, str2);
    printf("7)\nString2:  %s\nLength:   %zu\nCapacity: %zu\n\n", str2, cstring_length(str2), cstring_capacity(str2));

    // Free allocated memory.
    cstring_free(str2);
    cstring_free(str);

    return 0;
}

```

### API

| **std::basic_string** | **cstring** |
| --------------------- | ----------- |
| [`std::basic_string<type> str{};`](https://en.cppreference.com/w/cpp/string/basic_string/basic_string) | `cstring_string_type(type) str = NULL;` [^1],<br>`cstring_literal(name, type, lit)` [^2],<br>`cstring_init(name, type)` [^3] |
| [Destructor](https://en.cppreference.com/w/cpp/string/basic_string/%7Ebasic_string) | `cstring_free(str)` |
| [`str.assign(s, count)`](https://en.cppreference.com/w/cpp/string/basic_string/assign) | `cstring_assign(str, s, count)` |
| [`str.at(pos)`](https://en.cppreference.com/w/cpp/string/basic_string/at) | `cstring_at(str, pos)` |
| [`str[pos]`](https://en.cppreference.com/w/cpp/string/basic_string/operator_at) | `str[pos]` |
| [`str.front()`](https://en.cppreference.com/w/cpp/string/basic_string/front) | `cstring_front(str)` |
| [`str.back()`](https://en.cppreference.com/w/cpp/string/basic_string/back) | `cstring_back(str)` |
| [`str.data()`](https://en.cppreference.com/w/cpp/string/basic_string/data), [`str.c_str()`](https://en.cppreference.com/w/cpp/string/basic_string/c_str) | `str` |
| [`str.begin()`](https://en.cppreference.com/w/cpp/string/basic_string/begin) | `cstring_begin(str)` |
| [`str.end()`](https://en.cppreference.com/w/cpp/string/basic_string/end) | `cstring_end(str)` |
| [`str.empty()`](https://en.cppreference.com/w/cpp/string/basic_string/empty) | `cstring_empty(str)` |
| [`str.size()`, `str.length()`](https://en.cppreference.com/w/cpp/string/basic_string/size) | `cstring_size(str)`, `cstring_length(str)` |
| [`str.reserve(n)`](https://en.cppreference.com/w/cpp/string/basic_string/reserve) | `cstring_reserve(str, n)` |
| [`str.capacity()`](https://en.cppreference.com/w/cpp/string/basic_string/capacity) | `cstring_capacity(str)` |
| [`str.shrink_to_fit()`](https://en.cppreference.com/w/cpp/string/basic_string/shrink_to_fit) | `cstring_shrink_to_fit(str)` |
| N/A | `cstring_unsafe_set_size(str, size)` [^4] |
| [`str.clear()`](https://en.cppreference.com/w/cpp/string/basic_string/clear) | `cstring_clear(str)` |
| [`str.insert(index, s, count)`](https://en.cppreference.com/w/cpp/string/basic_string/insert) | `cstring_insert(str, index, s, count)` |
| [`str.erase(index, count)`](https://en.cppreference.com/w/cpp/string/basic_string/erase) | `cstring_erase(str, index, count)` |
| [`str.push_back(ch)`](https://en.cppreference.com/w/cpp/string/basic_string/push_back) | `cstring_push_back(str, ch)` |
| [`str.pop_back()`](https://en.cppreference.com/w/cpp/string/basic_string/pop_back) | `cstring_pop_back(str)` |
| [`str.append(s, count)`](https://en.cppreference.com/w/cpp/string/basic_string/append) | `cstring_append(str, s, count)` |
| [`str.replace(pos, count, s, count2)`](https://en.cppreference.com/w/cpp/string/basic_string/replace) | `cstring_replace(str, pos, count, s, count2)` |
| [`from.copy(to, npos, 0)`](https://en.cppreference.com/w/cpp/string/basic_string/copy) | `cstring_copy(from, to)` [^5] |
| [`str.resize(count, ch)`](https://en.cppreference.com/w/cpp/string/basic_string/resize) | `cstring_resize(str, count, ch)` |
| [`str.swap(other)`](https://en.cppreference.com/w/cpp/string/basic_string/swap) | `cstring_swap(str, other)` |
| N/A | `cstring_trim(str, value, mode)` [^6] |
| N/A | `cstring_fix(str, length, value, mode)` [^7] |
| N/A | `cstring_reverse(str)` [^8] |
| [`to = from.substring(pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/substring) | `cstring_substring(from, pos, count, to)` |

[^1]: Initializes a NULL string. Nothing similar for `std::basic_string`.  
[^2]: Declares a static cstring literal of `const type`. This is comparable with a C++20 `constexpr std::basic_string`.  
[^3]: Allocates and initializes a zero-length string.  
[^4]: Manually updates the size property after an update of the string buffer in a third party API.  
[^5]: This creates a duplicate of the string. For copying a substring see `cstring_substring()`.  
[^6]: Removes contiguous occurrences of the specified character from the begin and/or the end of a cstring.  
[^7]: Updates the cstring to a fixed length by either padding or shortening.  
[^8]: Reverses the character order in the cstring.  
