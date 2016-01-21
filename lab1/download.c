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
#include <sstream>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         500
#define HOST_NAME_SIZE      255
#define MAXGET 1000

// this function opens and closes the socket and send an http request to the server
bool download(char strHostName[],int nHostPort,int c,int times_to_download,int err,std::string page,vector<char*> header_lines, bool debug, bool show_html) {

	int hSocket;                 /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;


	if(debug == true) {
    	printf("\nMaking a socket\n");
    }

    if(debug == true) {
      	std::cout << "\nDebugging\n" << std::endl;
    }

    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return false;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    if(pHostInfo == 0) {
    	printf("\nThe host name could not be resolved\n");
    	return false;
    }

    // std::cout << pHostInfo << std::endl;

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
        printf("\nCould not connect to host through port given.\n");
        return false;
    }


    // Creat HTTP Message
    char *message = (char*)malloc(MAXGET);
    sprintf (message, "GET %s HTTP/1.1\r\nHost:%s:%u\r\n\r\n",page.c_str(),strHostName,nHostPort);
    // Send HTTP on the socket


    if(debug == true) {
    	printf("Request: %s\n", message);
    }
	
	write(hSocket,message,strlen(message));


    GetHeaderLines(header_lines,hSocket,false);


    if(debug == true) {
    	for(int i = 0; i < header_lines.size(); i++) {
	    	std::cout << header_lines[i] << std::endl;
	    }
    }
    


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
    // std::cout << "\n\n" << std::endl;
    while(total_bytes < content_length) {
    	// Read Respons back from socket
    	nReadAmount=read(hSocket,pBuffer,1);
    	if(nReadAmount == 0) return false; // code to tell if the number of bytes was zero after reading in the bytes
    	total_bytes += nReadAmount;

    	if(show_html == true) {
    		std::cout << pBuffer[0];
    	}
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


    if(debug == true) {

    	printf("\nClosing socket\n");
    }
    /* close socket */                       
    if(close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return true;
    }
    free(message);
    return true;
}



int  main(int argc, char* argv[])
{

    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    bool debug = false;
    int c, times_to_download = 1, err = 0;
    std::string page;
    vector<char*> header_lines;
    int successful_downloads = 0;
    bool show_html = true;




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
                    std::cout << "Attempting to Download " << optarg << " times" << std::endl;
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


    if(times_to_download > 1) {
    	show_html = false;
    	for(int i = 0; i < times_to_download; i++) {
    		bool was_successful = download( strHostName, nHostPort,c,times_to_download,err, page,header_lines,debug,show_html);
    		// std::string result_message = was_successful?"True":"False";
    		// std::cout << result_message << std::endl;
    		successful_downloads++;
    	}
    	stringstream ss;
    	ss << successful_downloads;
    	std::string success_result = ss.str();
    	std::cout << "Number of successful_downloads: " << success_result << std::endl;
    }
    else {
    	show_html = true;
    	download( strHostName, nHostPort,c,times_to_download,err, page,header_lines,debug,show_html);
    }

	return 0;
}// end of main