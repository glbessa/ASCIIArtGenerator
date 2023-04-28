CC = gcc
CFLAGS = -Wall -I$(IDIR) -g
CLIBS = -lm -lpthread

IDIR = include
SRCDIR = src
SOURCES = $(SRCDIR)/*.c
BUILDDIR = build
EXE_NAME = main

RUN_ARGS =

all: compile

compile: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(BUILDDIR)/$(EXE_NAME) $(CLIBS)

run: ./$(BUILDDIR)/$(EXE_NAME)
	@./$(BUILDDIR)/$(EXE_NAME) $(RUN_ARGS)

run-valgrind: ./$(BUILDDIR)/$(EXE_NAME)
	@valgrind ./$(BUILDDIR)/$(EXE_NAME) $(RUN_ARGS)