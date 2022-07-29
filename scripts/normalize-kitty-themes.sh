#!/usr/bin/env bash
set -eou pipefail
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd ../.
DEBUG_MODE=${DEBUG_MODE:-0}
PALETTES_LIMIT=100000
PALETTE_THEME_DIRS="\
submodules/kitty-themes-kovidgoyal/themes\
 submodules/iTerm2-Color-Schemes/kitty\
 submodules/kitty-themes/themes\
 submodules/theme.sh\
"

ls_palettes() {
	find submodules/kitty-themes-kovidgoyal/themes submodules/iTerm2-Color-Schemes/kitty submodules/kitty-themes/themes -type f | sort -u | head -n $PALETTES_LIMIT
}

normalize_content() {
	tr -s ' ' | tr ' ' '=' | tr -d '#' | grep '='
}

main() {
	ls_palettes | while read -r file; do
		local name="$(echo $(basename "$file" .conf) | tr '[A-Z]' '[a-z]' | tr -d ' ' | tr -d '(' | tr -d ')' | tr -d '-' | tr -d '_' | tr -d '+' | tr -d '.')"
		[[ "$name" == "" ]] && continue
		local data="$(cat "$file" | normalize_content)"
		local palette_file="palettes/kitty-themes/$name"
		ansi --yellow $file
		ansi --blue $name
		ansi --magenta "$data"
		ansi --cyan "$palette_file"
		echo -e "$data" >"$palette_file"
		echo
	done
}

main
