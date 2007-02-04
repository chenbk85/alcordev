#include "Fatal.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// public FatalError function
void FatalError(char *format, ...)
{
	va_list list;
	va_start(list, format);
	vprintf(format, list);
	va_end(list);
	exit(1);
}
