#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <math.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define HOST_NAME_SIZE      255
#define NSOCKETS 10
#define NSTD 3
#define MAXGET 1000



double calculate_average(std::vector<double> times,int count) {
	double average = 0;

	for(int i = 0; i < count; i++) {
		average += times[i];
	}
	average = average/count;

	return average;
}

double calculate_standard_deviation(std::vector<double> times,double average,int count) {
	double sum_of_diffs = 0;

	for(int i = 0; i < count; i++) {
		double difference = 0;
		difference = times[i] - average;
		sum_of_diffs += difference * difference;

	}

	return sqrt(sum_of_diffs/count);
}


int  main(int argc, char* argv[])
{
    
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    std::string path;
    double average = 0;
    std::vector<double> times;

    bool debug = false;
    int c, count = 0, err = 0;


    printf("%s\n",argv[4]);
    if(argc < 3)
      {
        printf("\nUsage: client host-name host-port\n");
        return 0;
      }
    else
    {

  		while((c = getopt(argc,argv, "d") ) != -1)
        {

            switch(c) 
            {
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
        path = argv[ optind + 2];
        count = atoi(argv[ optind + 3]);

      }



   int hSocket[count];                 /* handle to socket */
   struct timeval oldtime[count+NSTD];

   printf("Hostname: %s Path: %s Count: %d Debug: %d",strHostName,path.c_str(),count,debug);

    printf("\n\nMaking %d sockets\n",count);
    /* make a socket */
	for(int i = 0; i < count; i++) {
	    hSocket[i]=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

		if(hSocket[i] == SOCKET_ERROR)
		{
			printf("\nCould not make a socket\n");
			return 0;
		}
	}

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(nHostPort);
    Address.sin_family=AF_INET;

	int epollFD = epoll_create(1);
	// Send the requests and set up the epoll data
	for(int i = 0; i < count; i++) {
		/* connect to host */
		if(connect(hSocket[i],(struct sockaddr*)&Address,sizeof(Address)) 
		   == SOCKET_ERROR)
		{
			printf("\nCould not connect to host\n");
			return 0;
		}


		char *request = (char*)malloc(MAXGET);
		sprintf (request, "GET %s HTTP/1.1\r\nHost:%s:%u\r\n\r\n",path.c_str(),strHostName,nHostPort);
		//char request[] = "GET /test.html HTTP/1.0\r\n\r\n";

	    	write(hSocket[i],request,strlen(request));
		// Keep track of the time when we sent the request
		gettimeofday(&oldtime[hSocket[i]],NULL);
		// Tell epoll that we want to know when this socket has data
		struct epoll_event event;
		event.data.fd = hSocket[i];
		event.events = EPOLLIN;
		int ret = epoll_ctl(epollFD,EPOLL_CTL_ADD,hSocket[i],&event);
		if(ret)
			perror ("epoll_ctl");
	}
	for(int i = 0; i < count; i++) {
		struct epoll_event event;
		int rval = epoll_wait(epollFD,&event,1,-1);
		if(rval < 0)
			perror("epoll_wait");
		rval = read(event.data.fd,pBuffer,BUFFER_SIZE);
		struct timeval newtime;
		// Get the current time and subtract the starting time for this request.
		gettimeofday(&newtime,NULL);
		double usec = (newtime.tv_sec - oldtime[event.data.fd].tv_sec)*(double)1000000+(newtime.tv_usec-oldtime[event.data.fd].tv_usec);

		
			times.push_back(usec/1000000);
		if(debug == true) {
			std::cout << "\nTime "<<usec/1000000<<std::endl;
			printf("got %d from %d\n",rval,event.data.fd);
		}
		// Take this one out of epoll
		epoll_ctl(epollFD,EPOLL_CTL_DEL,event.data.fd,&event);

		// if(debug == true)
		// 	printf("\nClosing socket\n");
		// /* close socket */                       
		// if(close(hSocket[i]) == SOCKET_ERROR)
		// {
		// 	printf("\nCould not close socket\n");
		// 	return 0;
		// }

	}

	average = calculate_average(times,count);
	double standard_dev = calculate_standard_deviation(times,average,count);

	printf("\n\nAverage Time: %f",average);
	printf("\nStandard Deviation: %f\n\n",standard_dev);



	// Now close the sockets
	for(int i = 0; i < count; i++) {
		// if(debug == true)
		// 	printf("\nClosing socket\n");
		/* close socket */                       
		if(close(hSocket[i]) == SOCKET_ERROR)
		{
			printf("\nCould not close socket\n");
			return 0;
		}
	}
}
