CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS = -lsdl2

C_FILES = $(wildcard src/*.c)
HEADERS_FILES = $(wildcard src/*.h)
OBJ_FILES = $(C_FILES:.c=.o)

EXEC_NAME = learning-sdl-c

all: CFLAGS += -O3
all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $@

run: $(EXEC_NAME)
	./$(EXEC_NAME)

debug: CFLAGS += -O0 -DDEBUG -g
debug: $(EXEC_NAME)
	@echo "To debug on macOs, use:"
	@echo "$$ lldb $(EXEC_NAME)"
	@echo "(lldb) b main"
	@echo "(lldb) run"

clean:
	rm -f $(OBJ_FILES)
	rm -f $(EXEC_NAME)
