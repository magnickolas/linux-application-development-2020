TARGET = Show
CFLAGS = -O0 -g -Wall -Werror -Wextra `pkg-config ncursesw --cflags`
LDFLAGS = `pkg-config --libs ncursesw`

all: $(TARGET)

$(TARGET): show.o block.o file_with_name.o
	$(CC) -o $@ $^ $(LDFLAGS)

show.o: show.c
	$(CC) $(CFLAGS) -c $<

block.o: block.c
	$(CC) $(CFLAGS) -c $<

file_with_name.o: file_with_name.c
	$(CC) $(CFLAGS) -c $<

.phony clean:
	rm -rf *.o $(TARGET)
