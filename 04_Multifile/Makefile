GENERATES = prog prog-a prog-so prog*.txt liboutput.so README
TRASH = *.a *.o *~ o.*

all: README prog prog-a prog-so

prog: const.o fun.o prog.o

prog-a: prog.o liboutput_static.a
	$(LINK.o) -L. $< $(LOADLIBES) $(LDLIBS) -loutput_static -o $@

prog-so: prog.o liboutput.so
	$(LINK.o) -L. -Wl,-rpath=. $< $(LOADLIBES) $(LDLIBS) -loutput -o $@

prog.o: outlib.h

fun.o: fun.c outlib.h
	$(COMPILE.c) -fPIC $< -o $@

liboutput_static.a: const.o fun.o
	$(AR) $(ARFLAGS) $@ $^

liboutput.so: const.o fun.o
	$(LINK.c) -shared $^ -o $@

README: prog
	./$< 2> $@

clean:
	rm -f $(TRASH)

distclean: clean
	rm -rf $(GENERATES)
