#include <stdlib.h>

int main() {
    int* ptr = malloc(sizeof(*ptr));

    // Double free pointer to debug it
    free(ptr);
    free(ptr);
}
