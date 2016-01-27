#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         5000
#define MESSAGE             "This is the message I'm sending back and forth"
#define QUEUE_SIZE          5




// this function will take the file or directory name from the header and return it to us
std::string parse_file_dir_name(char pBuffer[]) {
//pBuffer
}


std::string get_file_dir_contents() {
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html>Hello, it's me!</html>\n";
}


int main(int argc, char* argv[])
{
    int hSocket,hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    char pBuffer[BUFFER_SIZE];
    int nHostPort;
    std::string baseDir;
    // unsigned nReadAmount;

    if(argc < 2)
      {
        printf("\nUsage: server host-port dir\n");
        return 0;
      }
    else
      {
        nHostPort=atoi(argv[1]);
        baseDir = argv[2];
        std::cout << "Base Directory: " << baseDir << std::endl;
      }

    printf("\nStarting server");

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);

    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    printf("\nBinding to port %d\n",nHostPort);

    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address)) 
                        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
 /*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );

        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );


    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return 0;
    }

    for(;;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */
        hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);

        printf("\nGot a connection from %X (%d)\n",
              Address.sin_addr.s_addr,
              ntohs(Address.sin_port));
        // strcpy(pBuffer,MESSAGE);
        // printf("\nSending \"%s\" to client",pBuffer);
        //  number returned by read() and write() is the number of bytes
        // ** read or written, with -1 being that an error occured
        // ** write what we received back to the server 
        // write(hSocket,pBuffer,strlen(pBuffer)+1);
        // /* read from socket into buffer */
        memset(pBuffer,0,sizeof(pBuffer));
        read(hSocket,pBuffer,BUFFER_SIZE);
        printf("Got from browser \n%s\n",pBuffer);
        parse_file_dir_name(pBuffer);


    printf("\nClosing the socket");
        memset(pBuffer,0,sizeof(pBuffer));
        std::string contents = get_file_dir_contents();
        sprintf(pBuffer,contents.c_str());
        write(hSocket,pBuffer, strlen(pBuffer));

        int optval = 1;
        setsockopt (hServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        /* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
         printf("\nCould not close socket\n");
         return 0;
        }
    }
}

