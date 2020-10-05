TARGET = Show
CFLAGS = -O2 -Wall -Werror -Wextra -Wno-implicit-function-declaration\
         `pkg-config ncursesw --cflags`
LDLIBS = `pkg-config --libs ncursesw`

all: $(TARGET)

$(TARGET): show.o block.o file_with_name.o
	$(CC) -o $@ $^ $(LDLIBS)

.phony clean:
	rm -rf *.o $(TARGET)
