/* tests/addons: an addon's address and the requests made from it. A "?query" after /manifest.json stays after the path
   of every request (streams, meta, subtitles, a catalog and its pages), as the official core sends it; an address
   without one comes out as it always did. tests/addons/run.sh */
#include "core/addons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* what addons.c needs from the rest of the app */
cJSON *sb_addons(void) { return NULL; }
int sb_addon_on(const cJSON *addon) { (void)addon; return 1; }

static int fails;
static void eq(const char *name, const char *got, const char *want) { if (strcmp(got, want)) { fails++; printf("FAIL %s:\n   got  %s\n   want %s\n", name, got, want); } }
static void base_of(const char *transport, char *out, size_t cap) {
  cJSON *ad = cJSON_CreateObject(); cJSON_AddStringToObject(ad, "transportUrl", transport); addon_base(ad, out, cap); cJSON_Delete(ad); }

int main(void) {
  char b[SB_BASE_MAX], u[SB_BASE_MAX + 800], p[SB_BASE_MAX + 440];
  base_of("https://h.example/x/manifest.json", b, sizeof b); eq("plain", b, "https://h.example/x");
  addon_url(u, sizeof u, b, "stream", "movie", "tt0111161", NULL); eq("plain stream", u, "https://h.example/x/stream/movie/tt0111161.json");
  addon_url(u, sizeof u, b, "subtitles", "series", "tt1:2:3", "videoSize=5&filename=a%20b"); eq("plain subtitles with extra", u, "https://h.example/x/subtitles/series/tt1%3A2%3A3/videoSize=5&filename=a%20b.json");

  base_of("https://h.example/x/manifest.json?token=abc&u=1", b, sizeof b); eq("query kept", b, "https://h.example/x?token=abc&u=1");
  addon_url(u, sizeof u, b, "stream", "movie", "tt0111161", NULL); eq("query after a stream's path", u, "https://h.example/x/stream/movie/tt0111161.json?token=abc&u=1");
  addon_url(u, sizeof u, b, "meta", "series", "tt2", NULL); eq("query after a meta's path", u, "https://h.example/x/meta/series/tt2.json?token=abc&u=1");
  addon_url(u, sizeof u, b, "subtitles", "movie", "tt3", "videoHash=ff"); eq("query after the extra", u, "https://h.example/x/subtitles/movie/tt3/videoHash=ff.json?token=abc&u=1");
  /* a catalog: its path first (kept in the row), then the extra and .json, then a later page */
  addon_join(p, sizeof p, b, "/catalog/movie/top"); eq("catalog path", p, "https://h.example/x/catalog/movie/top?token=abc&u=1");
  addon_join(u, sizeof u, p, ".json"); eq("catalog", u, "https://h.example/x/catalog/movie/top.json?token=abc&u=1");
  addon_join(u, sizeof u, p, "/genre=Action.json"); eq("catalog with a genre", u, "https://h.example/x/catalog/movie/top/genre=Action.json?token=abc&u=1");
  addon_join(u, sizeof u, p, "/genre=Action&skip=100.json"); eq("catalog page", u, "https://h.example/x/catalog/movie/top/genre=Action&skip=100.json?token=abc&u=1");

  base_of("https://h.example/manifest.json?k=v", b, sizeof b); eq("query at the root", b, "https://h.example?k=v");
  addon_url(u, sizeof u, b, "stream", "movie", "tt4", NULL); eq("query at the root, a stream", u, "https://h.example/stream/movie/tt4.json?k=v");
  base_of("https://h.example/x//manifest.json?k=v", b, sizeof b); eq("slashes before it go, the query stays", b, "https://h.example/x?k=v");
  base_of("https://h.example/x/manifest.json?", b, sizeof b); eq("an empty query is none", b, "https://h.example/x");
  base_of("https://h.example/x/manifest.json/more", b, sizeof b); eq("what follows that is no query goes, as before", b, "https://h.example/x");
  base_of("https://h.example/x/", b, sizeof b); eq("no manifest.json: the address as it is", b, "https://h.example/x");
  addon_join(u, sizeof u, "https://v3-cinemeta.strem.io/catalog/movie/top", "/genre=Drama.json"); eq("no query: plain joining", u, "https://v3-cinemeta.strem.io/catalog/movie/top/genre=Drama.json");

  /* a setup of 1500 characters in the path and a query after it: both whole */
  { char setup[1501], t[1700], want[1700]; memset(setup, 'A', 1500); setup[1500] = 0;
    snprintf(t, sizeof t, "https://h.example/%s/manifest.json?key=1234", setup); base_of(t, b, sizeof b);
    snprintf(want, sizeof want, "https://h.example/%s?key=1234", setup); eq("a long setup and a query", b, want);
    addon_url(u, sizeof u, b, "stream", "movie", "tt5", NULL); snprintf(want, sizeof want, "https://h.example/%s/stream/movie/tt5.json?key=1234", setup); eq("a long setup and a query, a stream", u, want); }
  printf(fails ? "%d failed\n" : "all ok\n", fails); return fails ? 1 : 0;
}
