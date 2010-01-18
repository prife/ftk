// demo_button.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo_button.h"

static int argc = 1;
static char* argv[] = {"ftk", NULL};
extern "C" int ftk_main(int argc, char* argv[]);
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	ftk_main(argc, argv);

	return 0;
}


