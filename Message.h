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

#define NO_MESSAGE 0

struct Message GetMessage(int fd, bool is_socket);

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