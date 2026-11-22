/* tests/cached: where a debrid file is, as the addons write it beside a stream (best_cached): in the service's cloud,
   not there yet, or not said. tests/cached/run.sh */
#include "core/best.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* what the scoring needs from the rest of the app */
const char *T(const char *en) { return en; }
const char *T1(const char *en, const char *k, const char *v) { (void)k; (void)v; return en; }
const TvCaps *best_caps(void) { return NULL; }
double best_net_mbps(void) { return 0; }

static int fails;
static void t(const char *what, const char *name, const char *desc, int p2p, int flag, int want) {
  Stream s; memset(&s, 0, sizeof s); snprintf(s.name, sizeof s.name, "%s", name); snprintf(s.title, sizeof s.title, "%s", desc);
  snprintf(s.url, sizeof s.url, "%s", p2p ? "p2p://0123456789abcdef0123456789abcdef01234567/0?" : "https://files.example/f.mkv"); s.p2p = p2p; s.cached = flag;
  int got = best_cached(&s); if (got != want) { fails++; printf("FAIL %s: got %d, want %d\n", what, got, want); }
  BestFacts f; best_facts(&s, NULL, 50, 110, &f); if (f.cached != want) { fails++; printf("FAIL %s: the row's facts say %d, want %d\n", what, f.cached, want); }
  if ((want < 0) != (strstr(f.warn, "Not cached yet") != NULL)) { fails++; printf("FAIL %s: the row's warning is \"%s\"\n", what, f.warn); }
  BestRate r; int rated = best_rate(&s, NULL, 50, 110, &r); if (want < 0 && rated) { fails++; printf("FAIL %s: a file not in the cloud is still a candidate for the best fit\n", what); }
}
int main(void) {
  t("[RD+]", "[RD+] Torrentio\n4k DV | HDR", "Film.2024.2160p.WEB-DL.mkv\n\xf0\x9f\x91\xa4 12 \xf0\x9f\x92\xbe 18.2 GB", 0, 0, 1);
  t("[RD download]", "[RD download] Torrentio\n1080p", "Film.2024.1080p.WEB-DL.mkv", 0, 0, -1);
  t("[PM+]", "[PM+] Addon 1080p", "", 0, 0, 1);
  t("[TB+]", "[TB+] Addon 1080p", "", 0, 0, 1);
  t("a bolt in the brackets", "[RD\xe2\x9a\xa1] Comet 2160p", "Film.2024.2160p.mkv", 0, 0, 1);
  t("a down arrow in the brackets", "[RD\xe2\xac\x87] Comet 1080p", "Film.2024.1080p.mkv", 0, 0, -1);
  t("a down arrow with its variation mark", "[RD\xe2\xac\x87\xef\xb8\x8f] Comet 1080p", "Film.2024.1080p.mkv", 0, 0, -1);
  t("a bolt alone", "\xe2\x9a\xa1 RD 4K", "Film.2024.2160p.mkv", 0, 0, 1);
  t("an hourglass", "\xe2\x8f\xb3 RD 1080p", "Film.2024.1080p.mkv", 0, 0, -1); /* (1080p: above the test's panel another warning comes first) */
  t("the other hourglass", "\xe2\x8c\x9b RD 1080p", "Film.2024.1080p.mkv", 0, 0, -1);
  t("the word cached", "Addon 1080p", "Cached \xc2\xb7 4.1 GB", 0, 0, 1);
  t("the word UNCACHED", "Addon 1080p", "UNCACHED \xc2\xb7 4.1 GB", 0, 0, -1);
  t("not cached", "Addon 1080p", "Not cached \xc2\xb7 4.1 GB", 0, 0, -1);
  t("needs to download", "Addon 1080p", "Needs to download first", 0, 0, -1);
  t("nothing said", "Addon 1080p", "Film.2024.1080p.WEB-DL.mkv \xc2\xb7 4.1 GB", 0, 0, 0);
  t("HDR10+ in brackets is not a service", "Addon 2160p [HDR10+]", "Film.2024.2160p.mkv", 0, 0, 0);
  t("one letter in brackets is not a service", "Addon [A+] 1080p", "", 0, 0, 0);
  t("a torrent says nothing", "[RD+] Addon 1080p", "\xe2\x9a\xa1 cached", 1, 0, 0);
  t("the addon's own flag: cached", "[RD download] Addon 1080p", "", 0, 1, 1);
  t("the addon's own flag: not cached", "[RD+] Addon 1080p", "", 0, -1, -1);
  printf(fails ? "%d failed\n" : "all ok\n", fails); return fails ? 1 : 0;
}
