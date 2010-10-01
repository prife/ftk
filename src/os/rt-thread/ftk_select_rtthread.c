
#include "ftk_rtthread.h"
#include "ftk_select_rtthread.h"
#include "ftk_pipe.h"

static struct rt_event 	ftk_event;
static rt_uint32_t 		ftk_select_fd_pool;

void ftk_select_init(void)
{
	rt_event_init(&ftk_event, "ftk_evt", RT_IPC_FLAG_FIFO);

	ftk_select_fd_pool = 0;
}

void ftk_select_deinit(void)
{
	rt_event_detach(&ftk_event);
}

int ftk_select(int mfd, fd_set *fdset, struct timeval *tv)
{
	rt_uint32_t tick = 0;
    rt_uint32_t flags = 0;
	int i = 0, n = 0;

	if (tv != NULL)
	{
		tick = (rt_uint32_t)(tv->tv_sec + tv->tv_usec/1000000)/RT_TICK_PER_SECOND;
	}
	else
	{
	    tick = 3*RT_TICK_PER_SECOND;
	}

	flags = fdset->fds_bits[0];
	FD_ZERO(fdset);;

	rt_event_recv(&ftk_event, flags, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, (rt_uint32_t)tick, &fdset->fds_bits[0]);

	for (i=0; i<32; i++)
	{
		if (fdset->fds_bits[0] & (0x01<<i))
		{
			n++;
		}
	}

	return n;
}

int ftk_set_file_readble(int fd)
{
    rt_event_send(&ftk_event, (0x01<<fd));
}

int ftk_select_fd_alloc(void)
{
	int	i;

	for (i=0; i<32; i++)
	{
		if (!(ftk_select_fd_pool & (0x01<<i)))
		{
			ftk_select_fd_pool |= (0x01<<i);
			return i;	
		}
	}
	return -1;
}

void ftk_select_fd_free(int fd)
{
    ftk_select_fd_pool &= ~(0x01<<fd);
}
