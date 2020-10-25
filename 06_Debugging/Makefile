CFLAGS = -Wall -O0 -g
GLIBC_PATH = /usr/src/glibc/glibc-2.30

all: main

.gdbinit:
	echo "set history save on" > $@
	find $(GLIBC_PATH) -maxdepth 1                           \
	                   ! -path $(GLIBC_PATH)                 \
	                   ! -path "$(GLIBC_PATH)/ChangeLog.old" \
	                   -type d                               \
	| xargs -I{} echo directory {} >> $@

main: main.c

.phony clean:
	rm -r main .gdbinit
