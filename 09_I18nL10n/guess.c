#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _(STRING) gettext(STRING)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", ".");
    textdomain("guess");

    const int FROM = 1;
    const int TO = 100;
    printf(_("Think of a number between %d and %d.\n"), FROM, TO);

    int left = FROM - 1;
    int right = TO;

    while (right > left + 1) {
        int mid = (left + right) / 2;
        printf(_("Is your number greater than %d?\n"), mid);
        char ans[10];
        fgets(ans, 10, stdin);
        if (!strcmp(ans, _("yes\n"))) {
            left = mid;
        } else {
            right = mid;
        }
    }
    int answer = right;

    printf(_("Your number is %d!\n"), answer);
    return 0;
}
