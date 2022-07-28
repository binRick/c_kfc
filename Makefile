default: all
SED=$(shell command -v gsed||command -v sed)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
DIR=$(shell $(PWD))
ETC_DIR=$(DIR)/etc
TIDIED_FILES = \
			   kfc.c \
			   palette-includes.h
CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wno-newline-eof
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc
CREATE_PALETTE_INCLUDES_SCRIPT := create-palette-includes-c.sh

debug-palette-includes-c:
	@env DEBUG_MODE=1 ./$(CREATE_PALETTE_INCLUDES_SCRIPT)

view-palette-includes-c:
	@env ./$(CREATE_PALETTE_INCLUDES_SCRIPT)

write-palette-includes-c:
	@./$(CREATE_PALETTE_INCLUDES_SCRIPT) > palette-includes.c

uncrustify:
	@$(UNCRUSTIFY) -c $(ETC_DIR)/uncrustify.cfg --replace $(TIDIED_FILES) 

uncrustify-clean:
	@find . -type f -name "*unc-back*"|xargs -I % unlink %

tidy: uncrustify uncrustify-clean fix-dbg

all: kfc kfc-test

kfc: kfc.c Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< $(LDFLAGS)

kfc-test:
	@./kfc -r -p;sleep 1
	@./kfc -s base16-outrun -p;sleep 1
	@./kfc -s vscode -p

install: all
	install -Dm755 kfc $(DESTDIR)$(BINDIR)/kfc
	mkdir -p $(DESTDIR)/usr/local/share/kfc
	cp -r palettes $(DESTDIR)/usr/local/share/kfc

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/kfc
	rm -rf $(DESTDIR)/usr/local/share/kfc

clean:
	rm -f kfc *.o

.PHONY: all install uninstall clean



fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)
