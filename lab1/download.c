#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "sample.cpp"

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         500
#define HOST_NAME_SIZE      255
#define MAXGET 1000

int  main(int argc, char* argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    bool debug = false;
    int c, times_to_download = 1, err = 0;
    std::string page;
    vector<char*> header_lines;




    if(argc > 6)
      {
        printf("\nUsage: download host-name host-port path -c or -d\n");
        return 0;
      }
    else
      {
        
        while((c = getopt(argc,argv, "c:d") ) != -1)
        {

            switch(c) 
            {
                case 'c':
                    std::cout << optarg << std::endl;
                    times_to_download = atoi( optarg );
                    break;
                case 'd':
                    debug = true;
                    break;
                case '?':
                    err = 1;
                    break;
            }
        }

        strcpy(strHostName,argv[optind]);
        nHostPort = atoi(argv[optind + 1]);
        page = argv[ optind + 2];
      }


















    printf("\nMaking a socket\n");
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);

    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

    // printf("\nConnecting to %s (%X) on port %d",strHostName,nHostAddress,nHostPort);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }


    // Creat HTTP Message
    char *message = (char*)malloc(MAXGET);
    sprintf (message, "GET %s HTTP/1.1\r\nHost:%s:%u\r\n\r\n",page.c_str(),strHostName,nHostPort);
    // Send HTTP on the socket
    //printf("Request: %s\n", message);
    write(hSocket,message,strlen(message));


    GetHeaderLines(header_lines,hSocket,false);

    // for(int i = 0; i < header_lines.size(); i++) {
    // 	std::cout << header_lines[i] << std::endl;
    // }


    char* html_start;
    int content_length = 0;

    for(int i = 0; i < header_lines.size(); i++) {
    	if(strstr(header_lines[i],"Content-Length") != NULL) {
    		html_start = strstr(header_lines[i],"Content-Length");
    		std::string test = string(html_start);
    		test.erase(0,16);
    		//std::cout << test << std::endl;
    		
    		content_length = atoi(test.c_str());
    			
    		
    		//std::cout << content_length << std::endl;
    	}
    	
    }


    int total_bytes = 0;
    while(total_bytes < content_length) {
    	// Read Respons back from socket
    	nReadAmount=read(hSocket,pBuffer,1);
    	total_bytes += nReadAmount;
    	std::cout << pBuffer[0];
    }





    
    //printf("%u\n",nReadAmount);
    //printf("Response %s", pBuffer);


    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    //printf("\nReceived \"%s\" from server\n",pBuffer);
    /* write what we received back to the server */
    //write(hSocket,pBuffer,nReadAmount);
    //printf("\nWriting \"%s\" to server",pBuffer);

    printf("\nClosing socket\n");
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
    free(message);


}// end of main




void download() {

}
