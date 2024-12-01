CC = clang
WARNINGS = -Wall -Wextra -Wmissing-declarations -Wmissing-variable-declarations -Wno-unused-variable
CFLAGS_COMMON = $(WARNINGS) -std=gnu99 -D_GNU_SOURCE -lm
CFLAGS_RELEASE_NO_LINK = $(WARNINGS) -std=c99 -D_GNU_SOURCE -O3
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O3
CFLAGS_DEBUG = $(CFLAGS_COMMON) $(CFLAGS_MINE) -O0 -g -DDEBUG

SRC_COMMON = $(wildcard ../libs/*.c)
INC = -I../includes

.PHONY: all debug
all: part1 part2

debug: part1-debug part2-debug

.PHONY: example real
example:
	rm -f data.in && ln -s example.in data.in

real:
	rm -f data.in && ln -s real.in data.in

part1: $(SRC_COMMON) part1.c
	$(CC) $^ $(CFLAGS_RELEASE) $(INC) -o $@

part1-debug: $(SRC) part1.c
	$(CC) $^ $(CFLAGS_DEBUG) $(INC) -o $@

part2: $(SRC_COMMON) part2.c
	$(CC) $^ $(CFLAGS_RELEASE) $(INC) -o $@

part2-debug: $(SRC) part2.c
	$(CC) $^ $(CFLAGS_DEBUG) $(INC) -o $@

.PHONY: clean
clean:
	rm -f part1 part2 part1-debug part2-debug data.in
