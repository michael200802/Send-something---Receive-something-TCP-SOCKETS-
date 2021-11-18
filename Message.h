#pragma once

#include <stddef.h>
#include <stdbool.h>

struct Message
{
    size_t size;
    char * buffer;
};

void ClearMessage(struct Message Message);

void ShowMessageContent(struct Message * Message);

#define MAX_BUFFER_SIZE_FOR_INPUT 3

struct Message GetMessageFromSock(int fd);

struct Message GetMessageFromFile(int fd);

struct Message StrToMessage(const char * restrict str);

////////////////////////////////////////////////////////////////

#include <time.h>

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

void ShowMessageQueueNode(struct MessageQueueNode * MessageQueueNode);

void ShowMessageQueue(struct MessageQueue * MessageQueue);