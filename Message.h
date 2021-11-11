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

struct Message StrToMessage(const char * restrict str);

////////////////////////////////////////////////////////////////

#include <time.h>
#include <stdbool.h>

struct MessageQueueNode
{
    struct tm date;
    struct Message Message;
    struct MessageQueueNode * next;
};

struct MessageQueue
{
    struct MessageQueueNode * MessageQueueFirstMessage, * MessageQueueLastMessage;
};

void InitMessageQueue(struct MessageQueue * Message);

bool AddToMessageQueue(struct Message Message, struct MessageQueue * MessageQueue);

void ClearMessageQueue(struct MessageQueue * MessageQueue);

void ShowMessageQueue(struct MessageQueue * MessageQueue);