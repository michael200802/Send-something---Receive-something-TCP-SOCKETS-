#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "Message.h"

int main(int argc, char * argv[])
{
    int fd;

    if(argc == 1)
    {
        puts("Type your input: ");
        fd = fileno(stdin);
    }
    else
    {
        fd = open(argv[1],O_RDONLY);
    }

    if(fd == -1)
    {
        puts("No se ha podido conseguir el descriptor de archivo correspondiente.");
        exit(EXIT_FAILURE);
    }

    struct Message Message = GetMessage(fd);

    ShowMessageContent(&Message);

    ClearMessage(Message);

    return 0;
}