#include "ftk.h"

const char* pngs[] = 
{
	"testdata/Camera.png",
	"testdata/Notes.png",
	"testdata/RSS.png",
	"testdata/Calculator.png",
	"testdata/Twitter.png",
	"testdata/Clock.png",
	"testdata/Games.png",
	"testdata/Youtube.png",
	"testdata/AIM.png",
	"testdata/LastFm.png",
	"testdata/IP.png",
	"testdata/Acrobat.png",
	"testdata/Settings.png",
	"testdata/App",
	"testdata/Customize.png",
	"testdata/Skype.png",
	"testdata/Linkedin.png",
	"testdata/Calender.png",
	"testdata/Call.png",
	"testdata/Install.png",
	"testdata/Facebook.png",
	"testdata/iPod.png",
	"testdata/Chat.png",
	"testdata/WLM.png",
	"testdata/Flickr.png",
	"testdata/Photos.png",
	"testdata/Stock.png",
	"testdata/Delicious.png",
	"testdata/Maps.png",
	"testdata/iTunes.png",
	"testdata/Memory.png",
	"testdata/Weather.png",
	"testdata/Wordpress.png",
	"testdata/iradio.png",
	"testdata/Safari.png",
	"testdata/Google.png",
	"testdata/Yahoo.png",
	"testdata/VLC.png",
	"testdata/Sms.png",
	"testdata/Mail.png",
	NULL
};

static Ret button_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int j = 0;
	int width = 0;
	int height = 0;
	FtkGc gc = {0};
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	ftk_init(argc, argv);

	width = ftk_display_width(ftk_default_display());
	height = ftk_display_height(ftk_default_display()) - FTK_STATUS_PANEL_HEIGHT;
	gc.mask = FTK_GC_BITMAP;
	win = ftk_window_create(0, 0, width, height);
	for(i = 0; i < height/80; i++)
	{
		for(j = 0; j < width/80; j++)
		{
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), pngs[i + 1]);
			button = ftk_button_create(1000, j * 80, i * 80, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
			ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_append_child(win, button);
			ftk_widget_show(button, 1);
			ftk_bitmap_unref(gc.bitmap);

			if(i == 0 && j == 0)
			{
				ftk_button_set_clicked_listener(button, button_clicked, NULL);
				ftk_button_set_text(button, "Quit");
			}
		}
	}
	ftk_window_set_title(win, "button demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}
