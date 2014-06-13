#include "ftk.h"
#include "ftk_util.h"
#include <pthread.h>
#include <stdarg.h>
#include <unistd.h>
#include "ftk_source_primary.h"

#define LDR_DATA_VSPACE      50
#define PROGRESS_BAR_HIGHT   30
#define FTK_EVT_USR_DEF     100

pthread_t loader_ui_thread_id;
struct PrivateInfo
{
    FtkWidget* win;
    FtkWidget* label_title;
    FtkWidget* label_stepmsg;
    FtkWidget* progress_bar;
    FtkWidget* label_errcode;
    FtkWidget* label_time;
    FtkWidget* label_errmsg;
};
static struct PrivateInfo priv;

typedef enum
{
    LABEL_TITLE = 0,
    LABEL_STEPMSG,
    PROGRESS_BAR,
    LABEL_ERRCODE,
    LABEL_TIME,
    LABEL_ERRMSG,
} whoenum;

struct Msg
{
    whoenum who;
    union
    {
        int percent;
        char * text;
    }u;
};

static int ui_flag;
static int ui_response_flag;

static Ret (*win_on_event)(FtkWidget* thiz, FtkEvent* event);
static Ret loader_win_on_event(FtkWidget* thiz, FtkEvent* event)
{
    if (event->type == FTK_EVT_USR_DEF)
    {
        struct Msg *msg = (struct Msg *)(event->u.extra);
        switch (msg->who)
        {
        case PROGRESS_BAR:
            {
                static char text[32];
                ftk_itoa(text, sizeof(text), msg->u.percent);
                strcat(text, "%");
                ftk_progress_bar_set_percent(priv.progress_bar, msg->u.percent);
                ftk_widget_set_text(priv.progress_bar, text);

                //update the frontpad percent
                sprintf(text, "d%03d", msg->u.percent);
            }
            break;
        case LABEL_TITLE:
            ftk_widget_set_text(priv.label_title, msg->u.text);
            free(msg->u.text);
            break;
        case LABEL_STEPMSG:
            ftk_widget_set_text(priv.label_stepmsg, msg->u.text);
            free(msg->u.text);
            break;
        case LABEL_ERRCODE:
            ftk_widget_set_text(priv.label_errcode, msg->u.text);
            free(msg->u.text);
            break;
        case LABEL_TIME:
            ftk_widget_set_text(priv.label_time, msg->u.text);
            free(msg->u.text);
            break;
        case LABEL_ERRMSG:
            ftk_widget_set_text(priv.label_errmsg, msg->u.text);
            free(msg->u.text);
            break;
        }
        ui_response_flag = 1;
        free(msg);
        return RET_OK;
    }

    return win_on_event(thiz, event);
}

static Ret update_time(void* ctx)
{
    static int total_sec;
	FtkWidget* label = ctx;
    int hour, min, sec;
	char buf[16];

    total_sec ++;
    hour = total_sec / 3600;
    min = (total_sec % 3600 / 60);
    sec = total_sec % 60;
    snprintf(buf, sizeof(buf), "TIME: %02d:%02d:%02d", hour, min, sec);
	ftk_widget_set_text(label, buf);

	return RET_OK;
}

int wait_init(void)
{
    ui_response_flag = 0;
    return 0;
}

int wait_response(void)
{
    while (ui_response_flag == 0)
    {
        usleep(10000); //10ms
    }
    //TODO: add more
    return 0;
}

