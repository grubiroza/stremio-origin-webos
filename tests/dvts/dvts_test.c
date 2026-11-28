/* domar: dvts.c on the PC (zig cc as a plain C compiler). A TS made by ffmpeg (-c copy -f mpegts) goes through the
   filter as the converter's output would, cut into n pieces handed on with the carry (as the segments are), and is
   written out for ffmpeg / dovi_tool to read back.
     zig cc -std=c11 -Isrc tests/dvts/dvts_test.c src/core/dvts.c -o build/dvts_test.exe
     build/dvts_test.exe <in.ts> <out.ts> <sdr|sdr8|hdr10> [pieces] [bright]
   (domar 0060: sdr8 = an H.264 TS as profile 9; an out name holding %d: each piece its own file, as segments) */
#include "core/dvts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int put(void *ud, const unsigned char *p, int n) { return fwrite(p, 1, (size_t)n, (FILE *)ud) == (size_t)n ? n : -1; }

int main(int argc, char **argv) {
  if (argc < 4) { fprintf(stderr, "usage: dvts_test in.ts out.ts sdr|sdr8|hdr10 [pieces] [bright]\n"); return 2; }
  int each = strstr(argv[2], "%d") != NULL; char on[1024]; snprintf(on, sizeof on, argv[2], 0);
  FILE *in = fopen(argv[1], "rb"), *out = fopen(on, "wb"); if (!in || !out) { perror("open"); return 2; }
  fseek(in, 0, SEEK_END); long n = ftell(in); fseek(in, 0, SEEK_SET);
  unsigned char *b = malloc((size_t)n); if (!b || fread(b, 1, (size_t)n, in) != (size_t)n) { fprintf(stderr, "read\n"); return 2; }
  int pieces = argc > 4 ? atoi(argv[4]) : 1, bright = argc > 5 ? atoi(argv[5]) : 0, avc = !strcmp(argv[3], "sdr8"), sdr = avc || !strcmp(argv[3], "sdr");
  long pkts = n / 188; DvtsCarry carry = DVTS_CARRY0; long ra = 0, pa = 0, oa = 0;
  /* the pieces start where the converter's segments do: at a keyframe's first packet (the PID with the most packets is
     the video; its start with the random access indicator), the nearest after an even share */
  static long cnt[8192]; for (long i = 0; i < pkts; i++) cnt[((b[i * 188 + 1] & 0x1f) << 8) | b[i * 188 + 2]]++;
  int vp = 0; for (int p = 1; p < 8191; p++) if (cnt[p] > cnt[vp]) vp = p;
  long cut[65]; int nc = 0; cut[nc++] = 0;
  for (int k = 1; k < pieces && nc < 64; k++) { long i = pkts * k / pieces;
    for (; i < pkts; i++) { const unsigned char *t = b + i * 188; if ((((t[1] & 0x1f) << 8) | t[2]) == vp && (t[1] & 0x40) && (t[3] & 0x20) && t[4] > 0 && (t[5] & 0x40)) break; }
    if (i < pkts && i > cut[nc - 1]) cut[nc++] = i; }
  cut[nc] = pkts; pieces = nc;
  for (int k = 0; k < pieces; k++) {
    long a = cut[k] * 188, z = cut[k + 1] * 188;
    if (each && k) { fclose(out); snprintf(on, sizeof on, argv[2], k); if (!(out = fopen(on, "wb"))) { perror("open"); return 2; } }
    DvTs *f = sdr ? dvts_new_sdr(bright, avc, avc ? 4 : 5, &carry, put, out) : dvts_new(996, 1000, 6, &carry, put, out);
    long at = a; while (at < z) { int c = (int)(z - at > 7000 ? 7000 : z - at); if (dvts_write(f, b + at, c) != c) { fprintf(stderr, "write failed\n"); return 1; } at += c; } /* (odd sizes: packets cut between writes) */
    if (dvts_flush(f) < 0) { fprintf(stderr, "flush failed\n"); return 1; }
    long r, p, o; dvts_stats(f, &r, &p, &o); ra += r; pa += p; oa += o; dvts_carry(f, &carry); dvts_free(f);
  }
  fclose(out); printf("%ld RPUs added, %ld PMTs marked, %ld packets as they were, %d pieces\n", ra, pa, oa, pieces);
  return 0;
}
