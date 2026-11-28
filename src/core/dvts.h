/* domar (0056): an HDR10 film shown as Dolby Vision. The converter (libsbdts) copies the picture into MPEG-TS as it
   is; this filter, between it and the TV, adds to each frame a static Dolby Vision 8.1 RPU (a NAL unit of type 62 at
   the end of the frame, made beforehand from the film's own HDR10 values: dvstatic_rpu.h) and marks the video's entry
   in the PMT with the DOVI registration and the Dolby Vision video stream descriptor. The base layer is not touched:
   a TV without Dolby Vision would still see plain HDR10. Whole TS packets in, whole packets out; a frame waits for the
   next frame's start (its own end is known only then), and the last one for the end of the segment. */
#ifndef SB_DVTS_H
#define SB_DVTS_H
typedef int (*dvts_out)(void *ud, const unsigned char *p, int n);   /* n, or < 0 to stop */
typedef struct DvTs DvTs;
/* what one segment hands the next: the video's continuity counter goes on across them, and the PIDs are known before
   the segment's own tables (a segment that continues the one before may start with video). DVTS_CARRY0 at the first */
typedef struct { int base, lastOrig, vidPid, pmtPid; } DvtsCarry;
#define DVTS_CARRY0 { 0, -1, -1, -1 }
/* maxCll / mastMax in nits (0 not known); level the Dolby Vision level for the descriptor */
DvTs *dvts_new(int maxCll, int mastMax, int level, const DvtsCarry *carry, dvts_out out, void *ud);
/* domar (0057): an SDR picture (BT.709, HEVC Main 10) as Dolby Vision: its RPU maps SDR into PQ (dvsdr_rpu.h), the
   descriptor says the base layer is SDR (compatibility 2); bright 0 white at 203 nits, 1 the highlights spread above */
DvTs *dvts_new_sdr(int bright, int level, const DvtsCarry *carry, dvts_out out, void *ud);
int   dvts_sdr_preset(int bright, int *white, int *peak);          /* the preset it takes and its white / peak (nits, for the log) */
int   dvts_write(void *f, const unsigned char *p, int n);          /* the converter's writer: n, or < 0 to stop */
int   dvts_flush(DvTs *f);                                         /* the last frame and whatever waits: 0, < 0 failed */
void  dvts_stats(const DvTs *f, long *rpus, long *pmts, long *odd);/* RPUs added, PMTs marked, packets passed as they were */
void  dvts_carry(const DvTs *f, DvtsCarry *carry);                 /* for the next segment's dvts_new */
void  dvts_free(DvTs *f);
void  dvts_pick(int maxCll, int mastMax, int *cll, int *mast);     /* the values of the RPU it takes (for the log) */
int   dvts_level(int w, int h, double fps);                        /* the Dolby Vision level for that picture */
#endif
