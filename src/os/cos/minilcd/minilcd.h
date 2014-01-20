#ifndef _MINIGL_H
#define _MINIGL_H

void hw_lcd_init(void);

int hw_lcd_get_screen_width(void);

int hw_lcd_get_screen_height(void);

void hw_lcd_sync(void);

int hw_lcd_get_cpuaddr(void **addr);
#endif // _MINIGL_H

