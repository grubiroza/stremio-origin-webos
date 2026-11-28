# What this fork is

A fork of [stremio-origin/webos](https://github.com/stremio-origin/webos) at **1.1.1**, with the changes I run every
day on one TV: an LG OLED65C9PLA (webOS 4.10.2, the consumer "webOS 4.5"), Magic Remote, rooted, Homebrew Channel.

Each change is **one commit** on the branch `fixes-1.1.1`, with the reason in its message, and each fix is reported
upstream. The fork exists so that a report can point at the exact change. It is not a second app: there are no
releases and no support here. The app, its releases and its author are upstream.

When upstream takes a change, it leaves this list as the fork moves to the next version. Commits already published on
this branch are not rewritten, so links to them keep working.

## Fixes (reported upstream)

| Commit | What | Report |
|---|---|---|
| [c9db9c8](https://github.com/grubiroza/stremio-origin-webos/commit/c9db9c85e3032f11b21b3ba1aa896954cf76228a) | webOS 4.x: the Magic Remote pointer never came back after the app hid it (the call is left out on every webOS: on 5 and later, where it works, the app no longer hides the TV's cursor itself) | [#89](https://github.com/stremio-origin/webos/issues/89) |
| [60d3abe](https://github.com/grubiroza/stremio-origin-webos/commit/60d3abe65313f6b85d9c3e56af842b9b1be5876b) | webOS 4.x: the TV's screen-saver request has no `state` and was dropped | [#90](https://github.com/stremio-origin/webos/issues/90) |
| [a1db198](https://github.com/grubiroza/stremio-origin-webos/commit/a1db198f589e212b30553f46d1e4b64ae0e303df) | the screen-saver request is registered from the start, not from the first film | [#90](https://github.com/stremio-origin/webos/issues/90) |
| [1a83c9b](https://github.com/grubiroza/stremio-origin-webos/commit/1a83c9b92167cf5904cf65b641e5aedb0bef97ae) | a paused film is told to the TV (ACB), so the TV's screen saver covers the whole screen and not only a frame around the picture; over a paused film and the menus the TV's own comes at its own time (about two minutes without a key), after the app's own when that is set to one minute in Settings | [#90](https://github.com/stremio-origin/webos/issues/90) |
| [8d356b5](https://github.com/grubiroza/stremio-origin-webos/commit/8d356b5ea9bf04f57cb7ce5343295d4594c4b383) | the pointer during playback as in 1.0.0; a click on a focused button's name no longer pauses (the left group's last button: fe8f436, below) | [#95](https://github.com/stremio-origin/webos/issues/95) |
| [638e5cb](https://github.com/grubiroza/stremio-origin-webos/commit/638e5cbe59d14edb95c75a2cb29fb3a447e199e5) | player menus: what the pointer is over is shown, and the list no longer moves under it | [#95](https://github.com/stremio-origin/webos/issues/95) |
| [eb3e830](https://github.com/grubiroza/stremio-origin-webos/commit/eb3e8307b33ce74674d968abbe83d9ced94a3648) | player: the subtitle style cards take the pointer (a value, Done, beside the card) | [#95](https://github.com/stremio-origin/webos/issues/95) |
| [feef8c6](https://github.com/grubiroza/stremio-origin-webos/commit/feef8c695fd0e585a57694909e373d8715c8aa31) | seek previews with a server that answers several ranges with one plain part (Real-Debrid): no previews at all before | [#81](https://github.com/stremio-origin/webos/issues/81) |
| [0e1c537](https://github.com/grubiroza/stremio-origin-webos/commit/0e1c537147a23bf18fccd6d3256ec77d542484c3) | an addon's address is no longer cut at 511 characters (Comet: "obsolete configuration") | [#30](https://github.com/stremio-origin/webos/issues/30) |
| [b21f173](https://github.com/grubiroza/stremio-origin-webos/commit/b21f17371a8ce4b370ed4812f7740326edfa3482) | the same for catalogs and for a subtitle's own address | [#30](https://github.com/stremio-origin/webos/issues/30) |
| [9a717d2](https://github.com/grubiroza/stremio-origin-webos/commit/9a717d2c9bfbe8a53f8f9f469ee982821ebb236a) | a remembered link that now plays something else (a debrid notice clip) no longer marks the episode watched and loses its place (on Trakt too since 09d41b1, below) | [#102](https://github.com/stremio-origin/webos/issues/102) |
| [0733ea9](https://github.com/grubiroza/stremio-origin-webos/commit/0733ea972004ac12706003b045a42fb5a3010266) | ... and the same file is asked for again by itself and played from the kept place | [#102](https://github.com/stremio-origin/webos/issues/102) |
| [2f844e2](https://github.com/grubiroza/stremio-origin-webos/commit/2f844e22e27cb62b4f1ac1120cf07089009c5535) | player: Sources is never wider than the screen (with several addons its list could not be seen) | [#103](https://github.com/stremio-origin/webos/issues/103) |
| [e085a97](https://github.com/grubiroza/stremio-origin-webos/commit/e085a973fd5ae966e7919d3baf2b1a6e70697cb3) | player: Sources has no tab for an addon that answered with nothing | [#103](https://github.com/stremio-origin/webos/issues/103) |
| [1845297](https://github.com/grubiroza/stremio-origin-webos/commit/18452973cc94448dfd08e0fbb5c75dadb1c26ee4) | the pointer never scrolls a page, and no longer takes the highlight just before an arrow key | [#104](https://github.com/stremio-origin/webos/issues/104) |
| [90f2fa2](https://github.com/grubiroza/stremio-origin-webos/commit/90f2fa2b6ef32f692f126773188895b916e51254) | streams sheet: the pointer held on a row opens its details, as the OK key held does | [#104](https://github.com/stremio-origin/webos/issues/104) |
| [4e88341](https://github.com/grubiroza/stremio-origin-webos/commit/4e883413f76f05c32440503615faac394df24051) | an addon's `?query` after /manifest.json stays on every request, as the official core sends it (it was dropped) | [#107](https://github.com/stremio-origin/webos/issues/107) |
| [4daf860](https://github.com/grubiroza/stremio-origin-webos/commit/4daf86081d0ede35941fd6da2f3d0a2aa3184f2f) | a correction to b21f173: an addon's subtitle with an address of 1024 characters or more was still left out of the list | [#30](https://github.com/stremio-origin/webos/issues/30) |
| [bc5b785](https://github.com/grubiroza/stremio-origin-webos/commit/bc5b78531c3e5a0adc79de6788126bf1a80f0246) | webOS 3 / 4: an HDR10 film's own mastering values and colour description are passed on to the TV (they went blank); a switch in Settings > Playback (shown on any TV with HDR; it does something on webOS 3 / 4 only) | another user's [#106](https://github.com/stremio-origin/webos/issues/106), fixed upstream for the next update |
| [446e2d8](https://github.com/grubiroza/stremio-origin-webos/commit/446e2d87cb66d0ac70e0b6f59b488c666973ee00) | Dolby Vision profile 5 in a Matroska file (web copies with no HDR10 base: washed out and green as the TV plays them) goes through the local stream and shows as Dolby Vision; it has a lesson of its own, so a TV that once showed HDR10 for a profile 7 or 8 picture still converts it | [#109](https://github.com/stremio-origin/webos/issues/109) (related: [#2](https://github.com/stremio-origin/webos/issues/2)) |
| [3d8e89a](https://github.com/grubiroza/stremio-origin-webos/commit/3d8e89ac6dd1c765f59a447d55915f543a0ba0f6) | the converted stream reads its file in requests of up to 8 MiB (2 MiB before): a 20 Mbit/s film was made at about its own speed and the TV stopped to buffer | [#110](https://github.com/stremio-origin/webos/issues/110) |

### Upstream's answers so far (2026-11-22)

- [#102](https://github.com/stremio-origin/webos/issues/102): fixed for the next update. A file far shorter than the one
  the place was kept in is no longer resumed, saved or marked watched. The automatic search for the same file (0733ea9)
  is not included.
- [#103](https://github.com/stremio-origin/webos/issues/103): the width is fixed for the next update, and the desktop
  app's Local Files addon is no longer asked on the TV. Other addons with no streams keep their tab.
- [#104](https://github.com/stremio-origin/webos/issues/104): not taken, as it changes how the remote feels everywhere.
  Both commits stay here for those who use a Magic Remote the way I do.
- [#30](https://github.com/stremio-origin/webos/issues/30): fixed for the next update. Addon addresses get room for
  4,000 characters everywhere they are used, and a subtitle's own address 2,000.

## Features and preferences (not reported yet; here as they are)

| Commit | What |
|---|---|
| [a6516f3](https://github.com/grubiroza/stremio-origin-webos/commit/a6516f32a0105201a23e97cbaee9fae38ac52a88) | player: a "Subtitle delay" button in the row of buttons, straight to the delay card |
| [a786233](https://github.com/grubiroza/stremio-origin-webos/commit/a786233cae5647e470f00ae645ca52023825f020) | a subtitle picked by hand is kept: for a film too, always, and the very file |
| [83ff21c](https://github.com/grubiroza/stremio-origin-webos/commit/83ff21c8bc9877695b01fe3225a7cc339ac937ce) | a second subtitle language (Settings > Subtitles), taken when a film has none in the first |
| [3da1c7e](https://github.com/grubiroza/stremio-origin-webos/commit/3da1c7e55a2b6f6f3e0a1cd3f69b21ff28fb6814) | subtitle size in steps of 5 % (70 to 180) |
| [fc963e3](https://github.com/grubiroza/stremio-origin-webos/commit/fc963e331249f3af9a910c7f8ad4add457188b56) | the subtitles go while a player menu is open (they showed through a tall one); the Subtitles menu keeps them in view beside it since a2d99d6 |
| [3857c32](https://github.com/grubiroza/stremio-origin-webos/commit/3857c32323b2ec5e4118ead955e5053db5a8a844) | streams sheet: a film's or an episode's stream in two lines, as the player's Sources lists it; held OK shows everything the addon wrote |
| [ff3bf77](https://github.com/grubiroza/stremio-origin-webos/commit/ff3bf774f44f5cae4a0ac616d39710aed47f6526) | an episode the metadata still calls "TBA" takes its name from TMDB (with the user's key) |
| [e908360](https://github.com/grubiroza/stremio-origin-webos/commit/e908360722746f653c3c90ab6ffc9693661314e3) | the log kept on the TV is 4 MB, not 400 KB |
| [1aad379](https://github.com/grubiroza/stremio-origin-webos/commit/1aad379be4d09d3796ee615834b8bdc23a8d2519) | a preferred subtitle addon (Settings > Subtitles): after the file's own track, its subtitles come before the built-in list's and the other addons' |
| [8ba4ed9](https://github.com/grubiroza/stremio-origin-webos/commit/8ba4ed97c3f7ef83ff3e86948426a898a294f4b7) | a debrid file in its service's cloud, or not there yet, is marked in the stream lists (a bolt, an hourglass) |

The last two, and the `?query` fix above, came from reading
[betterstremio/origin](https://github.com/betterstremio/origin), a fork of Stremio Origin 1.0.0 that went its own way
(a new Home, other platforms). It has had them since its 2.1.8 and 3.0.7. They are written here for this code; the
marks read beside a debrid file are the ones it reads.

## Corrections after a review (2026-11-24)

The changes above were read again by two independent reviewers. Each finding was checked on the code first, and on
the TV where it could be shown; these commits correct the ones that held. Findings that did not hold, or that are
upstream's own design, are not listed. The commit each one corrects is in the last column.

| Commit | What | Corrects |
|---|---|---|
| [bd46130](https://github.com/grubiroza/stremio-origin-webos/commit/bd461307003acd29efa6c3c5f71e4100b2efa8ee) | streams sheet: the press that opened a row's details no longer puts them away when it is let go beside a panel narrower than the row; a row pressed before the highlight reached it opens its own details | 90f2fa2 |
| [09d41b1](https://github.com/grubiroza/stremio-origin-webos/commit/09d41b113c4c0fd93defd9392b9614373f8c50ab) | a remembered link that plays something else: Trakt is told nothing of it (the notice clip played to its end was scrobbled as watched); found so again, the link is forgotten; the file's name stays out of the log | 9a717d2, 0733ea9 |
| [8a599fd](https://github.com/grubiroza/stremio-origin-webos/commit/8a599fd9fef931c1450442a3816299812fe74595) | subtitles: a file kept from the built-in list waits for that list, not only for the addons' (an addon's file was put on first; upstream's own wait for a show, which a786233 uses for films too); the second language waits for one retry of a failing list, not all three | a786233, 83ff21c |
| [fe8f436](https://github.com/grubiroza/stremio-origin-webos/commit/fe8f4364d01ed47043ebd10ace1b52ac268cf167) | player: a click on the name of the left group's last button ("Play from the beginning") takes that button; while the buttons slide, a pill stops at the next icon (from betterstremio/origin 4d11eae) | 8d356b5 |
| [a8da46c](https://github.com/grubiroza/stremio-origin-webos/commit/a8da46c89a2f76085a302358eb626266c2cfbb00) | player: Back from the delay card its own button opened puts it away (it opened Subtitles); a full row of buttons takes no more | a6516f3 |
| [0c93d2d](https://github.com/grubiroza/stremio-origin-webos/commit/0c93d2d2f66d48efd5ef3ba847837a1626bcb5fd) | player menus: a key that moves the focus ends the pointer's hold on the list | 638e5cb |
| [8211006](https://github.com/grubiroza/stremio-origin-webos/commit/8211006037fa63bc01182bc88acb025b4b017a99) | player: the subtitle size card slides with the pointer too (it stayed among the seven sizes shown first) | 3da1c7e, eb3e830 |
| [ecefb51](https://github.com/grubiroza/stremio-origin-webos/commit/ecefb51f73842cc8feb7c9d5035ccc548fb26af9) | Dolby Vision profile 5: one file shown otherwise no longer turns a learned "yes" off for good | 446e2d8 |
| [b3e359d](https://github.com/grubiroza/stremio-origin-webos/commit/b3e359daa54889662309d0e828cc0cb56c13c070) | seek previews: a plain part from where the link led is one range at a time there too; that place is no longer dropped for every reader | feef8c6 |

## Changes of 2026-11-25 (not reported yet)

From a night of watching: an 8K file offered as any other and said to fail on its sound, the failed file kept as the
one to resume, a stream list that called converted sound unplayable, and subtitles in parts (one per disc) whose first
part ended mid-film.

| Commit | What |
|---|---|
| [02c8f54](https://github.com/grubiroza/stremio-origin-webos/commit/02c8f5400ccaa58e7bec441fa4074c8043e71d8f) | an 8K file (named 4320p or 8K) is known as one: above a 4K panel it is left out of the best pick and goes to the end of Sources, and its failure says "This TV can't play 8K video" (the TV refuses its picture, 601, often its sound first, 200: it was said as the sound); a stream that never played is not the remembered one (the one before it is put back); HEVC from the local stream, once seen playing, stays learned when one file's converted stream fails; the file's audio tracks in the log |
| [b5363cb](https://github.com/grubiroza/stremio-origin-webos/commit/b5363cb4537ad7e46c9aab831319b3d3899be0dd) | sound this TV can't decode but the app converts: the stream lists say "TrueHD sound: converted on this TV" (or DTS), not that it can't play, and after a too-heavy warning |
| [3d8400b](https://github.com/grubiroza/stremio-origin-webos/commit/3d8400b42c6ef6dfb7b3f8d2bc654c114a0b7d6f) | a subtitle in parts (titlovi.com lists each disc's file as a subtitle, timed from its own start): one row, the parts laid end to end, each placed by a whole subtitle of the film in the same language (words matched, Cyrillic read as Latin, the frame-rate ratio that fits), else by the lines both parts repeat, else after the last line; only while they fit the film |
| [ffad5ad](https://github.com/grubiroza/stremio-origin-webos/commit/ffad5adce61a9a5ce4a928add00656ccac3dfa63) | Auto-sync: the subtitle checked by itself at the start waits for the picture (a resumed film was checked at 0 s, where nothing was read, and the check gave up) |
| [a2d99d6](https://github.com/grubiroza/stremio-origin-webos/commit/a2d99d67eba0141f04dd9b4a1449be4b1fc4654d) | the Subtitles menu keeps the subtitles in view, beside it and smaller, so a subtitle just picked can be seen against the film |

## Changes of 2026-11-27 (not reported yet)

Dolby Vision for HDR10: on a TV with Dolby Vision, an HDR10 film is shown in the TV's Dolby Vision modes. Tried by
hand first (an HDR10 clip with a static profile 8.1 RPU, through the app's converter: the TV said DolbyVision), then
in the app on an LG C9 (webOS 4.5) with two HDR10 clips (AAC and E-AC3 sound, 3840x2160 and 3840x1608): both
DolbyVision, `dynamicRange: dolbyHdr`. The values are static (no new picture data), so what it brings is the TV's own
Dolby Vision processing; an HDR10+ source's dynamic values are not used (yet).

| Commit | What |
|---|---|
| [4c7fd02](https://github.com/grubiroza/stremio-origin-webos/commit/4c7fd02f99950b7ba7b7d609c0767ede1e864bae) | once the TV says the file plays as HDR10 (HEVC in Matroska, no Dolby Vision of its own, at most 35 Mbit/s), the picture goes the way a web copy's profile 8 goes (converted on the TV, from local HLS), and on its way (`dvts.c`) each frame gets a static profile 8.1 RPU made beforehand by dovi_tool 2.3.4 from the film's MaxCLL and mastering peak (`dvstatic_rpu.h`), and the PMT's HEVC entry the DOVI registration and the Dolby Vision video stream descriptor; the base layer is untouched. Settings > Playback > "Dolby Vision for HDR10" (on); its own lesson (`dvHdrHls`) |

## Changes of 2026-11-28 (not reported yet)

Dolby Vision for SDR: on a TV with Dolby Vision, an SDR film (HEVC 10-bit) is shown in the TV's Dolby Vision modes,
the way some streaming boxes send everything as Dolby Vision. The RPU maps the SDR signal into PQ (white at 203 nits,
or "Brighter" with the highlights spread up to ~500), checked first on the PC (libplacebo's own Dolby Vision decoding
gives the intended nits on real frames) and then on an LG C9 (webOS 4.5): a 4K SDR clip showed as DolbyVision,
`dynamicRange: dolbyHdr`, with the picture as the SDR one. Worth knowing for anyone making such RPUs: on this TV an RPU
whose luma polynomial had a coefficient of 73.7 gave no picture at all, while dovi_tool's 8.4 RPU (up to 61.9) plays.

The same day, H.264 too: the C9 takes Dolby Vision profile 9 (an 8-bit AVC base layer, SDR) from a TS stream. A test
pattern sent as H.264 with the 8-bit RPU and as HEVC 10-bit with the profile 8 one gave the same screenshot within
one code, so the TV reads the 8-bit pivots as meant.

Interlaced H.264 on this TV, for anyone trying the same: a Blu-ray 1080i25 copy (MBAFF, picture timing with
`ct_type` progressive) plays as profile 9; x264's interlaced (MBAFF) and fake-interlaced streams, with the same RPU,
make the pipeline die ("1000 pipeline died abnormally"), with or without audio, and play fine without it.

| Commit | What |
|---|---|
| [dedb6c6](https://github.com/grubiroza/stremio-origin-webos/commit/dedb6c61c97793dcea8150a8b0663bdbf0c758cf) | once the TV says the file plays as SDR (HEVC Main 10 in Matroska, no Dolby Vision of its own, at most 35 Mbit/s), the picture goes the HDR10 way above, with a static RPU that maps SDR (BT.709, gamma 2.4) into PQ: luma in 8 second-order pieces, chroma by third-order MMR fitted in ICtCp, BT.709 matrices and L9 (`dvsdr_rpu.h`); the descriptor marks the base layer SDR (compatibility 2). An RPU now spans as many TS packets as it needs. Settings > Playback > "Dolby Vision for SDR" (Off / On / Brighter; on); its own lesson (`dvSdrHls`); `tests/dvts` runs the filter on the PC |
| [3a6cc05](https://github.com/grubiroza/stremio-origin-webos/commit/3a6cc05d41b99f5fa64b5b82d36a2d5b400afdcb) | the same for an H.264 film (8-bit 4:2:0 by its avcC, progressive), as Dolby Vision **profile 9** (AVC base layer, SDR): the RPU made for an 8-bit base (`DVS_SDR8`), carried in a NAL unit of type 28 with the same two-byte header 7C 01 (as Dolby's dlb_mp4base and GPAC know it), the PMT's H.264 entry marked with profile 9; its own lesson (`dvSdr9Hls`) |
| [28805fb](https://github.com/grubiroza/stremio-origin-webos/commit/28805fb02396f56efef05a755e23d61c698fb4da) | after the first evening with real films: DTS is made Dolby Digital instead of copied (in the converted stream the TV saw no audio track at all); interlaced pictures are tried too (a Blu-ray 1080i25 copy plays with the RPU, x264's interlaced ones make the pipeline die: then the plain file, nothing learned); nothing is decided while the TV plays something else than the file (a debrid notice clip: its length against the Matroska Duration); error 600 behind another app is a pipeline to load again once back, not a failure (a paused film behind YouTube ended in "Playback error"); another audio track keeps the Dolby Vision of HDR10 / SDR |

## Local aids (not meant for upstream)

| Commit | What |
|---|---|
| [2548fb5](https://github.com/grubiroza/stremio-origin-webos/commit/2548fb554f956f3b368020e479fe5c6d315568a1) | with a file `update.pin` in the app's data folder, Install in Settings > About installs nothing (a patched build would lose its changes); without the file nothing changes |
| [ef556e5](https://github.com/grubiroza/stremio-origin-webos/commit/ef556e50ea0c376a1a136e082bcc70328f039c45) | pointer commands read from a file `dev.cmd` in the data folder, to try pointer changes without a hand on the remote; without the file nothing happens (the app only looks for it, four times a second) |

## How it was tried

On the one TV above, every change before it was kept:

- **screen:** pictures taken with `com.webos.service.tv.capture`;
- **keys:** written to the remote's input device;
- **pointer:** through the testing hook, since the Magic Remote cannot be driven from outside the TV's input stack;
- **log:** the app's own.

Each commit message says what was seen. The upstream reports say what was not tried. Other webOS versions and other
TVs were not tried at all.

## Building

As upstream builds it: `tools/build.sh webos`, with Zig 0.17.0 and the sysroot of the webOS SDK
([openlgtv/buildroot-nc4](https://github.com/openlgtv/buildroot-nc4), release `2026.08-webos`) given in `ZIG` and
`SDK`. A clean build of 1.1.1 made this way matches the official binary: the same libraries and the same 280
imported functions.

A build made without upstream's private keys file has no TMDB or Trakt key built in. Add your own in
Settings > Keys. The torrent and DTS libraries are taken from the official package of the same version.

## How it was made

The changes were written with an AI coding assistant (Claude Code) working on my PC and my TV, at my request. Each
one was run on the TV before it was kept; what was seen is in its commit message. If one of them is wrong for your
TV, the upstream report it belongs to is the place to say so.

Licence: GPL-3.0, as upstream.
