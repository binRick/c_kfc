SED=$(shell command -v gsed||command -v sed)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
DIR=$(shell $(PWD))
ETC_DIR=$(DIR)/etc
TIDIED_FILES = \
			   kfc.c
CFLAGS += -std=c99 -Wall -Wextra -pedantic
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

uncrustify:
	@$(UNCRUSTIFY) -c $(ETC_DIR)/uncrustify.cfg --replace $(TIDIED_FILES) 

uncrustify-clean:
	@find . -type f -name "*unc-back*"|xargs -I % unlink %

tidy: uncrustify uncrustify-clean fix-dbg

all: kfc

kfc: kfc.c Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< -lX11 $(LDFLAGS)

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
