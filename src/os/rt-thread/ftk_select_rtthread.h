
#ifndef __FTK_SELECT_RTTHREAD_H__
#define __FTK_SELECT_RTTHREAD_H__

int ftk_fd_isset(int fd, fd_set *fdset);

int ftk_select(int mfd, fd_set *fdset, struct timeval *tv);

int ftk_select_fd_alloc(void);

void ftk_select_fd_free(int fd);

int ftk_set_file_readble(int fd);

#endif // __FTK_SELECT_RTTHREAD_H__
