#pragma once

#include <stdlib.h>
#include <stdio.h>

struct Message
{
    size_t size;
    char * buffer;
};

#define ClearMessage(Message)\
    free(Message.buffer);

#define ShowMessageContent(Message)\
    printf("| Content: \"%s\" | Size: %d | Final character: %d |\n",Message.buffer,Message.size,Message.buffer[Message.size-1]);

#define MAX_BUFFER_SIZE_FOR_INPUT 3

struct Message GetMessage(int fd);