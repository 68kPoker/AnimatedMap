
/* Game display */

#include <exec/memory.h>
#include <intuition/screens.h>

#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>

#include "Screen.h"

struct Display *openDisplay(UBYTE depth, ULONG *(*prepBitMap)(struct Display *disp, APTR user), APTR user)
{
	struct Display *disp;
	ULONG *colors;
	WORD pens[] = { ~0 };

	if (disp = AllocMem(sizeof(*disp), MEMF_PUBLIC | MEMF_CLEAR))
	{
		if (disp->bitmaps[0] = AllocBitMap(WIDTH, HEIGHT, depth, BMF_DISPLAYABLE | BMF_INTERLEAVED, NULL))
		{
			if (disp->bitmaps[1] = AllocBitMap(WIDTH, HEIGHT, depth, BMF_DISPLAYABLE | BMF_INTERLEAVED, NULL))
			{
				colors = prepBitMap(disp, user);

				if (disp->screen = OpenScreenTags(NULL,
					SA_DisplayID, DEFAULT_MONITOR_ID | LORES_KEY,
					SA_BitMap, disp->bitmaps[0],
					SA_Colors32, colors,
					SA_Quiet, TRUE,
					SA_ShowTitle, FALSE,
					SA_Exclusive, TRUE,
					SA_Draggable, FALSE,
					SA_BackFill, LAYERS_NOBACKFILL,
					SA_Title, "AnimatedMap Screen",
					SA_Pens, pens,
					SA_SharePens, TRUE,
					TAG_DONE))
				{
					if (disp->buffers[0] = AllocScreenBuffer(disp->screen, disp->bitmaps[0], 0))
					{
						if (disp->buffers[1] = AllocScreenBuffer(disp->screen, disp->bitmaps[1], 0))
						{
							if (disp->safePort = CreateMsgPort())
							{
								if (disp->dispPort = CreateMsgPort())
								{
									disp->safe = disp->disp = TRUE;
									disp->toggleFrame = 1;

									disp->buffers[0]->sb_DBufInfo->dbi_SafeMessage.mn_ReplyPort = disp->safePort;
									disp->buffers[1]->sb_DBufInfo->dbi_SafeMessage.mn_ReplyPort = disp->safePort;
									disp->buffers[0]->sb_DBufInfo->dbi_DispMessage.mn_ReplyPort = disp->dispPort;
									disp->buffers[1]->sb_DBufInfo->dbi_DispMessage.mn_ReplyPort = disp->dispPort;

									return(disp);
								}
								DeleteMsgPort(disp->safePort);
							}
							FreeScreenBuffer(disp->screen, disp->buffers[1]);
						}
						FreeScreenBuffer(disp->screen, disp->buffers[0]);
					}
					CloseScreen(disp->screen);
				}
				FreeBitMap(disp->bitmaps[1]);
			}
			FreeBitMap(disp->bitmaps[0]);
		}
		FreeMem(disp, sizeof(*disp));
	}
	return(NULL);
}

void closeDisplay(struct Display *disp)
{
	if (!disp->disp)
	{
		while (!GetMsg(disp->dispPort))
		{
			Wait(1L << disp->dispPort->mp_SigBit);
		}
	}

	if (!disp->safe)
	{
		while (!GetMsg(disp->safePort))
		{
			Wait(1L << disp->safePort->mp_SigBit);
		}
	}

	DeleteMsgPort(disp->dispPort);
	DeleteMsgPort(disp->safePort);
	FreeScreenBuffer(disp->screen, disp->buffers[1]);
	FreeScreenBuffer(disp->screen, disp->buffers[0]);
	CloseScreen(disp->screen);
	FreeBitMap(disp->bitmaps[1]);
	FreeBitMap(disp->bitmaps[0]);
	FreeMem(disp, sizeof(*disp));
}
