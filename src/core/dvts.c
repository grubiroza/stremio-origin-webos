/* domar (0056): an HDR10 film shown as Dolby Vision, the TS side (dvts.h). Proven first by hand on this TV (LG C9,
   2026-11-27): an HDR10 clip with dovi_tool's static 8.1 RPU in each frame, through this app's converter, showed as
   DolbyVision (dynamicRange dolbyHdr). domar (0057): an SDR picture the same way, its RPU mapping SDR into PQ
   (dvsdr_rpu.h) and the base layer marked SDR (compatibility 2); proven by hand on the same TV 2026-11-28. */
#include "core/dvts.h"
#include "core/dvstatic_rpu.h"
#include "core/dvsdr_rpu.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TSP 188
#define PEND_MAX (8 << 20)   /* a frame bigger than this goes out without its RPU (counted) */
#define OBUF (64 * 1024)
#define RPU_PK 3             /* TS packets one RPU may take (an SDR one, ~380 bytes, takes 3; an HDR10 one 1) */

struct DvTs {
  dvts_out out; void *ud;
  const uint8_t *rk, *rn; int rkN, rnN; /* the RPU NAL units: a keyframe's (scene refresh), the frames' after it */
  uint8_t desc[13]; int descLen;
  uint8_t part[TSP]; int partN;         /* a packet cut between two writes */
  int pmtPid, vidPid;                   /* -1 until the PAT / PMT say */
  uint8_t *pend; int pendN, pendCap;    /* the frame waiting for its end: its packets and those between them */
  int open, lastVid, pesLen, key;       /* a frame waits; its last video packet; its PES_packet_length (-1 none); a keyframe */
  /* the video's continuity counter: each added packet shifts the converter's own numbers by one (base); a segment that
     continues the one before (the converter's numbers go on) keeps the shift, one after a seek starts again */
  int base, lastOrig, lastOut, seen;
  uint8_t *ob; int obN;                 /* output, sent in blocks */
  long rpus, pmts, odd; int fail;
};

static uint32_t crc32_mpeg(const uint8_t *p, int n) {
  uint32_t c = 0xFFFFFFFFu; while (n--) { c ^= (uint32_t)*p++ << 24; for (int i = 0; i < 8; i++) c = c & 0x80000000u ? c << 1 ^ 0x04C11DB7u : c << 1; } return c;
}
static const int CLL[] = { 400, 600, 1000, 1500, 2000, 4000 };
void dvts_pick(int maxCll, int mastMax, int *cll, int *mast) {
  int m = mastMax >= 2000 ? 4000 : 1000, c = maxCll > 0 ? maxCll : (mastMax > 0 ? mastMax : 1000), k = 5;
  for (int i = 0; i < 6; i++) if (c <= CLL[i]) { k = i; break; }
  *cll = CLL[k]; *mast = m;
}
int dvts_level(int w, int h, double fps) { /* the smallest level whose pixel rate takes it (Dolby's table, levels 1-10) */
  static const double R[] = { 22118400, 27648000, 49766400, 62208000, 124416000, 199065600, 248832000, 398131200, 497664000, 995328000 };
  double r = (double)(w > 0 ? w : 3840) * (h > 0 ? h : 2160) * (fps > 1 ? fps : 24);
  for (int i = 0; i < 10; i++) if (r <= R[i] * 1.001) return i + 1;
  return 10;
}
/* the same descriptors the converter writes for a real profile 8 picture: profile 8, the level, RPU and base layer
   present, no enhancement layer, the base layer's compatibility (1 HDR10, 2 SDR) */
