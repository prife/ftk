#include "ftk_style.h"

static const FtkColor s_colors[FTK_COLOR_NR] =
{
	[FTK_COLOR_WINDOW]       = {.r=0xff, .g=0xff,.b=0xff},
	[FTK_COLOR_WINDOWFRAME]  = {.r=0x00, .g=0x00,.b=0x00},
	[FTK_COLOR_WINDOWTEXT]   = {.r=0x00, .g=0x00,.b=0x00},
	[FTK_COLOR_GRAYTEXT]     = {.r=0xac, .g=0xa8,.b=0x99},
	[FTK_COLOR_HIGHLIGHT]    = {.r=0x31, .g=0x6a,.b=0xc5},
	[FTK_COLOR_BTNFACE]      = {.r=0xec, .g=0xe9,.b=0xd8},
	[FTK_COLOR_BTNHIGHLIGHT] = {.r=0xff, .g=0xff,.b=0xff},
	[FTK_COLOR_BTNSHADOW]    = {.r=0xac, .g=0xa8,.b=0x99},
	[FTK_COLOR_BTNTEXT]      = {.r=0x00, .g=0x00,.b=0x00}
};

FtkColor ftk_style_get_color(FtkColorIndex index)
{
	return s_colors[index];
}

