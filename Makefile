CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

C_FILES = $(filter-out src/main.c, $(wildcard src/*.c))
OBJ_FILES = $(C_FILES:.c=.o)

EXEC_NAME = learning-sdl-c

.PHONY: all
all: CFLAGS += -O2 -ffast-math -DNDEBUG
all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES) src/main.o
	$(CC) $(LDFLAGS) $(OBJ_FILES) src/main.o -o $@

.PHONY: run
run: $(EXEC_NAME)
	./$(EXEC_NAME)

.PHONY: debug
debug: CFLAGS += -O0 -DDEBUG -g
debug: $(EXEC_NAME)
	@echo "To debug on macOs, use:"
	@echo "$$ lldb $(EXEC_NAME)"
	@echo "(lldb) b main"
	@echo "(lldb) run"

scripts/poly_sin_cos_perf: CFLAGS += -O2 -ffast-math
scripts/poly_sin_cos_perf: $(OBJ_FILES)

.PHONY: perf
perf: LDFLAGS += -Lsrc
perf: scripts/poly_sin_cos_perf
	scripts/poly_sin_cos_perf

.PHONY: clean
clean:
	rm -f $(OBJ_FILES) src/main.o
	rm -f $(EXEC_NAME)
	rm -f scripts/poly_sin_cos_perf
