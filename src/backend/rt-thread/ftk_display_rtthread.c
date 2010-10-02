
#include <stdlib.h>
#include "ftk_log.h"
#include "ftk_display_mem.h"
#include "ftk_display_rtthread.h"

#define LCD_WIDTH 			240
#define LCD_HEIGHT 			320
			
#define RT_HW_LCD_WIDTH		LCD_WIDTH
#define RT_HW_LCD_HEIGHT	LCD_HEIGHT

struct LcdInfo
{
    void* bits;
	int bits_per_pixel;
	int width;
	int height;
	int fd;
};

static int  lcd_open(struct LcdInfo* lcd, const char *filename)
{
	extern rt_uint16_t _rt_hw_framebuffer[RT_HW_LCD_HEIGHT][RT_HW_LCD_WIDTH];

	lcd->bits 		    = _rt_hw_framebuffer;
	lcd->bits_per_pixel = 16;
	lcd->width 			= LCD_WIDTH;
	lcd->height 		= LCD_HEIGHT;
	lcd->fd 			= 1;

 	return lcd->fd;
}

static void lcd_close(struct LcdInfo* lcd)
{
    if (lcd != NULL)
    {
        FTK_FREE(lcd);
    }
}

FtkDisplay* ftk_display_lcd_create(const char* filename)
{
    FtkDisplay* thiz = NULL;
    struct LcdInfo* lcd = NULL;
	FtkPixelFormat format = 0;

    lcd = FTK_ZALLOC(sizeof(struct LcdInfo));
    return_val_if_fail(lcd != NULL, NULL);

	if (lcd_open(lcd, filename) > 0)
	{
        if (lcd->bits_per_pixel == 16)
        {
            format = FTK_PIXEL_RGB565;
        }
        else if (lcd->bits_per_pixel == 24)
        {
            format = FTK_PIXEL_BGR24;
        }
        else if (lcd->bits_per_pixel == 32)
        {
            format = FTK_PIXEL_BGRA32;
        }
        else
        {
            assert(!"not supported framebuffer format.");
        }

        thiz = ftk_display_mem_create(format, lcd->width, lcd->height,
                lcd->bits, (FtkDestroy) lcd_close, lcd);
	}

    return thiz;
}