static DvTs *dvts_make(const uint8_t *rk, int rkN, const uint8_t *rn, int rnN, int compat, int level, const DvtsCarry *carry, dvts_out out, void *ud) {
  DvTs *f = calloc(1, sizeof *f); if (!f) return NULL;
  f->ob = malloc(OBUF); if (!f->ob) { free(f); return NULL; }
  f->out = out; f->ud = ud; f->lastVid = f->pesLen = -1; f->lastOut = -1;
  f->base = carry ? carry->base & 15 : 0; f->lastOrig = carry ? carry->lastOrig : -1; f->vidPid = carry ? carry->vidPid : -1; f->pmtPid = carry ? carry->pmtPid : -1;
  f->rk = rk; f->rkN = rkN; f->rn = rn; f->rnN = rnN;
  unsigned v = 8u << 9 | (unsigned)(level & 0x3f) << 3 | 1u << 2 | 0u << 1 | 1u; int k = 0;
  f->desc[k++] = 0x05; f->desc[k++] = 4; f->desc[k++] = 'D'; f->desc[k++] = 'O'; f->desc[k++] = 'V'; f->desc[k++] = 'I';
  f->desc[k++] = 0xB0; f->desc[k++] = 5; f->desc[k++] = 1; f->desc[k++] = 0; f->desc[k++] = (uint8_t)(v >> 8); f->desc[k++] = (uint8_t)v; f->desc[k++] = (uint8_t)((compat & 15) << 4);
  f->descLen = k;
  return f;
}
DvTs *dvts_new(int maxCll, int mastMax, int level, const DvtsCarry *carry, dvts_out out, void *ud) {
  int cll, mast; dvts_pick(maxCll, mastMax, &cll, &mast); const DvsRpu *r = NULL;
  for (int i = 0; i < DVS_RPU_N; i++) if (DVS_RPU[i].mast == mast && DVS_RPU[i].cll == cll) r = &DVS_RPU[i];
  if (!r) r = &DVS_RPU[2];
  return dvts_make(r->key, r->keyN, r->next, r->nextN, 1, level, carry, out, ud);
}
int dvts_sdr_preset(int bright, int *white, int *peak) { /* domar (0057): 0 white at 203 nits, 1 the highlights spread above it */
  int i = bright > 0 && DVS_SDR_N > 1 ? 1 : 0; if (white) *white = DVS_SDR[i].white; if (peak) *peak = DVS_SDR[i].peak; return i;
}
DvTs *dvts_new_sdr(int bright, int level, const DvtsCarry *carry, dvts_out out, void *ud) {
  const DvsSdrRpu *r = &DVS_SDR[dvts_sdr_preset(bright, NULL, NULL)];
  return dvts_make(r->key, r->keyN, r->next, r->nextN, 2, level, carry, out, ud);
}
void dvts_free(DvTs *f) { if (!f) return; free(f->pend); free(f->ob); free(f); }
void dvts_stats(const DvTs *f, long *rpus, long *pmts, long *odd) { *rpus = f->rpus; *pmts = f->pmts; *odd = f->odd; }
void dvts_carry(const DvTs *f, DvtsCarry *c) { c->base = f->base; c->lastOrig = f->lastOrig; c->vidPid = f->vidPid; c->pmtPid = f->pmtPid; }

/* one of the converter's video packets on its way out: its continuity counter shifted by the packets added so far */
static void vid_cc(DvTs *f, uint8_t *t) {
  int orig = t[3] & 15, pay = (t[3] & 0x10) != 0;
  if (!f->seen && pay) { f->seen = 1; if (f->lastOrig < 0 || orig != ((f->lastOrig + 1) & 15)) f->base = 0; } /* (not the next one: a new run, the converter counts from its start) */
  int out = (orig + f->base) & 15; t[3] = (uint8_t)((t[3] & 0xf0) | out);
  if (pay) { f->lastOrig = orig; f->lastOut = out; }
}

/* ---- output ---- */
static int oflush(DvTs *f) {
  if (!f->obN || f->fail) { f->obN = 0; return f->fail ? -1 : 0; }
  int r = f->out(f->ud, f->ob, f->obN); f->obN = 0; if (r < 0) f->fail = 1; return r < 0 ? -1 : 0;
}
static int oput(DvTs *f, const uint8_t *p, int n) {
  if (f->fail) return -1;
  if (f->obN + n > OBUF) { if (oflush(f) < 0) return -1; if (n > OBUF) { int r = f->out(f->ud, p, n); if (r < 0) f->fail = 1; return r < 0 ? -1 : 0; } }
  memcpy(f->ob + f->obN, p, (size_t)n); f->obN += n; return 0;
}