static FtkSource* minor_source;
static int loader_ui_main(int argc, char** argv)
{
    static const char* lab_table[] =
    {
        "MAN CODE:",
        "HW VER:",
        "SIG VER:",
        "LOAD VER:",
        "VARIANT:",
        "LOADER VER:"
    };
    FtkWidget* label = NULL;
    int i = 0;
    int width = 0;
    int height = 0;
    int x, y, w, h;
    FtkGc gc = {0};
    FtkWidget* win;

    ftk_init(argc, argv);

    /*1. creat window */
    priv.win = win = ftk_app_window_create();
    width = ftk_widget_width(win);
    height = ftk_widget_height(win);
    win_on_event = win->on_event;
    win->on_event = loader_win_on_event;
    ftk_window_set_fullscreen(win, 1);

    //init position
    y = 50;
    w = width * 3 / 4;
    x = (width - w) >> 1;
    h = 25;

    gc.mask = FTK_GC_BG;
    gc.bg.a = 0xff;
    gc.bg.r = 0xF0;
    gc.bg.g = 0xF0;
    gc.bg.b = 0x80;

    //label: LOADER DATA
    label = ftk_label_create(win, x, y, w, h);
    ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
    ftk_widget_unset_attr(label, FTK_ATTR_TRANSPARENT);
    priv.label_title = label;
    y += LDR_DATA_VSPACE;

    //label: LOADER DATA
    label = ftk_label_create(win, x, y, w, h);
    ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
    ftk_widget_unset_attr(label, FTK_ATTR_TRANSPARENT);
    ftk_widget_set_text(label, "LOADER DATA");
    ftk_label_set_alignment(label, FTK_ALIGN_CENTER);

    y += LDR_DATA_VSPACE;
    for(i = 0; i < 3; i++) {
        //row left
        label = ftk_label_create(win, x, y, w/4, h);
        ftk_widget_set_text(label, lab_table[i]);
        //value
        label = ftk_label_create(win, x+w/4, y, w/4, h);

        //row right
        label = ftk_label_create(win, x+w/2, y, w/4, h);
        ftk_widget_set_text(label, lab_table[3+i]);
        //value
        label = ftk_label_create(win, x+w/2+w/4, y, w/4, h);
        y += LDR_DATA_VSPACE;
    }

    //label: DOWNLOAD PROGRESS
    y += LDR_DATA_VSPACE;
    label = ftk_label_create(win, x, y, w, h);
    ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
    ftk_widget_unset_attr(label, FTK_ATTR_TRANSPARENT);
    ftk_widget_set_text(label, "DOWNLOAD PROGRESS");
    ftk_label_set_alignment(label, FTK_ALIGN_CENTER);
    y += LDR_DATA_VSPACE;

    //label:NULL
    label = ftk_label_create(win, x, y, w, h);
    //ftk_widget_set_text(label, "Reading download Header");
    priv.label_stepmsg = label;
    y += LDR_DATA_VSPACE;

    //progress bar:
    priv.progress_bar = ftk_progress_bar_create(win, x, y, w, PROGRESS_BAR_HIGHT);
    ftk_widget_set_text(priv.progress_bar, "0%");
    y += LDR_DATA_VSPACE;

    //ERRCODE & TIME
    label = ftk_label_create(win, x, y, w/2, h);
    //ftk_widget_set_text(label, "ERROR:0x80030003");
    priv.label_errcode = label;
    label = ftk_label_create(win, x + w/2 + w/4, y, w/4, h);
    ftk_label_set_alignment(label, FTK_ALIGN_RIGHT);
    //ftk_widget_set_text(label, "TIME: 0 MIN");
    priv.label_time = label;
    y += LDR_DATA_VSPACE;

    label = ftk_label_create(win, x, y, w, h);
    //ftk_widget_set_text(label, "Download header could not be acquired");
    priv.label_errmsg = label;

    //add soft timer to update the TIME
    {
        FtkSource* timer = NULL;
        timer = ftk_source_timer_create(1000, update_time, priv.label_time);
        ftk_main_loop_add_source(ftk_default_main_loop(), timer);
    }

    {
        minor_source = ftk_source_primary_create((FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
        ftk_main_loop_add_source(ftk_default_main_loop(), minor_source);
    }

    //run
    ftk_widget_show_all(win, 1);

    ui_flag = 1;
    ftk_run();

    return 0;
}

#include "ftk_log.h"
#include "ftk_event.h"
#include "ftk_globals.h"
#include "ftk_main_loop.h"

static int _ldrui_set_label_text(int who, const char * string)
{
    struct Msg *msg = malloc(sizeof(struct Msg));
    FtkEvent event;

    //init msg
    msg->who = who;
    msg->u.text = strdup(string); //note: string must be dupped here!

    //init event
    ftk_event_init(&event, FTK_EVT_USR_DEF);
    event.widget = priv.win;
    event.u.extra = (void*)msg;

    //send event
    //wait_init();
    ftk_source_queue_event(ftk_primary_source(), &event);
    //wait_response();
    return 0;
}

int ldrui_set_errcode(const char *fmt, ... )
{
    char buf[64];
    int ret = 0;
	va_list args;
	va_start(args, fmt);
	ret = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
    _ldrui_set_label_text(LABEL_ERRCODE, buf);

    return ret;
}

int ldrui_set_title(const char * str)
{
    return _ldrui_set_label_text(LABEL_TITLE, str);
}

int ldrui_set_stepmsg(const char * str)
{
    return _ldrui_set_label_text(LABEL_STEPMSG, str);
}

int ldrui_set_time(const char * str)
{
    return _ldrui_set_label_text(LABEL_TIME, str);
}

int ldrui_set_errmsg(const char * str)
{
    return _ldrui_set_label_text(LABEL_ERRMSG, str);
}

int ldrui_set_progress(int percent)
{
    FtkEvent event;
    struct Msg *msg = malloc(sizeof(struct Msg));

    //init msg
    msg->who = PROGRESS_BAR;
    msg->u.percent = percent;

    //init event
    ftk_event_init(&event, FTK_EVT_USR_DEF);
    event.widget = priv.win;
    event.u.extra = (void*)msg;

    //send event
    //wait_init();
    //ftk_source_queue_event(ftk_primary_source(), &event);
    assert(minor_source != NULL);
    ftk_source_queue_event(minor_source, &event);
    //wait_response();

    return 0;
}

/* pthread stuff */
static void* loader_ui_thread_entry(void * para)
{
    char * argv[] = {"loader_ui"};
    loader_ui_main(1, argv);

    return NULL;
}

int loaderui_thread_start(void)
{
    if (pthread_create(&loader_ui_thread_id, NULL, loader_ui_thread_entry, NULL) != 0)
        return -1;

    /* FIXME */
    while (ui_flag == 0)
        sleep(1);

    return 0;
}

int loaderui_thread_end(void)
{
    return pthread_join(loader_ui_thread_id, NULL);
}

