#include "ftk_util.h"
int main(int argc, char* argv[])
{
	int i = 0;
	unsigned short codes[] = {0x9690, 0x5f0f, 0x58f0, 0x660e, 0x4e0e, 0x5185, 0x5efa, 0x51fd, 0x6570};
	const char* str = "隐式声明与内建函数";
	const char* p = str;

	for(i = 0; *p; i++)
	{
		assert(codes[i] == utf8_get_char(p, &p));	
	}

	return 0;
}
