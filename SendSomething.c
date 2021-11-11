#include <stdio.h>

#include "Message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <time.h>

#include <stdlib.h>

#include "Mystring.h"

int main()
{
    struct MessageQueue MessageQueue;
    InitMessageQueue(&MessageQueue);
    struct Message input = {0,0}, file;
    char * mode_arg, *arg;
    int fd;
    while(1)
    {
        printf(">>> ");
        fflush(stdout);
        
        if(input.size != 0)
        {
            ClearMessage(input);
        }

        input = GetMessage(fileno(stdin));
        input.size--;
        input.buffer[input.size-1] = '\0';
        input.buffer = splitstr(input.buffer," ");

        if(equalstr(input.buffer,"exit") == TRUE)
        {
            break;
        }

        if(equalstr(input.buffer,"send") == TRUE)
        {
            mode_arg = splitstr(NULL," ");
            if(mode_arg)
            {
                arg = splitstr(NULL," ");
                if(equalstr(mode_arg,"-m") == TRUE)
                {
                    if(arg)
                    {
                        //send the message
                        AddToMessageQueue(StrToMessage(arg),&MessageQueue);
                    }
                    else
                    {
                        puts("Error: Message not specified.");
                    }
                }
                else if(equalstr(mode_arg,"-f") == TRUE)
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
                            AddToMessageQueue(file,&MessageQueue);
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
        else if(equalstr(input.buffer,"show") == TRUE)
        {

            ShowMessageQueue(&MessageQueue);
            
        }
        else if(equalstr(input.buffer,"clear") == TRUE)
        {
            system("clear");
        }
        else
        {
            puts("Error: Invalid command.");
        }
    }

    ClearMessageQueue(&MessageQueue);

    return 0;
}

