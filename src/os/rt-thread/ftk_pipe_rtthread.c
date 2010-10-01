
#include "ftk_pipe.h"
#include "ftk_allocator.h"
#include "ftk_select_rtthread.h"

struct _FtkPipe
{
    int  fd;
    char pipe[2048];
    struct rt_messagequeue mq;
};

FtkPipe* ftk_pipe_create(void)
{
    FtkPipe* thiz = FTK_ZALLOC(sizeof(FtkPipe));
    if (thiz != NULL)
    {
        rt_mq_init(&thiz->mq, "ftkpipe", 
		           &thiz->pipe[0], 
		           256, 
		           sizeof(thiz->pipe), 
		           RT_IPC_FLAG_FIFO); 
        
        thiz->fd = ftk_select_fd_alloc();
    }
    return thiz;
}

void ftk_pipe_destroy(FtkPipe* thiz)
{
    if (thiz != NULL)
    {
        ftk_select_fd_free(thiz->fd);

		rt_mq_detach(&thiz->mq);

        FTK_ZFREE(thiz, sizeof(*thiz));
    }
}

int ftk_pipe_get_read_handle(FtkPipe* thiz)
{
    return thiz != NULL ? thiz->fd : -1;
}

int ftk_pipe_get_write_handle(FtkPipe* thiz)
{
    return thiz != NULL ? thiz->fd : -1;
}

int ftk_pipe_read(FtkPipe* thiz, void* buff, size_t length)
{							 
    return_val_if_fail(thiz != NULL && buff != NULL, -1);

    rt_mq_recv(&thiz->mq, buff, length, RT_WAITING_FOREVER);

    return length;
}

int ftk_pipe_check(FtkPipe* thiz)
{
	if (thiz->mq.entry > 0)
	{
		ftk_set_file_readble(thiz->fd);
	}

	return -1;
}

int ftk_pipe_write(FtkPipe* thiz, const void* buff, size_t length)
{
    return_val_if_fail(thiz != NULL && buff != NULL, -1);

    rt_mq_send(&thiz->mq, buff, length);
    
    ftk_set_file_readble(thiz->fd);

    return length;
}
