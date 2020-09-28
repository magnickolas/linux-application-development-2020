TARGET=view

all: $(TARGET)

$(TARGET): view.c
	gcc -O0 -g -o $@ $< -lncursesw

.phony clean:
	rm -rf $(TARGET)
