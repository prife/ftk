#include <cstdio>
#include <cassert>
#include "pinyinime.h"
using namespace ime_pinyin;

int main(int argc, char* argv[])
{
	size_t i = 0;
	size_t nr = 0;
	size_t size = 0;
	char16 str[64] = {0};
	bool ret = im_open_decoder("./dict_pinyin.dat", "./user.dat");
	assert(ret);

	im_set_max_lens(32, 16);
	im_reset_search();
	nr = im_search("xian'jin", 8);
	printf("%s", im_get_sps_str(&size));
	for(i = 0; i < nr; i++)
	{
		im_get_candidate(i, str, 32);
	}

	im_close_decoder();

	return 0;
}
