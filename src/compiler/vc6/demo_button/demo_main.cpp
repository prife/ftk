// demo_button.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static int argc = 1;
static char* argv[] = {"ftk", NULL};
extern "C" int ftk_main(int argc, char* argv[]);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	return ftk_main(argc, argv);
}



