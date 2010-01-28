#include "ftk.h"

int main(int argc, char* argv[])
{
	const char* text = NULL;
	assert(!ftk_clipboard_has_data());
	assert(ftk_clipboard_set_text(NULL) != RET_OK);
	assert(ftk_clipboard_set_text("aaa") == RET_OK);
	assert(ftk_clipboard_get_text(&text) == RET_OK && strcmp(text, "aaa") == 0);

	return 0;
}
