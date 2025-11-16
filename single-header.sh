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
	--long 'help' \
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

header_deps="$(echo "$deps" | tr ' ' '\n' | grep '.h$' | tr '\n' ' ')"
src_deps="$(echo "$deps" | tr ' ' '\n' | grep '.c$' | tr '\n' ' ')"

for header in $header_deps
do
  grep --invert-match '^#include "' "$header" >> "$single_header"
  printf '\033[32m%s\033[0m %s\n' ✓ "$header"
done

echo "#ifdef GSHL_IMPLEMENTATION" >> "$single_header"
for src in $src_deps
do
  lines="$(awk 'END {print NR}' "$src")"
  first_empty_line_number="$(awk '/^$/ {print NR ; exit}' "$src")"
  if ! { [ 1 -le "$first_empty_line_number" ] && [ "$first_empty_line_number" -le "$lines" ]; }
  then
    printf 'Invalid first empty line number for %s:%d\n' "$src" "$first_empty_line_number"
    exit 1
  fi
  {
    printf '#ifdef GSHL_SOURCE_CODE_MAPPING\n'
    printf '#line %d "%s"\n' "$first_empty_line_number" "$src"
    printf '#endif\n'
  } >> "$single_header"
  grep --invert-match '^#include "' "$src" >> "$single_header"
  printf '\033[32m%s\033[0m %s\n' ✓ "$src"
done
echo "#endif" >> gshl.h
