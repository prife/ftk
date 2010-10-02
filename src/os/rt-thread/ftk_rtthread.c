
#include "ftk_typedef.h"
#include <rtthread.h>

size_t ftk_get_relative_time(void)
{
	rt_tick_t tick = rt_tick_get();

	return tick/RT_TICK_PER_SECOND*1000;
}

static int ftk_deinited = 0;

static void ftk_cleanup(void)
{
    extern void ftk_deinit(void);

    if (ftk_deinited == 0)
    {
        ftk_deinited = 1;

        ftk_deinit();
    }
}

int  ftk_platform_init(int argc, char** argv)
{
	void ftk_select_init(void);

	ftk_select_init();

    return RET_OK;
}

void ftk_platform_deinit(void)
{
	void ftk_select_deinit(void);

    ftk_deinited = 1;

	ftk_select_deinit();
}

void _exit(int exit_code)
{
	rt_thread_detach(rt_thread_self());
}

static const int N=100;

double coef(int n)
{
 	double t;

	if (n == 0) 
	{
		return 0;
	}

 	t = 1.0/n;

 	if (n%2==0) 
	{
		t = -t;
	}

 	return t;
}

double horner(double x)
{
	double u = coef(N);
 	int i;

 	for(i=N-1; i>=0; i--)
	{
  		u = u*x + coef(i);
	}

 	return u;
}

double sqrt(double b)
{
 	double x = 1;
	int step = 0;

 	while ((x*x-b<-0.000000000000001 || x*x-b>0.000000000000001) && step<50)
 	{
		x = (b/x+x)/2.0;
		step++;
	}
 	return x;
}

double ln(double x)
{
	int i;

 	if (x > 1.5)
 	{
  		for(i=0; x>1.25; i++)
		{
   			x=sqrt(x);
		}
  		return (1<<i)*horner(x-1);
 	}
 	else if (x<0.7 && x>0)
 	{
  		for(i=0; x<0.7; i++)
		{
   			x=sqrt(x);
		}
  		return (1<<i)*horner(x-1);
 	}
 	else if(x > 0)
	{
  		return horner(x-1);
	}
}

double log(double m,double base)
{
 	return ln(m)/ln(base);
}

double exp(double x)
{
	double sum = 1;
 	int i;

 	for(i=N; i>0; i--)
 	{ 
	  	sum /= i;
	  	sum *= x;
	  	sum += 1;
 	}
 	return sum;
}

double pow(double m, double n)
{
	return exp(n*ln(m));
}
