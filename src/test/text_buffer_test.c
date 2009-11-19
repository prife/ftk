#include "ftk_text_buffer.h"

void english_test(void)
{
	FtkTextBuffer* thiz = ftk_text_buffer_create(6);
	assert(ftk_text_buffer_insert(thiz, 0, "b") == RET_OK);
	assert(strcmp(thiz->buffer, "b") == 0);
	assert(ftk_text_buffer_insert(thiz, 1, "c") == RET_OK);
	assert(strcmp(thiz->buffer, "bc") == 0);
	assert(ftk_text_buffer_insert(thiz, 0, "a") == RET_OK);
	assert(strcmp(thiz->buffer, "abc") == 0);
	assert(ftk_text_buffer_insert(thiz, 4, "a") != RET_OK);
	assert(strcmp(thiz->buffer, "abc") == 0);
	assert(ftk_text_buffer_insert(thiz, 3, "defg") == RET_OK);
	assert(strcmp(thiz->buffer, "abcdefg") == 0);

	assert(ftk_text_buffer_delete_chars(thiz, 0, 1) == RET_OK);
	assert(strcmp(thiz->buffer, "bcdefg") == 0);
	
	assert(ftk_text_buffer_delete_chars(thiz, 0, 2) == RET_OK);
	assert(strcmp(thiz->buffer, "defg") == 0);
	
	assert(ftk_text_buffer_delete_chars(thiz, 1, 2) == RET_OK);
	assert(strcmp(thiz->buffer, "dg") == 0);
	
	assert(ftk_text_buffer_delete_chars(thiz, 1, -1) == RET_OK);
	assert(strcmp(thiz->buffer, "g") == 0);
	assert(ftk_text_buffer_delete_chars(thiz, 1, -1) == RET_OK);
	assert(strcmp(thiz->buffer, "") == 0);

	ftk_text_buffer_destroy(thiz);

	return;
}

void chinese_test(void)
{
	int offset = 0;
	FtkTextBuffer* thiz = ftk_text_buffer_create(6);
	assert(ftk_text_buffer_insert(thiz, 0, "中文汉字测试") == RET_OK);
	offset  = ftk_text_buffer_chars_bytes(thiz, 0, 2);
	assert(ftk_text_buffer_delete_chars(thiz, offset, -2) == RET_OK);
	assert(strcmp(thiz->buffer, "汉字测试") == 0);
	
	assert(ftk_text_buffer_delete_chars(thiz, 0, 2) == RET_OK);
	assert(strcmp(thiz->buffer, "测试") == 0);
	
	assert(ftk_text_buffer_delete_chars(thiz, 0, 1) == RET_OK);
	assert(strcmp(thiz->buffer, "试") == 0);
	
	offset  = ftk_text_buffer_chars_bytes(thiz, 0, 1);
	assert(ftk_text_buffer_delete_chars(thiz, offset, -1) == RET_OK);
	assert(strcmp(thiz->buffer, "") == 0);

	ftk_text_buffer_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{
	english_test();
	chinese_test();

	return 0;
}
