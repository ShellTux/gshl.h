#!/bin/sh
set -e

single_header=gshl.h
rm --force "$single_header"

usage() {
  printf 'Usage: %s [OPTION].. <DEPS>..\n' "$0"
  printf 'Generate single header library: %s\n' "$single_header"
  printf '\n'
  printf '  -h, --help      Help page\n'
  exit 1
}

TEMP=$(getopt \
	--options 'h' \
	--long 'help,dry-run' \
	--name "$(basename "$0")" -- "$@")

# shellcheck disable=SC2181
if [ $? -ne 0 ]
then
	echo 'Terminating...' >&2
	exit 1
fi

# Note the quotes around "$TEMP": they are essential!
eval set -- "$TEMP"
unset TEMP

while true
do
  case "$1" in
    -h | --help) usage ;;
    --dry-run)
      single_header=/dev/null
      shift;
      ;;
    --)
      shift
      break
      ;;
    *) # Option not dealt with
      echo 'Internal error!' >&2
      exit 1
      ;;
  esac
done

deps="$*"

[ -z "$deps" ] && usage

header_deps="$(echo "$deps" \
  | tr ' ' '\n' \
  | grep '.h$' \
  | awk '!seen[$0]++' \
  | tr '\n' ' ')"

src_deps="$(echo "$deps" \
  | tr ' ' '\n' \
  | grep '.c$' \
  | awk '!seen[$0]++' \
  | tr '\n' ' ')"

rm --force "$single_header" || true

echo '#ifndef INCLUDE_GSHL_H_' | tee --append "$single_header" >/dev/null
echo '#define INCLUDE_GSHL_H_' | tee --append "$single_header" >/dev/null
for header in $header_deps
do
  awk '/^\/\/ gshl-priority: [0-9]+$/ {found=1; print $3 " " FILENAME} END {if (!found) print 0 " " FILENAME}' "$header"
done | sort --reverse | while read -r priority header
do
  printf ' %i %s\r' "$priority" "$header"
  cat "$header" \
    | grep --invert-match '^#\s*include "' \
    | grep --invert-match '^#ifndef INCLUDE_' \
    | grep --invert-match '^#define INCLUDE_' \
    | grep --invert-match '^#endif // INCLUDE_' \
    | tee --append "$single_header" >/dev/null
  printf '\033[32m%s\033[0m %i %s\n' ✓ "$priority" "$header"
done
echo '#endif // INCLUDE_GSHL_H_' | tee --append "$single_header" >/dev/null

echo '#ifdef GSHL_IMPLEMENTATION' | tee --append "$single_header" >/dev/null
for src in $src_deps
do
  awk '/^\/\/ gshl-priority: [0-9]+$/ {found=1; print $3 " " FILENAME} END {if (!found) print 0 " " FILENAME}' "$src"
done | sort --reverse | while read -r priority src
do
  printf ' %i %s\r' "$priority" "$src"

  lines="$(awk 'END {print NR}' "$src")"
  [ "$lines" -le 0 ] && continue

  first_empty_line_number="$(awk '/^$/ {print NR ; exit}' "$src")"
  if ! { [ 1 -le "$first_empty_line_number" ] && [ "$first_empty_line_number" -le "$lines" ]; }
  then
    printf 'Invalid first empty line number for %s:%d\n' "$src" "$first_empty_line_number"
    exit 1
  fi

  {
    printf '#ifdef GSHL_SOURCE_CODE_MAPPING\n'
    printf '#line %d "%s"\n' "$first_empty_line_number" "$src"
    printf '#endif // GSHL_SOURCE_CODE_MAPPING\n'
  } | clang-format | tee --append "$single_header" >/dev/null
  cat "$src" \
    | grep --invert-match '^#\s*include "' \
    | tee --append "$single_header" >/dev/null

  printf '\033[32m%s\033[0m %i %s\n' ✓ "$priority" "$src"
done
echo '#endif // GSHL_IMPLEMENTATION' | tee --append "$single_header" >/dev/null
