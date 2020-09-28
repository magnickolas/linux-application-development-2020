TARGET=show
CFLAGS = -O2 -Wall -Werror -Wextra

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $< -lncursesw

.phony clean:
	rm -rf $(TARGET)
