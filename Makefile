default: all
##############################################################
PASSH=$(shell command -v passh)
GIT=$(shell command -v git)
SED=$(shell command -v gsed||command -v sed)
NODEMON=$(shell command -v nodemon)
FZF=$(shell command -v fzf)
BLINE=$(shell command -v bline)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
FIND=$(shell command -v find)
EMBED_BINARY=$(shell command -v embed)
JQ_BIN=$(shell command -v jq)
##############################################################
DIR=$(shell $(PWD))
M1_DIR=$(DIR)
LOADER_DIR=$(DIR)/loader
EMBEDS_DIR=$(DIR)/embeds
VENDOR_DIR=$(DIR)/vendor
PROJECT_DIR=$(DIR)
MESON_DEPS_DIR=$(DIR)/meson/deps
VENDOR_DIR=$(DIR)/vendor
DEPS_DIR=$(DIR)/deps
BUILD_DIR=$(DIR)/build
ETC_DIR=$(DIR)/etc
MENU_DIR=$(DIR)/menu
DOCKER_DIR=$(DIR)/docker
LIST_DIR=$(DIR)/list
SOURCE_VENV_CMD=$(DIR)/scripts
VENV_DIR=$(DIR)/.venv
SCRIPTS_DIR=$(DIR)/scripts
ACTIVE_APP_DIR=$(DIR)/active-window
SOURCE_VENV_CMD = source $(VENV_DIR)/bin/activate
CREATE_PALETTE_INCLUDES_SCRIPT := scripts/create-palette-includes-c.sh
##############################################################
TIDIED_FILES = \
			   */*.h */*.c
##############################################################
all: build muon test

do-muon: do-muon-setup do-muon-build

do-muon-setup:
	@muon setup build-muon

do-muon-clean:
	@rm -rf build-muon

do-muon-build:
	@muon samu -C build-muon

do-muon-install:
	@cd build-muon && muon install

do-muon-test:
	@cd build-muon && muon test

build-muon: do-muon-setup do-muon-build do-muon-test
muon: do-muon-setup do-muon-build

test-embedded-palettes:
	@clear;kfc -e|xargs -I % env bash -c "clear && kfc-cli -S % && sleep 3;"

debug-palette-includes-c:
	@env DEBUG_MODE=1 $(CREATE_PALETTE_INCLUDES_SCRIPT)

view-palette-includes-c:
	@env $(CREATE_PALETTE_INCLUDES_SCRIPT)

write-palette-includes-c:
	@$(CREATE_PALETTE_INCLUDES_SCRIPT) > kfc-utils/kfc-utils-palettes.c

clean: do-muon-clean
	@rm -rf build .cache
muon: do-muon
do-meson: 
	@eval cd . && {  meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; }
do-install: all
	@meson install -C build
do-build:
	@meson compile -C build
do-test:
	@passh meson test -C build -v --print-errorlogs
install: do-install
test: do-test
build: do-meson do-build muon
uncrustify:
	@$(UNCRUSTIFY) -c submodules/meson_deps/etc/uncrustify.cfg --replace $(TIDIED_FILES) 
uncrustify-clean:
	@find  . -type f -name "*unc-back*"|xargs -I % unlink %
fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % llu);|, %llu);|g' -i $(TIDIED_FILES)
rm-make-logs:
	@rm .make-log* 2>/dev/null||true
tidy: rm-make-logs uncrustify uncrustify-clean fix-dbg
	@shfmt -w scripts/*.sh

dev-all: all

pull:
	@git pull

palettes-hash:
	@(grep '^INCTXT(' kfc-utils/kfc-utils-palettes.c |cut -d'"' -f2|xargs md5sum; md5sum kfc*/*.c kfc*/*.h meson.build */meson.build|md5sum)|md5sum|cut -d' ' -f1
dev: nodemon
nodemon:
	@$(PASSH) -L .nodemon.log $(NODEMON) -i build \
		--delay 1 \
		-w submodules/meson_deps/exec-fzf/exec-fzf.c \
		-w submodules/meson_deps/exec-fzf/exec-fzf.h \
		-w meson.build \
		-w "*/*.c" -w "*/*.h" -w Makefle -w "*/meson.build" \
		-e c,h,sh,Makefile,build \
			-x sh -- -c 'passh make||true'
meson-introspect-all:
	@meson introspect --all -i meson.build
meson-introspect-targets:
	@meson introspect --targets -i meson.build
meson-binaries:
	@meson introspect --targets  meson.build -i | jq 'map(select(.type == "executable").filename)|flatten|join("\n")' -Mrc|xargs -I % echo ./build/%
meson-binaries-loc:
	@make meson-binaries|xargs -I % echo %.c|sort -u|xargs Loc --files|bline -a bold:green -r yellow -R 1-6

do-pull-submodules-cmds:
	@command find submodules -type d -maxdepth 1|xargs -I % echo -e "sh -c 'cd % && git pull'"
run-binary:
	@make meson-binaries | fzf --reverse | xargs -I % nodemon -w build --delay 1000 -x passh "./%"
meson-tests-list:
	@meson test -C build --list
meson-tests:
	@make meson-tests-list|fzf --reverse -m | xargs -I % env cmd="\
		meson test --num-processes 1 -C build -v --no-stdsplit --print-errorlogs \"%\"" \
			env bash -c '\
	eval "$$cmd" && \
	ansi -n --green --bold "OK" && \
	echo -n "> " && \
	ansi -n --yellow --italic "$$cmd" && \
	echo \
'	
