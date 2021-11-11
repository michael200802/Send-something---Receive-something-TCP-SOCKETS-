#pragma oncr

#include <stddef.h>
#include "BOOL.h"

BOOL equalstr(const char * str1, const char * str2);

size_t strlength(const char * restrict str);

char * findstr(char * str, const char * substr);

char * splitstr(char * str, const char * delim);

char * strdup(const char * restrict str);
