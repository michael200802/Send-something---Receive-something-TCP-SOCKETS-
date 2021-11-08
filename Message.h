#pragma once

#include <stddef.h>

struct Message
{
    size_t size;
    char * buffer;
};

void ClearMessage(struct Message Message);

void ShowMessageContent(struct Message * Message);

#define MAX_BUFFER_SIZE_FOR_INPUT 3

struct Message GetMessage(int fd);