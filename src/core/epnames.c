/* domar: TMDB's names for the episodes the metadata has none for yet (see epnames.h) */
#include "core/epnames.h"
#include "core/stremio.h"
#include "core/store.h"
#include "core/tmdb.h"
#include "core/net.h"
#include "core/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>

#define EN_KEEP 300          /* names kept in the store, the oldest out first */
#define EN_AGAIN (6 * 3600)  /* a season still short of names is asked again after this long */
void sb_meta_each(const char *type, const char *id, void (*fn)(cJSON *meta, void *ud), void *ud); /* stremio.c: every cached copy of a title's metadata */

static unsigned gen;
unsigned epnames_gen(void) { return gen; }
/* no name: none at all, or the stand-in the metadata carries until someone types the real one in */
static int no_name(const char *n) { while (*n == ' ') n++; if (!*n) return 1; return (!strncasecmp(n, "TBA", 3) || !strncasecmp(n, "TBD", 3)) && (n[3] == 0 || n[3] == ' '); }
/* TMDB's own stand-in ("Episode 7") */
static int stand_in(const char *s) { if (strncasecmp(s, "Episode", 7)) return 0; const char *p = s + 7; while (*p == ' ') p++; if (!isdigit((unsigned char)*p)) return 0; while (isdigit((unsigned char)*p)) p++; return !*p; }
/* the name in place of the stand-in. The old words are left where they are (a few bytes, once a name): a page drawing
   this very copy may still hold them */
static void set_name(cJSON *v, const char *name) {
  cJSON *nm = cJSON_GetObjectItemCaseSensitive(v, "name");
  if (cJSON_IsString(nm)) { char *c = strdup(name); if (c) nm->valuestring = c; }
  else if (!nm) cJSON_AddStringToObject(v, "name", name);
}
int epnames_patch(cJSON *m) {
  const cJSON *map = store_node("epNames"); cJSON *vs = cJSON_GetObjectItemCaseSensitive(m, "videos"), *v; int n = 0;
  if (!cJSON_IsObject(map) || !map->child || !cJSON_IsArray(vs)) return 0;
  cJSON_ArrayForEach(v, vs) { if (!no_name(js_str(v, "name", js_str(v, "title", "")))) continue;
    const char *k = js_str(map, js_str(v, "id", ""), NULL); if (k && *k) { set_name(v, k); n++; } }
  return n;
}
typedef struct { char id[SB_ID]; int season, eps[64], n; } Ask;
static struct { char key[SB_ID + 12]; time_t at; } asked[32]; static int nasked;
static int asked_lately(const char *id, int s) {
  char k[SB_ID + 12]; snprintf(k, sizeof k, "%s:%d", id, s); time_t now = time(NULL);
  for (int i = 0; i < nasked; i++) if (!strcmp(asked[i].key, k)) { if (now - asked[i].at < EN_AGAIN) return 1; asked[i].at = now; return 0; }
  int i = nasked < 32 ? nasked++ : (int)(now % 32); snprintf(asked[i].key, sizeof asked[i].key, "%s", k); asked[i].at = now; return 0;
}
static void patch_cached(cJSON *m, void *ud) { *(int *)ud += epnames_patch(m); }
static void on_season(int st, const unsigned char *b, size_t n, void *ud) {
  Ask *a = ud; (void)n; cJSON *j = st == 200 ? net_json(b) : NULL; const cJSON *e; int got = 0;
  cJSON *map = store_node("epNames"); if (!cJSON_IsObject(map)) { map = cJSON_CreateObject(); store_set_node("epNames", map); }
  cJSON_ArrayForEach(e, js_arr(j, "episodes")) { const char *nm = js_str(e, "name", ""); int en = (int)js_num(e, "episode_number", 0), want = 0;
    for (int i = 0; i < a->n; i++) if (a->eps[i] == en) want = 1;
    if (!want || no_name(nm) || stand_in(nm)) continue;
    char k[SB_VID]; snprintf(k, sizeof k, "%s:%d:%d", a->id, a->season, en);
    cJSON_DeleteItemFromObjectCaseSensitive(map, k); cJSON_AddStringToObject(map, k, nm); got++; }
  net_json_free(j);
  int put = 0;
  if (got) { while (cJSON_GetArraySize(map) > EN_KEEP) cJSON_DeleteItemFromArray(map, 0); store_set_node("epNames", map);
    sb_meta_each("series", a->id, patch_cached, &put); gen++; }
  fprintf(stderr, "[epnames] %s season %d: %d of %d nameless episode(s) named by TMDB (HTTP %d), %d put into the metadata at hand\n", a->id, a->season, got, a->n, st, put);
  free(a);
}
void epnames_apply(const char *type, const char *id, cJSON *m) {
  if (strcmp(type, "series") || !sb_is_imdb(id)) return;
  epnames_patch(m);
  cJSON *vs = cJSON_GetObjectItemCaseSensitive(m, "videos"), *v; if (!cJSON_IsArray(vs) || !tmdb_has_key()) return;
  int tid = (int)js_numstr(m, "moviedb_id", 0); if (tid <= 0) return;
  Ask *asks[4]; int na = 0; time_t now = time(NULL);
  cJSON_ArrayForEach(v, vs) { if (!no_name(js_str(v, "name", js_str(v, "title", "")))) continue;
    int s = (int)js_num(v, "season", -1), e = (int)js_num(v, "episode", -1); if (s < 1 || e < 1) continue;
    const char *rel = js_str(v, "released", js_str(v, "firstAired", "")); struct tm tm; memset(&tm, 0, sizeof tm);
    if (strlen(rel) < 10 || sscanf(rel, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) continue; tm.tm_year -= 1900; tm.tm_mon--;
    time_t t = timegm(&tm); if (t > now + 14 * 86400 || t < now - 120 * 86400) continue; /* (an old show's nameless episode never gets a name: not asked on every visit) */
    Ask *a = NULL; for (int i = 0; i < na; i++) if (asks[i]->season == s) a = asks[i];
    if (!a) { if (na >= 4 || !(a = calloc(1, sizeof *a))) continue; snprintf(a->id, sizeof a->id, "%s", id); a->season = s; asks[na++] = a; }
    if (a->n < 64) a->eps[a->n++] = e; }
  for (int i = 0; i < na; i++) { Ask *a = asks[i];
    if (asked_lately(id, a->season)) { free(a); continue; }
    char p[96]; snprintf(p, sizeof p, "/tv/%d/season/%d?language=en-US", tid, a->season);
    if (!tmdb_request(p, on_season, a)) free(a); }
}
