#include <stdio.h>

#include "Message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef char BOOL;
enum BOOL_VALUE {FALSE,TRUE};

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

int main()
{ 
    struct Message input, file;
    char * mode_arg, *arg;
    int fd;
    do
    {
        printf(">>> ");
        fflush(stdout);
        input = GetMessage(fileno(stdin));
        for (size_t i = 0, len = input.size-1; i < len; i++)
        {
            input.buffer[i] = (input.buffer[i] >= 'A' && input.buffer[i] <= 'Z' ? input.buffer[i]+32 : input.buffer[i]);
        }
        input.buffer = splitstr(input.buffer," ");
        if(equalstr(input.buffer,"send") == TRUE)
        {
            mode_arg = splitstr(NULL," ");
            if(mode_arg)
            {
                arg = splitstr(NULL," ");
                if(equalstr(mode_arg,"-M") == TRUE)
                {
                    if(arg)
                    {
                        //send the message
                    }
                    else
                    {
                        puts("Error: Message not specified.");
                    }
                }
                else if(equalstr(mode_arg,"-F") == FALSE)
                {
                    if(arg)
                    {
                        fd = open(arg,O_RDONLY);
                        if(fd == -1)
                        {
                            puts("Error: The file wasn't opened.");
                        }
                        else
                        {
                            file = GetMessage(fd);
                            close(fd);
                            //send the file
                            
                        }
                    }
                    else
                    {
                        puts("Error: File not specified.");
                    }
                }
                else
                {
                    puts("Error: Unknown message type.");
                }
            }
            else
            {
                puts("Error: Message type wasn't specified.");
            }
        }
    } while (equalstr(input.buffer,"exit") != TRUE);
    
}