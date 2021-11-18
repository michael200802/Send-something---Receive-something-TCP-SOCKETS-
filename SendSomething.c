#include <stdio.h>

#include "Message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <time.h>

#include <stdlib.h>

#include "Mystring.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_MAX_SIZE 16
#define PORT 9000

int main()
{

    //Variables for the socket
    char IP[IP_MAX_SIZE];
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    ssize_t send_return;
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    for(unsigned char i = 0; i < 8; i++)
    {
        sin.sin_zero[i] = 0;
    }
    BOOL socket_ready = FALSE;

    //Variables for the CLI
    struct MessageQueue MessageQueue;
    InitMessageQueue(&MessageQueue);
    struct Message input = {0,0}, msg;
    char * mode_arg, *arg;
    int fd;

    //Loop, obviously
    while(1)
    {
        printf(">>> ");
        fflush(stdout);
        
        if(input.size != 0)
        {
            ClearMessage(input);
        }

        input = GetMessageFromFile(fileno(stdin));
        input.size--;
        input.buffer[input.size-1] = '\0';
        input.buffer = splitstr(input.buffer," ");

        if(equalstr(input.buffer,"exit") == TRUE)
        {
            break;
        }

        if(equalstr(input.buffer,"send") == TRUE)
        {
            if(socket_ready == TRUE)
            {
                mode_arg = splitstr(NULL," ");
                if(mode_arg)
                {
                    arg = splitstr(NULL," ");
                    if(equalstr(mode_arg,"-m") == TRUE)
                    {
                        if(arg)
                        {
                            msg = StrToMessage(arg);
                            send_return = send(socket_fd,msg.buffer,msg.size,0);
                            if(send_return == msg.size)
                            {
                                AddToMessageQueue(msg,&MessageQueue);
                                continue;
                            }
                            else if(send_return == -1)
                            {
                                puts("Error: Message wasn't sent.");
                                socket_ready = FALSE;
                            }
                            else
                            {
                                printf("Error: The hole message wasn't sent. %d/%d bytes were sent successfully.\n",send_return,msg.size);
                            }
                            ClearMessage(msg);
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
                                msg = GetMessageFromFile(fd);
                                close(fd);
                                send_return = send(socket_fd,msg.buffer,msg.size,0);
                                if(send_return == msg.size)
                                {
                                    AddToMessageQueue(msg,&MessageQueue);
                                    continue;
                                }
                                else if(send_return == -1)
                                {
                                    puts("Error: The file wasn't sent.");
                                    socket_ready = FALSE;
                                }
                                else
                                {
                                    printf("Error: The hole file wasn't sent. %d/%d bytes were sent successfully.\n",send_return,msg.size);
                                }
                                ClearMessage(msg);
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
            else
            {
                puts("Error: You haven't set the socket info.");
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
        else if(equalstr(input.buffer,"set ip") == TRUE)
        {
            printf("IP:");
            fgets(IP,IP_MAX_SIZE-1,stdin);
            {
                size_t IPlen = strlength(IP);
                if(IP[IPlen-1] == '\n')
                {
                    IP[IPlen-1] = '\0';
                }
            }
            if (inet_pton(AF_INET,IP,&sin.sin_addr) <= 0)
            {
                printf("\"%s\"\n",IP);
                puts("Error: Invalid ip.");
            }
            else
            {
                if(connect(socket_fd,(struct sockaddr*)&sin,sizeof(sin)) == -1)
                {
                    puts("Error: The connection wasn't established.");
                }
                else
                {
                    printf("Connection with %s established successfully.\n",inet_ntop(AF_INET,&sin.sin_addr,IP,INET_ADDRSTRLEN));
                    socket_ready = TRUE;
                }
            }
        }
        else
        {
            puts("Error: Invalid command.");
        }
    }

    ClearMessageQueue(&MessageQueue);

    close(socket_fd);

    return 0;
}

