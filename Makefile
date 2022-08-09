default: all
##############################################################
include submodules/c_deps/etc/tools.mk
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
TIDIED_FILES = $(shell find */*.c */*.h -type f|egrep -v 'kfc-utils-colors.c|kfc-utils/kfc-utils-palettes-backup.c|kfc-utils/kfc-utils-palettes-backup1.c|kfc-utils/kfc-utils-palettes-rendered.c|kfc-utils/kfc-utils-palettes.backup.c|kfc-utils/kfc-utils-palettes.c')
_TIDIED_FILES = \
			   */*.h */*.c
##############################################################
include submodules/c_deps/etc/includes.mk
all: build muon test
test-embedded-palettes:
	@clear;kfc -e|xargs -I % env bash -c "clear && kfc-cli -S % && sleep 3;"

debug-palette-includes-c:
	@env DEBUG_MODE=1 $(CREATE_PALETTE_INCLUDES_SCRIPT)

view-palette-includes-c:
	@env $(CREATE_PALETTE_INCLUDES_SCRIPT)

write-palette-includes-c:
	@$(CREATE_PALETTE_INCLUDES_SCRIPT) > kfc-utils/kfc-utils-palettes.c

test: do-test
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
	@$(NODEMON) -i build \
		--delay 1 \
		-w submodules/c_deps/exec-fzf/exec-fzf.c \
		-w etc/kfc-utils-palettes.c.j2 \
		-w submodules/c_deps/exec-fzf/exec-fzf.h \
		-w meson.build \
		-w "*/*.c" -w "*/*.h" -w Makefle -w "*/meson.build" \
		-e j2,c,h,sh,Makefile,build \
			-x sh -- -c 'passh make||true'
do-pull-submodules-cmds:
	@command find submodules -type d -maxdepth 1|xargs -I % echo -e "sh -c 'cd % && git pull'"
