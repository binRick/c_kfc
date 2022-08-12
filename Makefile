default: all
##############################################################
include submodules/c_deps/etc/includes.mk
##############################################################
TIDIED_FILES = $(shell find */*.c */*.h -type f|egrep -v 'kfc-utils-colors.c|kfc-utils/kfc-utils-palettes-backup.c|kfc-utils/kfc-utils-palettes-backup1.c|kfc-utils/kfc-utils-palettes-rendered.c|kfc-utils/kfc-utils-palettes.backup.c|kfc-utils/kfc-utils-palettes.c')
_TIDIED_FILES = \
			   */*.h */*.c
##############################################################
test-embedded-palettes:
	@clear;kfc -e|xargs -I % env bash -c "clear && kfc-cli -S % && sleep 3;"

debug-palette-includes-c:
	@env DEBUG_MODE=1 $(CREATE_PALETTE_INCLUDES_SCRIPT)

view-palette-includes-c:
	@env $(CREATE_PALETTE_INCLUDES_SCRIPT)

write-palette-includes-c:
	@$(CREATE_PALETTE_INCLUDES_SCRIPT) > kfc-utils/kfc-utils-palettes.c

palettes-hash:
	@(grep '^INCTXT(' kfc-utils/kfc-utils-palettes.c |cut -d'"' -f2|xargs md5sum; md5sum kfc*/*.c kfc*/*.h meson.build */meson.build|md5sum)|md5sum|cut -d' ' -f1
