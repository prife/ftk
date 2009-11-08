#ifndef FTK_STYLE_H
#define FTK_STYLE_H

#include "ftk_widget.h"
#include "ftk_typedef.h"

typedef enum _FtkColorIndex
{
	FTK_COLOR_WINDOW = 0,
	FTK_COLOR_WINDOWFRAME,
	FTK_COLOR_BORDER,
	FTK_COLOR_WINDOWTEXT,
	FTK_COLOR_GRAYTEXT,
	FTK_COLOR_HIGHLIGHT,
	FTK_COLOR_BTNFACE,
	FTK_COLOR_BTNHIGHLIGHT,
	FTK_COLOR_BTNSHADOW,
	FTK_COLOR_BTNTEXT,
	FTK_COLOR_MENU_BG,
	FTK_COLOR_MENU_FG,
	FTK_COLOR_NR
}FtkColorIndex;

FtkColor ftk_style_get_color(FtkColorIndex index);

typedef enum _FtkBgStyle
{
	FTK_BG_NORMAL = 0,
	FTK_BG_TILE,
	FTK_BG_CENTER,
	FTK_BG_FOUR_CORNER
}FtkBgStyle;

Ret ftk_style_fill_background_image(FtkCanvas* canvas, FtkBgStyle style, int x, int y, int w, int h, FtkBitmap* bitmap);

#endif/*FTK_STYLE_H*/

