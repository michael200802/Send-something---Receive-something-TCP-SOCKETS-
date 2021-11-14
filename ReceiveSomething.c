#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/mman.h>

#include <pthread.h>
#include <semaphore.h>

#include "Message.h"
#include "Mystring.h"

#define DATABASE "ReceiveSomething.txt"

#define PORT 9000

#define IP_MAX_SIZE 16

pthread_mutex_t stdout_stream_mutex = PTHREAD_MUTEX_INITIALIZER;

struct Message Message;
struct MessageQueue MessageQueue;
int fd_history;

void * handle_connection(void*args)
{
    sem_t * PtrToSem = *(sem_t**)args;
    int client_socket_fd = *(int*)((char*)args+sizeof(sem_t*));
    struct sockaddr_in client_sin = *(struct sockaddr_in*)((char*)args+sizeof(sem_t*)+sizeof(int));

    sem_post(PtrToSem);

    while (1)
    {
        Message = GetMessage(client_socket_fd,true);
        puts("READ MSG");
        if(Message.size == NO_MESSAGE)
        {
            if(Message.buffer == NULL)
            {
                break;
            }
            puts("Wait...");
            sleep(100);
            continue;
        }

        pthread_mutex_lock(&stdout_stream_mutex);
        AddToMessageQueue(Message,&MessageQueue);
        ShowMessageQueueNode(MessageQueue.MessageQueueFirstMessage);
        pthread_mutex_unlock(&stdout_stream_mutex);
    }

    char buffer[IP_MAX_SIZE];
    pthread_mutex_lock(&stdout_stream_mutex);
    printf("Connection with %s closed.\n",inet_ntop(AF_INET,&client_sin.sin_addr,buffer,IP_MAX_SIZE));
    pthread_mutex_unlock(&stdout_stream_mutex);


    pthread_exit(NULL);

    return NULL;
}

int main()
{
    //Load the content of the file to the Queue
    {
        struct stat file_status;
        unsigned char * mapfile;
        size_t offset;
        
        fd_history = open(DATABASE,O_RDWR|O_CREAT,S_IRWXU);
        if (fd_history == -1)
        {
            return -1;
        }
        
        stat(DATABASE,&file_status);

        InitMessageQueue(&MessageQueue);

        if(file_status.st_size != 0)
        {
            mapfile = mmap(NULL,file_status.st_size,PROT_READ,MAP_PRIVATE,fd_history,0); 
            if(mapfile[file_status.st_size-1] != '\0')
            {
                pthread_mutex_lock(&stdout_stream_mutex);
                puts("Error: Corrupted data.");
                return -1;
            }
            else
            {
                offset = 0;
                while(offset < file_status.st_size)
                {
                    Message = StrToMessage((char*)mapfile+offset);
                    AddToMessageQueue(Message,&MessageQueue);
                    offset += Message.size;
                }
            }
            munmap(mapfile,file_status.st_size);
        }

        close(fd_history);
    }

    fd_history = open(DATABASE,O_WRONLY|O_APPEND);

    int server_socket_fd;
    struct sockaddr_in sin_server;

    server_socket_fd = socket(PF_INET,SOCK_STREAM,0);

    sin_server.sin_family = AF_INET;
    sin_server.sin_addr.s_addr = INADDR_ANY;
    sin_server.sin_port = htons(PORT);
    for (unsigned char i = 0; i < 8; i++)
    {
        sin_server.sin_zero[i] = 0;
    }

    if(bind(server_socket_fd,(struct sockaddr*)&sin_server,sizeof(struct sockaddr_in)) == -1)
    {
        pthread_mutex_lock(&stdout_stream_mutex);
        puts("Error: Cannot bound a local socket address to the socket.");
        return -1;
    }

    if(listen(server_socket_fd,SOMAXCONN) == -1)
    {
        pthread_mutex_lock(&stdout_stream_mutex);
        puts("Error: Cannot mark the connection mode as acceptiong connections.");
        return -1;
    }

    char IP[IP_MAX_SIZE];

    sem_t Sem;
    sem_init(&Sem,0,0);
    socklen_t sin_client_length;
    pthread_t handle_connection_thread;

    unsigned char handle_connection_args[sizeof(sem_t*) + sizeof(int) + sizeof(struct sockaddr_in)];
    *(sem_t**)handle_connection_args = &Sem; 
    int * PtrTo_client_socket_fd = (int*)(handle_connection_args+sizeof(sem_t*));
    struct sockaddr_in * PtrTo_sin_client = (struct sockaddr_in*)(PtrTo_client_socket_fd+1);
    while(1)
    {
        if((*PtrTo_client_socket_fd = accept(server_socket_fd,(struct sockaddr*)PtrTo_sin_client,&sin_client_length)) == -1)
        {
            pthread_mutex_lock(&stdout_stream_mutex);
            puts("Error: Cannot accept connection request.");
            pthread_mutex_unlock(&stdout_stream_mutex);
        }
        else
        {
            printf("Connection with %s successfully established.\n",inet_ntop(AF_INET,&PtrTo_sin_client->sin_addr,IP,IP_MAX_SIZE));;
        }
        pthread_create(&handle_connection_thread,NULL,handle_connection,(void*)handle_connection_args);
        sem_wait(&Sem);
    }

    close(fd_history);

    ClearMessageQueue(&MessageQueue);
    return 0;
}