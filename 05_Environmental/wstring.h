#ifndef WSTRING_H_
#define WSTRING_H_
#include <wchar.h>

typedef struct Wstring {
    wchar_t* buf;
    size_t len;
    size_t cap;
} Wstring;

Wstring* new_wstring();
void push_wstring(Wstring* wstr, wchar_t wc);
void pop_wstring(Wstring* wstr);
size_t len_wstring(Wstring* wstr);
void destroy_wstring(Wstring* wstr);

#endif // WSTRING_H_
