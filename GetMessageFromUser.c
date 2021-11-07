#include "GetMessageFromUser.h"

struct Message GetMessageFromUser()
{
    printf("Message: ");

    char buffer[MAX_BUFFER_SIZE_FOR_INPUT];
    size_t buffer_size;
    
    struct Message Message = {0,0};

    fgets(buffer,MAX_BUFFER_SIZE_FOR_INPUT,stdin);
    buffer_size = strlen(buffer);

    if(buffer[buffer_size-1] == '\n')
    {
        buffer_size--;
        buffer[buffer_size] = '\0';
    }

    Message.size = buffer_size+1;
    Message.buffer = (char*)calloc(Message.size,sizeof(char));
    memcpy(Message.buffer,buffer,Message.size);

    while(buffer[buffer_size-1] != '\n' && buffer_size == MAX_BUFFER_SIZE_FOR_INPUT-1)
    {
        fgets(buffer,MAX_BUFFER_SIZE_FOR_INPUT,stdin);
        buffer_size = strlen(buffer);
        if(buffer[buffer_size-1] == '\n')
        {
            buffer_size--;
            buffer[buffer_size] = '\0';

            if(buffer_size == 0)
            {
                break;
            }
        }

        Message.buffer = (char*)realloc(Message.buffer,Message.size+buffer_size);
        memcpy(Message.buffer + Message.size -1, buffer, buffer_size+1);

        Message.size += buffer_size;

        //ShowMessageContent(Message);
    }

    return Message;

} 