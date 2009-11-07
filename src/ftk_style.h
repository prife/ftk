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

Ret ftk_style_fill_button_background(FtkWidget* button);
Ret ftk_style_fill_menu_item_button_background(FtkWidget* button);
Ret ftk_style_fill_background(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap);

#endif/*FTK_STYLE_H*/

