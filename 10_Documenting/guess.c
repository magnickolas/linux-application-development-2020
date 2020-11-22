#include <libintl.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "config.h"

#define FROM 1
#define TO 100
#define MANUAL _("Guess is used for guessing user's number from 1 to 100\n\
\n\
Usage: guess\n\
\n\
    --help		print this help, then exit\n\
    --version		print version number, then exit\n\
    -r			use Roman numerals\n\
\n\
Guess uses binary search to guess user's number and asks questions\n\
\n\
")
/** @file guess.c
 * Guess program
 *
 */

#define _(STRING) gettext(STRING)

const char* ROMANS[] = {
    "",       "I",       "II",     "III",     "IV",       "V",      "VI",
    "VII",    "VIII",    "IX",     "X",       "XI",       "XII",    "XIII",
    "XIV",    "XV",      "XVI",    "XVII",    "XVIII",    "XIX",    "XX",
    "XXI",    "XXII",    "XXIII",  "XXIV",    "XXV",      "XXVI",   "XXVII",
    "XXVIII", "XXIX",    "XXX",    "XXXI",    "XXXII",    "XXXIII", "XXXIV",
    "XXXV",   "XXXVI",   "XXXVII", "XXXVIII", "XXXIX",    "XL",     "XLI",
    "XLII",   "XLIII",   "XLIV",   "XLV",     "XLVI",     "XLVII",  "XLVIII",
    "XLIX",   "L",       "LI",     "LII",     "LIII",     "LIV",    "LV",
    "LVI",    "LVII",    "LVIII",  "LIX",     "LX",       "LXI",    "LXII",
    "LXIII",  "LXIV",    "LXV",    "LXVI",    "LXVII",    "LXVIII", "LXIX",
    "LXX",    "LXXI",    "LXXII",  "LXXIII",  "LXXIV",    "LXXV",   "LXXVI",
    "LXXVII", "LXXVIII", "LXXIX",  "LXXX",    "LXXXI",    "LXXXII", "LXXXIII",
    "LXXXIV", "LXXXV",   "LXXXVI", "LXXXVII", "LXXXVIII", "LXXXIX", "XC",
    "XCI",    "XCII",    "XCIII",  "XCIV",    "XCV",      "XCVI",   "XCVII",
    "XCVIII", "XCIX",    "C"};

/**
 * Converts integer to Roman numeral
 *
 * @param x integer to convert
 * @returns const pointer to corresponding Roman numeral
 */
const char* arabic_to_roman(int x) {
    if (x >= 1 && x <= 100) {
        return ROMANS[x];
    }
    return NULL;
}

/**
 * Converts Roman numeral to integer
 *
 * @param s string containing Roman numeral with null-terminator
 * @returns integer result of conversion
 */
int roman_to_arabic(const char* s) {
    for (int i = 1; i < sizeof(ROMANS) / sizeof(*ROMANS); i++) {
        if (!strcmp(ROMANS[i], s)) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", ".");
    textdomain("guess");

    bool roman = false;

    while (1) {
        static struct option long_options[] = {{"help", no_argument, 0, 0},
                                               {"version", no_argument, 0, 0},
                                               {0, 0, 0, 0}};

        int c = 0;
        int option_index = 0;
        c = getopt_long(argc, argv, "hr", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            switch (option_index) {
            case 0:
                printf("%s\n", MANUAL);
                return 0;
            case 1:
                printf("guess %s\n", VERSION);
                return 0;
            default:
                fprintf(stderr, "%s\n", MANUAL);
                return 1;
            }
            break;
        case 'h':
            printf("%s\n", MANUAL);
            return 0;
        case 'r':
            roman = true;
            break;
        default:
            fprintf(stderr, "%s\n", MANUAL);
            return 1;
        }
    }

    const char* fmt = roman ? "%s" : "%d";
    char num_buf[10];

#define REPR(x)                                                                \
    (roman ? arabic_to_roman(x) : (snprintf(num_buf, 10, "%d", (x)), num_buf))

    printf(_("Think of a number between %s and "), REPR(FROM));
    printf(_("%s.\n"), REPR(TO));

    int left = FROM - 1;
    int right = TO;

    while (right > left + 1) {
        int mid = (left + right) / 2;
        printf(_("Is your number greater than %s?\n"), REPR(mid));
        char ans[10];
        fgets(ans, 10, stdin);
        if (!strcmp(ans, _("yes\n"))) {
            left = mid;
        } else {
            right = mid;
        }
    }
    int answer = right;

    printf(_("Your number is %s!\n"), REPR(answer));
    return 0;
#undef REPR
}
