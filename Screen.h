
/* Game display */

#ifndef SCREEN_H
#define SCREEN_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define WIDTH  320
#define HEIGHT 256

struct Display
{
	struct BitMap *bitmaps[2];
	struct Screen *screen;
	struct ScreenBuffer *buffers[2];
	struct MsgPort *safePort, *dispPort;
	BOOL safe, disp;
	UWORD toggleFrame;
};

/* Open display. PrepBitMap() prepares initial graphics and returns RGB color table. */

struct Display *openDisplay(UBYTE depth, ULONG *(*prepBitMap)(struct Display *disp, APTR user), APTR user);

void closeDisplay(struct Display *disp);

#endif
