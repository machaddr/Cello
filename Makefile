CC ?= gcc
AR ?= ar

VERSION = 2.1.0
PACKAGE = libCello-$(VERSION)

BINDIR = ${PREFIX}/bin
INCDIR = ${PREFIX}/include
LIBDIR = ${PREFIX}/lib

SRC := $(wildcard src/*.c)
# Exclude the std modules from main SRC to avoid duplication
MAIN_SRC := $(filter-out src/Math.c src/Collections.c src/Io.c src/Strings.c src/Time.c src/Os.c, $(SRC))
STD_SRC := src/Math.c src/Collections.c src/Io.c src/Strings.c src/Time.c src/Os.c
OBJ := $(addprefix obj/,$(notdir $(MAIN_SRC:.c=.o)))
STD_OBJ := $(addprefix obj/std/,$(notdir $(STD_SRC:.c=.o)))

TESTS := $(wildcard tests/*.c)
TESTS_OBJ := $(addprefix obj/,$(notdir $(TESTS:.c=.o)))

EXAMPLES := $(wildcard examples/*.c)
EXAMPLES_OBJ := $(addprefix obj/,$(notdir $(EXAMPLES:.c=.o)))
EXAMPLES_EXE := $(EXAMPLES:.c=)

CFLAGS = -I ./include -I ./src -std=gnu99 -O3 -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fstack-clash-protection -ftrivial-auto-var-init=zero -fPIE -fno-delete-null-pointer-checks -fno-strict-aliasing

PLATFORM := $(shell uname)
COMPILER := $(shell $(CC) -v 2>&1 )

DYNAMIC = libCello.so
STATIC = libCello.a
LIBS = -lpthread -lm

ifeq ($(findstring CYGWIN,$(PLATFORM)),CYGWIN)
	PREFIX ?= /usr/local
	
	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm
	
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/* ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -rf ${INCDIR}/Cello
else ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	# MSYS2
	ifeq ($(findstring MINGW32,$(MSYSTEM)),MINGW32)
		CC = i686-w64-mingw32-gcc
		PREFIX ?= /mingw32
	else ifeq ($(findstring MINGW64,$(MSYSTEM)),MINGW64)
		CC = x86_64-w64-mingw32-gcc
		PREFIX ?= /mingw64
	else ifeq ($(findstring MSYS,$(MSYSTEM)),MSYS)
		CC = gcc
		PREFIX ?= /usr
	else
		# MinGW64 mingw-builds
		PREFIX ?= c:/mingw64/x86_64-w64-mingw32
	endif

	DYNAMIC = libCello.dll
	STATIC = libCello.a
	LIBS = -lpthread -lm

	
	INSTALL_LIB = cp $(STATIC) $(LIBDIR)/$(STATIC); cp $(DYNAMIC) $(BINDIR)/$(DYNAMIC)
	INSTALL_INC = cp -r include/* $(INCDIR)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC); rm -f ${BINDIR}/$(DYNAMIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
else ifeq ($(findstring FreeBSD,$(PLATFORM)),FreeBSD)
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm

	CFLAGS += -fPIC
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC) && cp -f ${DYNAMIC} ${LIBDIR}/$(DYNAMIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/* ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
else
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm

	CFLAGS += -fPIC
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC) && cp -f ${DYNAMIC} ${LIBDIR}/$(DYNAMIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/* ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC) && rm -f ${LIBDIR}/$(DYNAMIC)
	UNINSTALL_INC = rm -rf ${INCDIR}/Cello
endif


# Libraries

all: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ) $(STD_OBJ)
	$(CC) $(OBJ) $(STD_OBJ) -shared $(LFLAGS) $(LIBS) -o $@

$(STATIC): $(OBJ) $(STD_OBJ)
	$(AR) rcs $@ $(OBJ) $(STD_OBJ)

obj/%.o: src/%.c include/Cello.h | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/std/Math.o: src/Math.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@

obj/std/Collections.o: src/Collections.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@

obj/std/Io.o: src/Io.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@

obj/std/Strings.o: src/Strings.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@

obj/std/Time.o: src/Time.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@

obj/std/Os.o: src/Os.c | obj/std
	$(CC) $< -c $(CFLAGS) -I./src -o $@


obj:
	mkdir -p obj

obj/std:
	mkdir -p obj/std

# Tests

check: CFLAGS += -Werror -g -ggdb
check: $(TESTS_OBJ) $(STATIC) $(EXAMPLES_EXE)
	@echo "Running core library tests..."
	$(CC) $(TESTS_OBJ) $(STATIC) $(LIBS) $(LFLAGS) -o ./tests/test
	./tests/test
	rm -f ./tests/test.bin ./tests/test.txt
	@echo ""
	@echo "Running example programs..."
	@for example in $(EXAMPLES_EXE); do \
		echo "Running $$example..."; \
		LD_LIBRARY_PATH=. ./$$example || echo "$$example failed"; \
		echo ""; \
	done
	@echo "All tests and examples completed!"

check-clean:
	@echo "Cleaning example binaries..."
	rm -f $(EXAMPLES_EXE)

obj/%.o: tests/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

# Benchmarks
ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
bench: CFLAGS += -DCELLO_NDEBUG -O3
bench: clean $(STATIC)
	cd benchmarks; ./benchmark.sh; cd ../
else
bench: CFLAGS += -DCELLO_NDEBUG -pg -O3
bench: clean $(STATIC)
	cd benchmarks; ./benchmark; cd ../
endif

# Examples

examples: $(EXAMPLES_EXE)

examples/%: CFLAGS += -Werror
examples/%: examples/%.c $(STATIC) | obj
	$(CC) $< $(STATIC) $(CFLAGS) $(LIBS) $(LFLAGS) -o $@

# Dist

dist: all | $(PACKAGE)
	cp -R examples include src tests LICENSE.md Makefile README.md $(PACKAGE)
	tar -czf $(PACKAGE).tar.gz $(PACKAGE) --exclude='*.exe' --exclude='*.pdb'

$(PACKAGE):
	mkdir -p $(PACKAGE)

# Clean

clean:
	rm -f $(OBJ) $(STD_OBJ) $(TESTS_OBJ) $(EXAMPLES_OBJ) $(STATIC) $(DYNAMIC) tests/test

# Install

install: all
	$(INSTALL_LIB)
	$(INSTALL_INC)
	
# Uninstall

uninstall:
	$(UNINSTALL_LIB)
	$(UNINSTALL_INC)

# Help

help:
	@echo "Cello Build System"
	@echo ""
	@echo "Main targets:"
	@echo "  all              - Build Cello library (static and dynamic)"
	@echo "  $(STATIC)        - Build static library"
	@echo "  $(DYNAMIC)       - Build dynamic library"
	@echo "  check            - Run tests and build/test all examples"
	@echo "  check-clean      - Clean example binaries"
	@echo "  examples         - Build examples"
	@echo "  bench            - Run benchmarks"
	@echo "  install          - Install library and headers"
	@echo "  uninstall        - Uninstall library and headers"
	@echo "  clean            - Clean build files"
	@echo "  dist             - Create distribution package"
	@echo ""
	@echo "Standard Library modules (integrated):"
	@echo "  All std modules included in main library automatically"
	@echo ""
	@echo "Standard Library modules available:"
	@echo "  math             - Mathematical functions and types"
	@echo "  collections      - Additional data structures (Stack, Queue, etc.)"
	@echo "  strings          - String manipulation utilities"
	@echo "  time             - Date/time operations"
	@echo "  io               - Enhanced I/O operations"
	@echo "  os               - Operating system interface"


