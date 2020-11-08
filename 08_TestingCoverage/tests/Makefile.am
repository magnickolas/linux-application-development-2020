TESTS = grow_trunc pop push clear_null
check_PROGRAMS = grow_trunc pop push clear_null

CFLAGS = -I$(top_builddir)/src @CHECK_CFLAGS@
LDADD = $(top_builddir)/src/libbuf.la @CHECK_LIBS@

.ts.c:
	checkmk $< > $@
