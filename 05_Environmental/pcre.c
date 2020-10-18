#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif
#include "pcre.h"
#include <pcre2.h>
#include <stdio.h>
#include <string.h>

char* get_match_groups(const char* pattern_str, const char* subject_str) {
    pcre2_code* re;
    PCRE2_SPTR pattern = (PCRE2_SPTR)pattern_str;
    PCRE2_SPTR subject = (PCRE2_SPTR)subject_str;
    PCRE2_SIZE subject_length = (PCRE2_SIZE)strlen((const char*)subject);

    int errnum;
    int i, rc;

    PCRE2_SIZE erroffs;
    PCRE2_SIZE* ovector;

    pcre2_match_data* match_data;

#ifdef NO_UTF
    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errnum, &erroffs,
                       NULL);
#else
    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, PCRE2_UCP, &errnum,
                       &erroffs, NULL);
#endif

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errnum, buffer, sizeof(buffer));
#define ARGS "PCRE2 compilation failed at offset %d: %s\n", (int)erroffs, buffer
        size_t res_len = snprintf(NULL, 0, ARGS);
        char* res = malloc((res_len + 1) * sizeof(*res));
        snprintf(res, res_len + 1, ARGS);
#undef ARGS
        return res;
    }

    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

    if (rc < 0) {
        size_t res_len;
        char* res;
        switch (rc) {
        case PCRE2_ERROR_NOMATCH:
            res_len = snprintf(NULL, 0, "No match\n");
            res = malloc((res_len + 1) * sizeof(*res));
            snprintf(res, res_len + 1, "No match\n");
            break;
        default:
            res_len = snprintf(NULL, 0, "Matching error %d\n", rc);
            res = malloc((res_len + 1) * sizeof(*res));
            snprintf(res, res_len + 1, "Matching error %d\n", rc);
            break;
        }
        pcre2_match_data_free(
            match_data);     /* Release memory used for the match */
        pcre2_code_free(re); /*   data and the compiled pattern. */
        return res;
    }

    ovector = pcre2_get_ovector_pointer(match_data);

    size_t res_len = 0;
#define ARGS                                                                   \
    "%2ld: %.*s\n", ovector[2 * i],                                            \
        (int)(ovector[2 * i + 1] - ovector[2 * i]), subject + ovector[2 * i]

    for (i = 0; i < rc; i++) {
        res_len += snprintf(NULL, 0, ARGS);
    }
    char* res = malloc((res_len + 1) * sizeof(*res));
    size_t cur_len = 0;

    for (i = 0; i < rc; i++) {
        size_t num = snprintf(NULL, 0, ARGS);
        snprintf(res + cur_len, num + 1, ARGS);
        cur_len += num;
    }
#undef ARGS

    pcre2_match_data_free(match_data); /* Release the memory that was used */
    pcre2_code_free(re);               /* for the match data and the pattern. */

    return res;
}
