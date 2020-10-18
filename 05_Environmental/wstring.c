#include "wstring.h"
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>

Wstring* new_wstring() {
    Wstring* wstr = malloc(sizeof(*wstr));
    assert(wstr != NULL);
    wstr->len = 0;
    wstr->cap = 1;
    wstr->buf = calloc(wstr->cap, sizeof(*wstr->buf));
    assert(wstr->buf != NULL);
    return wstr;
}

void push_wstring(Wstring* wstr, wchar_t wc) {
    if (wstr->len + 1 >= wstr->cap) {
        wstr->cap *= 2;
        assert(wstr->len + 1 < wstr->cap);
        wstr->buf = realloc(wstr->buf, sizeof(*wstr->buf) * wstr->cap);
        assert(wstr->buf != NULL);
    }
    wstr->buf[wstr->len++] = wc;
    wstr->buf[wstr->len] = '\0';
}

void pop_wstring(Wstring* wstr) {
    if (wstr->len > 0) {
        wstr->buf[--wstr->len] = L'\0';
        if (wstr->len <= wstr->cap / 4) {
            wstr->cap /= 4;
            if (wstr->len + 1 > wstr->cap) {
                wstr->cap = wstr->len + 1;
            }
            wstr->buf = realloc(wstr->buf, sizeof(*wstr->buf) * wstr->cap);
        }
    }
}

size_t len_wstring(Wstring* wstr) { return wcslen(wstr->buf); }

void destroy_wstring(Wstring* wstr) {
    free(wstr->buf);
    free(wstr);
}
