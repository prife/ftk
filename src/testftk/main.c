#include <stdio.h>
#include <unistd.h>

int main()
{
    int i;
    loaderui_thread_start();
    for (i=0; i<10000; i++)
    {
        ldrui_set_progress(i%100);
        //sleep(1);
    }

    loaderui_thread_end();
    return 0;
}
