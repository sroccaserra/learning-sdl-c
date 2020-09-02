CC = clang
LD = clang
SANITIZE= -fsanitize=address,undefined,nullability

CFLAGS = -std=c18 -Wall -Wextra -Wpedantic -Werror -Iinclude $(shell sdl2-config --cflags)
CDEBUGFLAGS = -O0 -DDEBUG -g -fno-omit-frame-pointer $(SANITIZE)

LDFLAGS = $(shell sdl2-config --libs)
LDDEBUGFLAGS = $(SANITIZE)

C_FILES = $(filter-out src/main.c, $(shell find src -type f -name '*.c'))
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
debug: CFLAGS += $(CDEBUGFLAGS)
debug: LDFLAGS += $(LDDEBUGFLAGS)
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
	rm -f scripts/sound
	rm -rf scripts/*.dSYM

scripts/sound: scripts/sound.c
	$(CC) $(CFLAGS) $(CDEBUGFLAGS) $(LDFLAGS) -o scripts/sound scripts/sound.c

.PHONY: sound
sound: scripts/sound
	scripts/sound
