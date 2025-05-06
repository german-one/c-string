This is a type-safe, header-only implementation of a `std::basic_string`-like string in plain C code.  
It can be considered an offshoot of the [c-vector](https://github.com/eteran/c-vector) library and is intended to be binary compatible with it. Many sequences of the `c-string` library essentially correspond to the `c-vector` library code. All credits go to [eteran](https://github.com/eteran) and contributors.  
While the `c-vector` library implements macros to emulate methods of a `std::vector`, the `c-string` library is specialized for null-terminated strings of characters. Its macros emulate `std::basic_string` methods. A few additional features are implemented that don't have an equivalent for a `std::basic_string`.  
The `cstring_array` API extension is for vectors of `cstring`. It supports tokenization (also known as `split`) and concatenation (also known as `join`).  
  
- Include __cstring.h__ to use the API. No other file of this repository is necessary.  
- Besides of containing the code for tests, the __cstring-unit-tests.c__ is also meant to provide some examples of how to use the API.  
- PDF prints of the relevant parts of a Doxygen-generated manual can be found in the __doc__ folder.  
  
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
  
----
  
### cstring API

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
| [`std::basic_string<type>::max_size()`](https://en.cppreference.com/w/cpp/string/basic_string/max_size) | `cstring_max_size(type)` |
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
| [`offset = str.find(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/find) | `cstring_find(str, pos, s, count, offset)` |
| [`offset = str.rfind(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/rfind) | `cstring_rfind(str, pos, s, count, offset)` |
| [`offset = str.find_first_of(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/find_first_of) | `cstring_find_first_of(str, pos, s, count, offset)` |
| [`offset = str.find_first_not_of(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/find_first_not_of) | `cstring_find_first_not_of(str, pos, s, count, offset)` |
| [`offset = str.find_last_of(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/find_last_of) | `cstring_find_last_of(str, pos, s, count, offset)` |
| [`offset = str.find_last_not_of(s, pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/find_last_not_of) | `cstring_find_last_not_of(str, pos, s, count, offset)` |
| [`order = str1.compare(str2)`](https://en.cppreference.com/w/cpp/string/basic_string/compare) | `cstring_compare(str1, str2, order)` |
| [`found = str.starts_with(s)`](https://en.cppreference.com/w/cpp/string/basic_string/starts_with) | `cstring_starts_with(str, s, count, found)` |
| [`found = str.ends_with(s)`](https://en.cppreference.com/w/cpp/string/basic_string/ends_with) | `cstring_ends_with(str, s, count, found)` |
| [`found = str.contains(s)`](https://en.cppreference.com/w/cpp/string/basic_string/contains) | `cstring_ends_with(str, s, count, found)` |
| [`to = from.substring(pos, count)`](https://en.cppreference.com/w/cpp/string/basic_string/substring) | `cstring_substring(from, pos, count, to)` |
  
----
  
### cstring_array API  
A `cstring_array` is a NULL pointer-terminated vector of `cstring`.  
The concept is similar to the vector of arguments in `int main(int argc, char *argv[])`. While `argv` is guaranteed to be NULL pointer-terminated, this terminator is not counted in `argc`.  
Like a `cstring`, a `cstring_array` is a heap-allocated and metadata-prefixed object.  
  
| **Macro** | **Description** |
| --------- | --------------- |
| `cstring_array_type(type) arr = NULL` | Declare a vector of `cstring` using the specified character type. |
| `cstring_split(str, max_tok, ptr, count, ret_array)` | Tokenize `str` into a vector of `cstring`. |
| `cstring_array_free(arr)` | Recursively free all memory associated with the vector. |
| `cstring_array_at(arr, pos)` | Return the string pointer at position `pos` in the cstring_array. |
| `cstring_array_front(arr)` | Return the string pointer to the first string in the cstring_array. |
| `cstring_array_back(arr)` | Return the string pointer to the last string in the cstring_array. |
| `cstring_array_begin(arr)` | Return an iterator to first string of the vector. |
| `cstring_array_end(arr)` | Return an iterator to one past the last string of the vector. |
| `cstring_array_empty(arr)` | Return non-zero if the vector is empty. |
| `cstring_array_size(arr)` | Get the current length of the vector. |
| `cstring_array_max_size(type)` | Get the maximum number of elements a vector of strings of the specified character type is able to hold. |
| `cstring_array_reserve(arr, n)` | Request that the vector capacity be at least enough to contain `n` strings. |
| `cstring_array_capacity(arr)` | Get the current capacity of the vector. |
| `cstring_array_shrink_to_fit(arr)` | Request the container to reduce its capacity to fit its size. |
| `cstring_array_clear(arr)` | Erase all of the strings in the vector. |
| `cstring_array_insert(arr, pos, ptr, count)` | Insert a string at position `pos` into the vector. |
| `cstring_array_erase(arr, pos, n)` | Remove the strings beginning at offset `pos` from the cstring_array. |
| `cstring_array_push_back(arr, ptr, count)` | Add a string to the end of the vector. |
| `cstring_array_pop_back(arr)` | Remove the last string from the cstring_array. |
| `cstring_array_copy(from, to)` | Copy a cstring_array. |
| `cstring_array_resize(arr, n, ptr, count)` | Resize the container to contain `count` strings. |
| `cstring_array_swap(arr, other)` | Exchange the content of the cstring_array by the content of another cstring_array of the same type. |
| `cstring_array_slice(from, pos, n, to)` | Copy a part of a vector. |
| `cstring_array_join(arr, ptr, count, ret_str)` | Concatenate the strings of a vector using the specified joiner. |
  
----
  
Neil Henning's [unit test header](https://github.com/sheredom/utest.h) is used to verify the proper functionality of all API macros.  
  
----
  
[^1]: Initializes a NULL string. Nothing similar for `std::basic_string`.  
[^2]: Declares a static cstring literal of `const type`. This is comparable with a C++20 `constexpr std::basic_string`.  
[^3]: Allocates and initializes a zero-length string.  
[^4]: Manually updates the size property after an update of the string buffer in a third party API.  
[^5]: This creates a duplicate of the string. For copying a substring see `cstring_substring()`.  
[^6]: Removes contiguous occurrences of the specified character from the begin and/or the end of a cstring.  
[^7]: Updates the cstring to a fixed length by either padding or shortening.  
[^8]: Reverses the character order in the cstring.  
  
