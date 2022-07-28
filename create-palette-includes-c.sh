#!/usr/bin/env bash
set -eou pipefail
PALETTES_LIMIT=1000
DEBUG_MODE=${DEBUG_MODE:-0}
PALETTES=
INCBIN_PREFIX=inc_palette_
PALETTE_DIRS="palettes/dark palettes/kitty-themes palettes/iTerm2-Color-Schemes"

PALETTE_INCLUDES_C_PREFIX="#pragma once\n#include \"palette-includes.h\"\n"
get_file_bytes() {
	local cmd="stat -f%z $1"
	if ! eval $cmd 2>/dev/null; then
		cmd="stat --printf=\"%s\" $1"
	fi
	eval "$cmd"
}

ls_palettes() {
	if [[ "$PALETTES" == "" ]]; then
		PALETTES="$(find $PALETTE_DIRS -type f | sort -u | head -n $PALETTES_LIMIT)"
	fi
	echo -e "$PALETTES"
}

get_palettes_qty() {
	ls_palettes | wc -l | tr -d ' '
}

palette_names_list() {
	palette_names | while read -r n; do
		local item="$(palette_name_char_list_item "$n")"
		echo -e "  $item"
	done
}

palette_file_palette_t_list_item() {
	local file="$1"
	local name="$(palette_file_to_inc_name $file)"
	local size="$(get_file_bytes $file)"
	local data="xxx"
	local inc_name="$(palette_file_to_inc_name $file)"
	echo -ne "{ .name = \"$(basename $file)\", .size = $size, .file = \"$file\", .data = ${INCBIN_PREFIX}${inc_name}_data },"
}

palette_name_char_list_item() {
	echo -ne "{ ${INCBIN_PREFIX}${1} },"
}

palette_file_to_inc_name() {
	echo -e "$(basename $(echo "$1" | tr '-' '_'))"
}

get_palettes_list_line() {
	local qty="$(get_palettes_qty)"
	local line="const char *PALETTES_LIST[] = {\n\
$(palette_names_list)\
\n  NULL,\n\
};"
	echo -e "$line"
}

get_palettes_qty_line() {
	local qty="$(get_palettes_qty)"
	local line="const size_t PALETTES_QTY = $qty;"
	echo -e "$line"
}

get_palette_inc_line() {
	local palette_file="$1"
	local palette_inc_name="$(palette_file_to_inc_name "$1")"
	local line="INCTXT($palette_inc_name, \"$palette_file\");"

	echo -e "$line"
}

palette_names() {
	ls_palettes | while read -r p; do
		local name=$(palette_file_to_inc_name $p)
		echo -e "$name"
	done
}

names_f=$(mktemp)
main() {
	local qty=$(get_palettes_qty)
	if [[ "$DEBUG_MODE" -eq 0 ]]; then
		echo -e "$PALETTE_INCLUDES_C_PREFIX"
		get_palettes_qty_line
	fi
	ls_palettes | while read -r file; do
		local inc=$(palette_file_to_inc_name $file)
        if grep -q "^${inc}$" $names_f; then continue; fi
        echo -e "$inc" >> $names_f
		local inc_line=$(get_palette_inc_line $file)
		local char_item="$(palette_name_char_list_item "$inc")"
		local struct_item="$(palette_file_palette_t_list_item "$file")"
		if [[ "$DEBUG_MODE" -eq 1 ]]; then
			{
				ansi --yellow "<$file>"
				ansi --blue " - $inc"
				ansi --magenta "$inc_line"
				ansi --yellow "$char_item"
				ansi --red "$struct_item"
				echo
			} >&2
		else
			echo -e "$inc_line"
		fi
	done

	echo -e "struct inc_palette_t palette_t_list[] = {"
	ls_palettes | while read -r file; do
		echo -ne "  "
		palette_file_palette_t_list_item $file
		echo
	done
	echo -e "  { 0 },"
	echo -e "};"
	#if [[ "$DEBUG_MODE" -eq 0 ]]; then
	#  get_palettes_list_line
	#fi
}

main
