TARGET=show

all: $(TARGET)

$(TARGET): $(TARGET).c
	gcc -O0 -g -o $@ $< -lncursesw

.phony clean:
	rm -rf $(TARGET)