/* ---- the tables ---- */
static int payload_at(const uint8_t *t) { /* where the payload starts, -1 none */
  if (!(t[3] & 0x10)) return -1; int o = 4; if (t[3] & 0x20) o += 1 + t[4]; return o < TSP ? o : -1;
}
static void pat_read(DvTs *f, const uint8_t *t) {
  int o = payload_at(t); if (o < 0) return; o += 1 + t[o]; if (o + 8 > TSP || t[o] != 0x00) return;
  int sl = ((t[o + 1] & 0x0f) << 8) | t[o + 2], end = o + 3 + sl - 4; if (end > TSP) end = TSP;
  for (int i = o + 8; i + 4 <= end; i += 4) { int prog = t[i] << 8 | t[i + 1]; if (prog) { f->pmtPid = ((t[i + 2] & 0x1f) << 8) | t[i + 3]; return; } }
}
/* a PMT that starts in this packet: the HEVC entry's PID taken, and the descriptors added to it (its length, the
   section's and the CRC again); one carrying them already, or with no room, is left as it is */
static void pmt_fix(DvTs *f, uint8_t *t) {
  int o = payload_at(t); if (o < 0) return; o += 1 + t[o]; if (o + 12 > TSP) return;
  uint8_t *sec = t + o; if (sec[0] != 0x02) return;
  int sl = ((sec[1] & 0x0f) << 8) | sec[2], end = 3 + sl; if (sl < 13 || o + end > TSP) return;
  int pil = ((sec[10] & 0x0f) << 8) | sec[11], at = 12 + pil;
  while (at + 5 <= end - 4) {
    int type = sec[at], pid = ((sec[at + 1] & 0x1f) << 8) | sec[at + 2], eil = ((sec[at + 3] & 0x0f) << 8) | sec[at + 4], de = at + 5 + eil;
    if (de > end - 4) return;
    if (type == 0x24) {
      f->vidPid = pid;
      for (int i = at + 5; i + 1 < de; i += 2 + sec[i + 1]) if (sec[i] == 0xB0) return; /* (marked already) */
      if (o + end + f->descLen > TSP) { f->odd++; return; }
      memmove(sec + de + f->descLen, sec + de, (size_t)(end - de)); memcpy(sec + de, f->desc, (size_t)f->descLen);
      eil += f->descLen; sec[at + 3] = (uint8_t)(0xf0 | eil >> 8); sec[at + 4] = (uint8_t)eil;
      sl += f->descLen; end += f->descLen; sec[1] = (uint8_t)((sec[1] & 0xf0) | sl >> 8); sec[2] = (uint8_t)sl;
      uint32_t c = crc32_mpeg(sec, end - 4); sec[end - 4] = (uint8_t)(c >> 24); sec[end - 3] = (uint8_t)(c >> 16); sec[end - 2] = (uint8_t)(c >> 8); sec[end - 1] = (uint8_t)c;
      f->pmts++;
      return;
    }
    at = de;
  }
}

/* ---- the frames ---- */
static int rpu_npk(const DvTs *f) { int nl = f->key ? f->rkN : f->rnN; return (3 + nl + 183) / 184; }
/* the RPU as TS packets of the frame's PES (no new start): 00 00 01 + its NAL unit, the room left over stuffed in the
   first packet's adaptation field (0057: an SDR RPU takes 3 packets; until then one always did) */
