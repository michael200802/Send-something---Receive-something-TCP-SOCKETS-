#include "Message.h"

#include <stdlib.h>

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

struct Message StrToMessage(const char * restrict str)
{
    size_t length = 0;
    while(str[length++] != '\0');
    struct Message Message = {length,(char*)calloc(length,sizeof(char))};
    Mymemcpy(Message.buffer,str,length);
    return Message;
}

void InitMessageQueue(struct MessageQueue * Message)
{
    Message->MessageQueueFirstMessage = NULL;
    Message->MessageQueueLastMessage = NULL;
}

bool AddToMessageQueue(struct Message Message, struct MessageQueue * MessageQueue)
{
    //Create and fill
    struct MessageQueueNode *PtrToNewNode = (struct MessageQueueNode*)calloc(1,sizeof(struct MessageQueueNode)); 
    if(PtrToNewNode == NULL)
    {
        return false;
    }
    time_t timer = time(NULL);
    PtrToNewNode->date = *localtime(&timer);
    PtrToNewNode->Message = Message;

    //Put it in
    if(MessageQueue->MessageQueueLastMessage == NULL && MessageQueue->MessageQueueFirstMessage == NULL)
    {
        MessageQueue->MessageQueueFirstMessage = PtrToNewNode;
        MessageQueue->MessageQueueLastMessage = PtrToNewNode;
    }
    else
    {
        MessageQueue->MessageQueueLastMessage->next = PtrToNewNode;
        MessageQueue->MessageQueueLastMessage = PtrToNewNode;
    }

    //Sucessful
    return true;
}

void ClearMessageQueue(struct MessageQueue * MessageQueue)
{
    struct MessageQueueNode *PrevNode = MessageQueue->MessageQueueFirstMessage, *NextNode = PrevNode;
    while(NextNode)
    {
        NextNode = PrevNode->next;
        free(PrevNode->Message.buffer);
        free(PrevNode);
        PrevNode = NextNode;
    }
}

void ShowMessageQueue(struct MessageQueue * MessageQueue)
{
    struct MessageQueueNode * PtrToNode = MessageQueue->MessageQueueFirstMessage;
    while(PtrToNode)
    {
        printf("\nDate: %s",asctime(&PtrToNode->date));
        ShowMessageContent(&PtrToNode->Message);
        PtrToNode = PtrToNode->next;
    }
    putchar(10);
}