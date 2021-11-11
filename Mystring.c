#include "Mystring.h"

#include <stdlib.h>

BOOL equalstr(const char * str1, const char * str2)
{
    while (*str1 && *str2)
    {
        if(*str1++ != *str2++)
        {
            return FALSE;
        }
    }
    return TRUE;
}

size_t strlength(const char * restrict str)
{
    size_t i = 0;
    while (*str)
    {
        i++;
        str++;
    }
    return i;
}

char * findstr(char * str, const char * substr)
{
    size_t i;
    while (*str)
    {
        for (i = 0; substr[i] != '\0' && str[i] == substr[i]; i++);
        if(substr[i] == '\0')
        {
            return str;
        }
        str++;
    }
    return NULL;
}

char * splitstr(char * str, const char * delim)
{
    static char * internal_str;
    static BOOL first_use = TRUE;
    
    if(first_use == FALSE && str != NULL)
    {
        first_use = TRUE;
    }

    if(first_use)
    {
        if(str == NULL)
        {
            return NULL;
        }
        internal_str = str;
        first_use = FALSE;
    }

    char * substr;

    substr = findstr(internal_str,delim);
    if(NULL == substr)
    {
        first_use = TRUE;
        if(*internal_str)
        {
            return internal_str;
        }
        return NULL;
    }
    *substr = '\0';

    char * returned_str = internal_str;

    do
    {
        internal_str = substr+strlength(delim);
        substr = findstr(internal_str,delim);
    } while (substr == internal_str);
    
    return returned_str;
}

char * strdup(const char * restrict str)
{
    size_t length = strlength(str);
    char * newstr = (char*)calloc(length+1,sizeof(char));
    while(length)
    {
        length--;
        newstr[length] = str[length]; 
    }
    return newstr;
}