static void rpu_packets(DvTs *f, uint8_t *t) {
  const uint8_t *nal = f->key ? f->rk : f->rn; int nl = f->key ? f->rkN : f->rnN, np = rpu_npk(f), af = np * 184 - (3 + nl), done = -3;
  for (int k = 0; k < np; k++, t += TSP) {
    t[0] = 0x47; t[1] = (uint8_t)((f->vidPid >> 8) & 0x1f); t[2] = (uint8_t)f->vidPid; int a = k == 0 ? af : 0;
    if (a > 0) { t[3] = 0x30; t[4] = (uint8_t)(a - 1); if (a > 1) { t[5] = 0x00; memset(t + 6, 0xff, (size_t)(a - 2)); } } else t[3] = 0x10;
    for (int i = 4 + a; i < TSP; i++, done++) t[i] = done < 0 ? (done == -1 ? 1 : 0) : nal[done]; }
}
static int pend_add(DvTs *f, const uint8_t *t) { /* (room kept for the RPU's packets) */
  int spare = (1 + RPU_PK) * TSP;
  if (f->pendN + spare > f->pendCap) { int nc = f->pendCap ? f->pendCap * 2 : 256 * 1024; if (nc > PEND_MAX + spare) nc = PEND_MAX + spare;
    if (f->pendN + spare > nc) return -1; uint8_t *np = realloc(f->pend, (size_t)nc); if (!np) return -1; f->pend = np; f->pendCap = nc; }
  memcpy(f->pend + f->pendN, t, TSP); f->pendN += TSP; return 0;
}
static int flush_frame(DvTs *f) { /* the waiting frame out, its RPU right after its last video packet */
  if (!f->pendN) { f->open = 0; return 0; }
  int ins = f->open && f->lastVid >= 0 && f->vidPid >= 0 && rpu_npk(f) <= RPU_PK, n = f->pendN, after = -1, end = -1;
  if (ins) {
    int add = rpu_npk(f) * TSP;
    after = f->lastVid + TSP; end = after + add; memmove(f->pend + end, f->pend + after, (size_t)(f->pendN - after)); rpu_packets(f, f->pend + after); n += add;
    if (f->pesLen >= 0) { int l = f->pend[f->pesLen] << 8 | f->pend[f->pesLen + 1];
      if (l) { l += 3 + (f->key ? f->rkN : f->rnN); if (l > 0xFFFF) l = 0; f->pend[f->pesLen] = (uint8_t)(l >> 8); f->pend[f->pesLen + 1] = (uint8_t)l; } }
    f->rpus++;
  }
  for (int off = 0; off < n; off += TSP) { uint8_t *t = f->pend + off;
    if (off >= after && off < end) { f->lastOut = (f->lastOut + 1) & 15; t[3] = (uint8_t)((t[3] & 0xf0) | f->lastOut); f->base = (f->base + 1) & 15; continue; } /* (an added packet: the next number, and every later one one further) */
    if ((((t[1] & 0x1f) << 8) | t[2]) == f->vidPid) vid_cc(f, t); }
  f->pendN = 0; f->open = 0; f->lastVid = f->pesLen = -1; f->key = 0;
  return oput(f, f->pend, n);
}
static int packet(DvTs *f, uint8_t *t) {
  if (t[0] != 0x47) { f->odd++; return f->open && pend_add(f, t) == 0 ? 0 : oput(f, t, TSP); }
  int pid = ((t[1] & 0x1f) << 8) | t[2], pusi = t[1] & 0x40;
  if (pid == 0 && pusi) pat_read(f, t);
  else if (pusi && f->pmtPid >= 0 && pid == f->pmtPid) pmt_fix(f, t);
  if (f->vidPid >= 0 && pid == f->vidPid) {
    if (pusi) {
      if (flush_frame(f) < 0) return -1;
      f->open = 1; f->key = (t[3] & 0x20) && t[4] > 0 && (t[5] & 0x40); f->pesLen = -1;
      int o = payload_at(t); if (o >= 0 && o + 6 <= TSP && t[o] == 0 && t[o + 1] == 0 && t[o + 2] == 1) f->pesLen = f->pendN + o + 4;
    }
    if (!f->open) { f->odd++; vid_cc(f, t); return oput(f, t, TSP); } /* (a frame's tail with no start seen: as it is) */
    if (pend_add(f, t) < 0) { f->odd++; f->open = 0; if (flush_frame(f) < 0) return -1; vid_cc(f, t); return oput(f, t, TSP); } /* (too big: out without its RPU) */
    if (t[3] & 0x10) f->lastVid = f->pendN - TSP;
    return 0;
  }
  if (f->open) { if (pend_add(f, t) == 0) return 0; f->odd++; f->open = 0; if (flush_frame(f) < 0) return -1; }
  return oput(f, t, TSP);
}
int dvts_write(void *ud, const unsigned char *p, int n) {
  DvTs *f = ud; int k = 0; if (f->fail) return -1;
  if (f->partN) { int c = TSP - f->partN; if (c > n) c = n; memcpy(f->part + f->partN, p, (size_t)c); f->partN += c; k = c;
    if (f->partN < TSP) return n; f->partN = 0; if (packet(f, f->part) < 0) return -1; }
  while (n - k >= TSP) { uint8_t t[TSP]; memcpy(t, p + k, TSP); if (packet(f, t) < 0) return -1; k += TSP; }
  if (n - k > 0) { memcpy(f->part, p + k, (size_t)(n - k)); f->partN = n - k; }
  return oflush(f) < 0 ? -1 : n;
}
int dvts_flush(DvTs *f) {
  if (f->partN) { f->odd++; if (oput(f, f->part, f->partN) < 0) return -1; f->partN = 0; }
  if (flush_frame(f) < 0) return -1;
  return oflush(f);
}
