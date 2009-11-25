#include "ftk.h"

const char* pngs[] = 
{
	TESTDATA_DIR"/Camera.png",
	TESTDATA_DIR"/Notes.png",
	TESTDATA_DIR"/RSS.png",
	TESTDATA_DIR"/Calculator.png",
	TESTDATA_DIR"/Twitter.png",
	TESTDATA_DIR"/Clock.png",
	TESTDATA_DIR"/Games.png",
	TESTDATA_DIR"/Youtube.png",
	TESTDATA_DIR"/AIM.png",
	TESTDATA_DIR"/LastFm.png",
	TESTDATA_DIR"/IP.png",
	TESTDATA_DIR"/Acrobat.png",
	TESTDATA_DIR"/Settings.png",
	TESTDATA_DIR"/App",
	TESTDATA_DIR"/Customize.png",
	TESTDATA_DIR"/Skype.png",
	TESTDATA_DIR"/Linkedin.png",
	TESTDATA_DIR"/Calender.png",
	TESTDATA_DIR"/Call.png",
	TESTDATA_DIR"/Install.png",
	TESTDATA_DIR"/Facebook.png",
	TESTDATA_DIR"/iPod.png",
	TESTDATA_DIR"/Chat.png",
	TESTDATA_DIR"/WLM.png",
	TESTDATA_DIR"/Flickr.png",
	TESTDATA_DIR"/Photos.png",
	TESTDATA_DIR"/Stock.png",
	TESTDATA_DIR"/Delicious.png",
	TESTDATA_DIR"/Maps.png",
	TESTDATA_DIR"/iTunes.png",
	TESTDATA_DIR"/Memory.png",
	TESTDATA_DIR"/Weather.png",
	TESTDATA_DIR"/Wordpress.png",
	TESTDATA_DIR"/iradio.png",
	TESTDATA_DIR"/Safari.png",
	TESTDATA_DIR"/Google.png",
	TESTDATA_DIR"/Yahoo.png",
	TESTDATA_DIR"/VLC.png",
	TESTDATA_DIR"/Sms.png",
	TESTDATA_DIR"/Mail.png",
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
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), pngs[i + 2]);
			button = ftk_button_create(win, j * 80, i * 80, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
			if(i == 0)
			{
				ftk_widget_unset_attr(button, FTK_ATTR_TRANSPARENT);
			}
			ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), pngs[1]);
			ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), pngs[0]);
			ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
			ftk_bitmap_unref(gc.bitmap);

			if(i == 0 && j == 0)
			{
				ftk_button_set_clicked_listener(button, button_clicked, NULL);
				ftk_widget_set_text(button, "Quit");
			}
		}
	}
	ftk_widget_set_text(win, "image button demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}
