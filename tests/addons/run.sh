#!/bin/bash
# tests/addons/run.sh : an addon's address and its requests, with and without a "?query" (Mac; elsewhere any C compiler)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"; mkdir -p "$ROOT/build"
${CC:-cc} -std=c11 -Wall -Wextra -O1 -g ${SB_ASAN:+-fsanitize=address} -I"$ROOT/src" -I"$ROOT/third_party/cjson" \
  "$ROOT/tests/addons/addons_test.c" "$ROOT/src/core/addons.c" "$ROOT/third_party/cjson/cJSON.c" -o "$ROOT/build/addons_test"
"$ROOT/build/addons_test"
