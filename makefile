SRCDIR = src
BINDIR = bin

APP = file-to-header
DEPS = $(SRCDIR)/main.c makefile

.PHONY: build install

build: $(DEPS)
	@mkdir -p $(BINDIR)
	gcc $(SRCDIR)/main.c -o $(BINDIR)/$(APP)

install:
	cp $(BINDIR)/$(APP) /usr/bin/$(APP)
