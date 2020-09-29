TARGET=show
CFLAGS = -O2 -Wall -Werror -Wextra

all: $(TARGET)

$(TARGET): show.o block.o file_with_name.o
	$(CC) $(CFLAGS) -o $@ $^ -lncursesw

show.o: show.c
	$(CC) $(CFLAGS) -c $<

block.o: block.c
	$(CC) $(CFLAGS) -c $<

file_with_name.o: file_with_name.c
	$(CC) $(CFLAGS) -c $<

.phony clean:
	rm -rf *.o $(TARGET)
