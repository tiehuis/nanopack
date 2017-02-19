CC      ?= clang
CFLAGS  += -Wall -Wextra -std=c89 -pedantic -Os -Isrc

# We rely on C99 for testing solely for long-long literal suffixes
TCFLAGS += -Wall -Wextra -std=c99 -pedantic -Os -Isrc

# Code coverage report generation
ifeq ($(@enable-gcov), 1)
TCFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -lgcov
endif

all:
	@echo "   test - run all tests"
	@echo "  clean - clean all build files"

test: test_runner
	@./test_runner

test_runner: build/test.o build/libnp.a
	$(CC) $(CFLAGS) $(TCFLAGS) $^ -o $@ $(LDFLAGS)

build/test.o: test/test.c | build
	$(CC) $(CFLAGS) $(TCFLAGS) -c $< -o $@

build/nanopack.o: src/nanopack.c src/nanopack.h | build
	$(CC) $(CFLAGS) $(TCFLAGS) -c $< -o $@

build/libnp.a: build/nanopack.o | build
	ar rcs $@ $^

build:
	mkdir build

clean:
	rm -rf build test_runner *.profraw *.gcov

.PHONY:	test
