#!/bin/bash
# tests/cached/run.sh : a debrid file in its service's cloud or not, as the addons write it (Mac; elsewhere any C compiler)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"; mkdir -p "$ROOT/build"
${CC:-cc} -std=c11 -D_GNU_SOURCE -DSB_BEST_PURE -Wall -Wextra -Wno-unused-parameter -O1 -g ${SB_ASAN:+-fsanitize=address} -I"$ROOT/src" -I"$ROOT/third_party/cjson" \
  "$ROOT/tests/cached/cached_test.c" "$ROOT/src/core/best.c" "$ROOT/third_party/cjson/cJSON.c" -lm -o "$ROOT/build/cached_test"
"$ROOT/build/cached_test"
