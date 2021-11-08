#include "Message.h"

#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>

void ClearMessage(struct Message Message)
{
    Message.size = 0;
    free(Message.buffer);
}

void ShowMessageContent(struct Message * Message)
{
    puts("------------");

    puts("Content:");
    if (Message->size == 1)
    {
        puts("\"\"");
    }
    else
    {
        size_t line = 0; 
        printf("%d: \"",line);
        for (size_t i = 0, len = Message->size-1; i < len; i++)
        {
            if(Message->buffer[i] == '\n')
            {
                printf("\"\n%d: \"",++line);
            }
            else
            {
                putchar(Message->buffer[i]);
            }
        }
        puts("\"");
    }
    
    printf("Size: %d\n",Message->size);

    puts("------------");
}

void * Mymemcpy(void * dest, const void * src, size_t n)
{
    while(n)
    {
        *(unsigned char*)dest = *(unsigned char*)src;
        dest = (unsigned char*)dest+1;
        src = (unsigned char*)src+1;
        n--;
    }
    return dest;
}

struct Message GetMessage(int fd)
{

    char buffer[MAX_BUFFER_SIZE_FOR_INPUT+1];
    buffer[MAX_BUFFER_SIZE_FOR_INPUT] = '\0';
    size_t buffer_size;
    struct Message Message = {0,0};
    bool isstdin = fileno(stdin) == fd;

    buffer_size = read(fd,buffer,MAX_BUFFER_SIZE_FOR_INPUT);

    buffer[buffer_size] = '\0';

    Message.size = buffer_size+1;
    Message.buffer = (char*)calloc(Message.size,sizeof(char));
    
    if(buffer_size == 0)
    {
        return Message;
    }

    Mymemcpy(Message.buffer,buffer,Message.size);

    while((isstdin ? buffer[buffer_size-1] != '\n' : true) && buffer_size == MAX_BUFFER_SIZE_FOR_INPUT)
    {
        buffer_size = read(fd,buffer,MAX_BUFFER_SIZE_FOR_INPUT);

        if(buffer_size == 0)
        {
            break;
        }

        buffer[buffer_size] = '\0';

        Message.buffer = (char*)realloc(Message.buffer,Message.size+buffer_size);
        Mymemcpy(Message.buffer + Message.size -1, buffer, buffer_size+1);

        Message.size += buffer_size;
    }

    return Message;
}

