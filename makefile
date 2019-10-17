SRCDIR = src
BINDIR = bin

APP = file_to_header
DEPS = $(SRCDIR)/main.c makefile

.PHONY: build

build: $(DEPS)
	@mkdir -p $(BINDIR)
	gcc $(SRCDIR)/main.c -o $(BINDIR)/$(APP)
