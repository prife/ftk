#include <signal.h>
#include "ftk_typedef.h"

static void signal_handler(int sig)
{
	int i = 0;
	void* buffer[100] = {0};
	int n = backtrace(buffer, 100);
	const char** symbols = (const char**)backtrace_symbols(buffer, n);

	for(i = 0; i < n; i++)
	{
		printf("%p: %s\n", buffer[i], symbols[i]);
	}

	fflush(stdout);

	signal(SIGABRT, NULL);
	signal(SIGSEGV, NULL);
	signal(SIGILL,  NULL);
	signal(SIGKILL, NULL);

	_exit(0);

	return;
}

void ftk_install_crash_signal(void)
{
	signal(SIGABRT, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGKILL, signal_handler);
	
	return ;
}

