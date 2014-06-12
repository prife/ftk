#ifndef LOADER_UI
#define LOADER_UI

int loaderui_thread_start(void);
int loaderui_thread_end(void);

int ldrui_set_progress(int percent);
int ldrui_set_errmsg(const char * str);
int ldrui_set_time(const char * str);
int ldrui_set_stepmsg(const char * str);
int ldrui_set_errcode(const char *fmt, ... );
int ldrui_set_title(const char * str);

#endif
