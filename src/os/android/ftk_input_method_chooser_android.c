
#include "ftk_input_method_chooser.h"

extern void Android_ShowInputMethodChooser();

int ftk_input_method_chooser(void)
{
	Android_ShowInputMethodChooser();

	return 0;
}
