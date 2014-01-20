#include "ftk.h"

int main(int argc, char** argv)
{
    int width = 0;
    int height = 0;
    FtkWidget* win = NULL;
    FtkWidget* label = NULL;
    ftk_init(argc, argv);

    win = ftk_app_window_create();
    ftk_window_set_animation_hint(win, "app_main_window");
    width = ftk_widget_width(win);
    height = ftk_widget_height(win);
    label = ftk_label_create(win, 270, 130+615, 340, height);

    ftk_widget_set_text(win, "image demo");
    ftk_widget_show_all(win, 1);

    FTK_QUIT_WHEN_WIDGET_CLOSE(win);
    FTK_RUN();
    return 0;
}

