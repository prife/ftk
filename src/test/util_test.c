#include "ftk_util.h"
const char* normalize_path_name(const char* in, char* out)
{
	if(in != NULL)
	{
		strcpy(out, in);
		ftk_normalize_path(out);
		return out;
	}
	else
	{
		return NULL;
	}
}

void test_normalize_path()
{
	char path_out[FTK_MAX_PATH + 1] = {0};

	assert(normalize_path_name(NULL, path_out) == NULL);
	assert(normalize_path_name(NULL, NULL) == NULL);
	assert(strcmp(normalize_path_name("/test", path_out), "/test") == 0);
	assert(strcmp(normalize_path_name("/test/.", path_out), "/test/") == 0);
	assert(strcmp(normalize_path_name("/test/..", path_out), "/") == 0);
	assert(strcmp(normalize_path_name("/test/../..", path_out), "/") == 0);
	assert(strcmp(normalize_path_name("/test/../test/..", path_out), "/") == 0);
	assert(strcmp(normalize_path_name("/test.txt", path_out), "/test.txt") == 0);
	assert(strcmp(normalize_path_name("//////test.txt", path_out), "/test.txt") == 0);
	assert(strcmp(normalize_path_name("/./././././test.txt", path_out), "/test.txt") == 0);

	chdir("/home");
	assert(strcmp(normalize_path_name("test.txt", path_out), "/home/test.txt") == 0);
	assert(strcmp(normalize_path_name("./test.txt", path_out), "/home/test.txt") == 0);
	assert(strcmp(normalize_path_name("./", path_out), "/home/") == 0);
	assert(strcmp(normalize_path_name(".", path_out), "/home") == 0);

	setenv("HOME", "/home/lixianjing", 1);
	assert(strcmp(normalize_path_name("~/test.txt", path_out), "/home/lixianjing/test.txt") == 0);
	assert(strcmp(normalize_path_name("~/", path_out), "/home/lixianjing/") == 0);
	assert(strcmp(normalize_path_name("~", path_out), "/home/lixianjing") == 0);

	return 0;
}

int main(int argc, char* argv[])
{
	int i = 0;
	unsigned short codes[] = {0x9690, 0x5f0f, 0x58f0, 0x660e, 0x4e0e, 0x5185, 0x5efa, 0x51fd, 0x6570};
	const char* str = "隐式声明与内建函数";
	const char* p = str;

	for(i = 0; *p; i++)
	{
		assert(codes[i] == utf8_get_char(p, &p));	
		assert(codes[i] == utf8_get_prev_char(p, NULL));
	}
	assert(utf8_count_char(str, strlen(str)) == 9);
	str = "abc";
	assert(utf8_count_char(str, strlen(str)) == 3);
	str = "abc中国";
	assert(utf8_count_char(str, strlen(str)) == 5);
	
	str = "abc中国";
	assert(utf8_count_char(str, 3) == 3);

	test_normalize_path();

	return 0;
}
