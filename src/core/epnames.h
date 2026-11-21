/* domar: an episode its metadata has no name for yet takes TMDB's.
   Cinemeta lists a new episode as "TBA" for days after it has aired (seen 2026-11-21: The Rings of Power S3:E5 and
   S3:E6, aired on the 18th, named on TMDB since): the player said "S3:E6 · TBA" and the title page called the episode
   upcoming. With the user's TMDB key, the season of such an episode (aired in the last 120 days, or due in the next 14)
   is asked once; the names are kept in the store ("epNames", the last 300) and put into every copy of the metadata as
   it enters the cache, so from then on they are there at once. TMDB's own stand-ins ("Episode 7") are not names.
   Nothing is asked without a key. */
#ifndef SB_EPNAMES_H
#define SB_EPNAMES_H
#include "cJSON.h"
void epnames_apply(const char *type, const char *id, cJSON *meta); /* a series' metadata on its way into the cache: the kept names in, the missing ones asked for */
int  epnames_patch(cJSON *meta);   /* the kept names into a copy of a series' metadata (a page's own); how many it put in */
unsigned epnames_gen(void);        /* bumps when names came in: a page puts them into its own copy */
#endif
