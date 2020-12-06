#include "buf.h"
#include <stdio.h>
#include <stdlib.h>

#test clear_null
    /* Clearing an NULL pointer is a no-op */
    float* a = NULL;
    buf_clear(a);
    ck_assert_msg(buf_size(a) == 0, "clear empty");
    ck_assert_msg(a == 0, "clear no-op");
