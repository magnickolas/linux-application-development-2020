GENERATES = prog prog-a prog-so prog*.txt liboutput.so README
TRASH = *.a *.o *~ o.*

all: README prog prog-a prog-so

# Execute each make binary dependency with passed arguments
# $(1): suffix for output files
# $(2): string of arguments separated by space
define test_with_args
	echo "===Test with $(1) arguments==="
	printf "$^" | tr ' ' '\0' | \
	xargs -0 -I{} \
	      sh -c '[ -z "$(2)" ] \
	                 && ./{} >{}$(1).txt 2>&1 \
	                 || echo $(2) | xargs ./{} >{}$(1).txt 2>&1'
	  diff $<$(1).txt $(word 2,$^)$(1).txt && \
	  diff $<$(1).txt $(word 3,$^)$(1).txt \
	      && printf "\e[1;32mPASSED\e[0m\n" \
	      || printf "\e[1;31mFAILED\e[0m\n"
endef

test: prog prog-a prog-so
	@$(call test_with_args,0)
	@$(call test_with_args,1,1337)
	@$(call test_with_args,3,1337 228 42)

prog: const.o fun.o prog.o

prog-a: prog.o liboutput_static.a
	$(LINK.o) -L. $< $(LOADLIBES) $(LDLIBS) -loutput_static -o $@

prog-so: prog.o liboutput.so
	$(LINK.o) -L. -Wl,-rpath . $< $(LOADLIBES) $(LDLIBS) -loutput -o $@

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
