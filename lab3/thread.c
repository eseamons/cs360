#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <sys/signal.h>

// includes needed for threading lab
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
#include <queue>
#include <semaphore.h>
#include <pthread.h>



#define THREAD_NUM          10

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////



#define SOCKET_ERROR        -1
#define BUFFER_SIZE         10000
#define MESSAGE             "This is the message I'm sending back and forth"
#define QUEUE_SIZE          5

sem_t empty, full, mutex;
std::string baseDir;

int nHostPort;


class myqueue {
    std::queue <int> stlqueue;
    public:
    void push (int sock) {
        sem_wait(&empty);
        sem_wait(&mutex);
        stlqueue.push(sock);
        sem_post(&mutex);
        sem_post(&full);
    }
    int pop () {
        sem_wait(&full);
        sem_wait(&mutex);
        int rval = stlqueue.front();
        stlqueue.pop();
        sem_post(&mutex);
        sem_post(&empty);
        return (rval);
    }

} sockqueue;


std::string get_last_char(std::string file_path) {
    if(file_path.length() != 0)
        return file_path.substr(file_path.length() - 1);
    else
        return "";
}

// this function will take the file or directory name from the header and return it to us
std::string parse_file_dir_name(char pBuffer[]) {//vector<char*> header_lines

    std::stringstream ss;
    ss << pBuffer;
    std::string temp;
    ss >> temp; // Get
    ss >> temp; // filepath
    std::cout << temp << std::endl;


    return temp;
}

std::string remove_leading_slash(std::string file_path) {
    if(file_path.length() != 0) {
        file_path = file_path.substr(1,file_path.length());
        std::cout << file_path<< std::endl;
    }
        
        return file_path;
}

std::string get_file_type(std::string file_path) {

    // Finds the last persiod character of the string
    int period = file_path.find_last_of(".");
    // I use  + 1 because I don't really need the to include the period
    std::string ext = file_path.substr(period + 1);
    if(file_path != "") {
        ext = file_path.substr(period + 1);
    }
    else {
        ext = "";
    }

    std::cout << "\n" << ext << "\n";

    return ext;
}

std::string get_content_type(std::string content_type) {
    if(content_type == "html")
        return "Content-Type: text/html";
    else if(content_type == "txt")
        return "Content-Type: text/plain";
    else if(content_type == "jpg")
        return "Content-Type: image/jpg";
    else if(content_type == "gif")
        return "Content-Type: image/gif";
    else
        return "Content-Type: text/plain";
}



void* serve(void* arg) {


char pBuffer[BUFFER_SIZE];

    for(;;) { // end of infinite loop


        // strcpy(pBuffer,MESSAGE);
        // printf("\nSending \"%s\" to client",pBuffer);
        //  number returned by read() and write() is the number of bytes
        // ** read or written, with -1 being that an error occured
        // ** write what we received back to the server 
        // write(hSocket,pBuffer,strlen(pBuffer)+1);
        // /* read from socket into buffer */
        int hSocket = sockqueue.pop();
        memset(pBuffer,0,sizeof(pBuffer));
        read(hSocket,pBuffer,BUFFER_SIZE);
        printf("Got from browser \n%s\n",pBuffer);

        std::string file_path = parse_file_dir_name(pBuffer);
        file_path = remove_leading_slash(file_path);


        std::string path_for_directory_listing = file_path;

        if(get_last_char(baseDir) == "/") {
            file_path = baseDir + file_path;
        }
        else {
            file_path = baseDir + "/" + file_path;
        }
        
        struct stat filestat;

        bool error = false;
        std::string file_type;
        if(stat(file_path.c_str(), &filestat)) {
            std::cout <<"ERROR in stat\n";
            error = true;
        }

        std::cout << "file size = "<<filestat.st_size <<"\n";




        printf("\nClosing the socket");
        memset(pBuffer,0,sizeof(pBuffer));


        if(!error) {
                    


            if(S_ISREG(filestat.st_mode)) {


                file_type = get_file_type(file_path);
                std::string content_type = get_content_type(file_type);

                std::string response_header = "HTTP/1.1 200 OK\r\n"+content_type+"\r\n\r\n";
                sprintf(pBuffer,response_header.c_str());
                write(hSocket,pBuffer, strlen(pBuffer));




                std::cout << file_path << " is a regular file \n";
                FILE *fp = fopen(file_path.c_str(),"rb");
                char *buff = (char *)malloc(filestat.st_size+1);
                fread(buff, filestat.st_size, 1, fp);
                write(hSocket,buff, filestat.st_size);
                printf("Got\n%s\n",buff);
                free(buff);
                fclose(fp);
            }
            if(S_ISDIR(filestat.st_mode)) {



                struct stat index_stat;

                std::string directory_index_html_path = file_path + "/index.html";
                if(stat(directory_index_html_path.c_str(), &index_stat)) {// there is no index.html for the directory
                    std::cout <<"\nThere is not a index.html file in this directory\n";

                    file_type = get_file_type(file_path);
                    std::string content_type = get_content_type(file_type);

                    std::cout << file_path << " is a directory \n";
                      DIR *dirp;
                      struct dirent *dp;

                      dirp = opendir(file_path.c_str());
                      std::string html_file_listing = "<!DOCTYPE HTML><html><head><title>Directory Listing</title></head><body><h1>Directory listing for "+file_path+"</h1>";
                      while ((dp = readdir(dirp)) != NULL) {
                            // printf("name %s\n",dp->d_name);


                            if(get_last_char(path_for_directory_listing) == "/" || path_for_directory_listing == "") {
                                html_file_listing += "<p><a href=\"/"+path_for_directory_listing+dp->d_name+"\">";
                            }
                            else {
                                html_file_listing += "<p><a href=\"/"+path_for_directory_listing+"/"+dp->d_name+"\">";
                            }

                            
                            html_file_listing.append(dp->d_name);
                            html_file_listing += "</a></p>";
                      }
                      (void)closedir(dirp);
                      html_file_listing += "</body></html>";

                    std::string response_header = "HTTP/1.1 200 OK\r\nContent Type: text/html\r\n\r\n" + html_file_listing;
                    sprintf(pBuffer,response_header.c_str());
                    write(hSocket,pBuffer, strlen(pBuffer));
                }
                else {
                    std::string response_header = "HTTP/1.1 200 OK\r\nContent Type: text/html\r\n\r\n";
                    sprintf(pBuffer,response_header.c_str());
                    write(hSocket,pBuffer, strlen(pBuffer));

                    std::cout <<"\nHere are the contents of index.html\n";
                    FILE *fp = fopen(directory_index_html_path.c_str(),"rb");
                    char *buff = (char *)malloc(index_stat.st_size+1);
                    fread(buff, index_stat.st_size, 1, fp);
                    write(hSocket,buff, index_stat.st_size);
                    printf("Got\n%s\n",buff);
                    free(buff);
                    fclose(fp);
                }

            }// end of directory if statement

        }
        else {
            std::stringstream length;
            length << filestat.st_size;
            std::string response_header = "HTTP/1.1 404 NOT FOUND\r\nContent Type: text/html\r\nContent Length: "+length.str()+"\r\n\r\n<!DOCTYPE HTML><html><head><title>404 NOT FOUND</title></head><body><h1 style=\"text-align:center;font-size:40px;\">404 NOT FOUND</h1></body></html>";
            sprintf(pBuffer,response_header.c_str());
            write(hSocket,pBuffer, strlen(pBuffer));

        }


  //       linger lin;
		// unsigned int y=sizeof(lin);
		// lin.l_onoff=1;
		// lin.l_linger=10;
		// setsockopt(hSocket,SOL_SOCKET, SO_LINGER,&lin,sizeof(lin));

        /* close socket */
        if(close(hSocket) == SOCKET_ERROR)
        {
         printf("\nCould not close socket\n");
         return 0;
        }


    }// end of infinite loop
}





// void handler (int status);   /* definition of signal handler */

// void handler (int status)
// {
//     printf("received signal %d\n",status);
// }

int main(int argc, char* argv[])
{

    // code added for lab 3
    long threadid;
    int num_threads;
    int queue_size = 20;
    int hSocket;  /* handle to socket */
    int hServerSocket;
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);
    sem_init(&full, PTHREAD_PROCESS_PRIVATE, 0);
    sem_init(&empty, PTHREAD_PROCESS_PRIVATE, queue_size);

    ///////////////////////
    ///////////////////////
    ///////////////////////

    // First set up the signal handler
    // struct sigaction sigold, signew;

    // signew.sa_handler=handler;
    // sigemptyset(&signew.sa_mask);
    // sigaddset(&signew.sa_mask,SIGINT);
    // signew.sa_flags = SA_RESTART;
    // sigaction(SIGINT,&signew,&sigold);
    // sigaction(SIGHUP,&signew,&sigold);
    // sigaction(SIGPIPE,&signew,&sigold);

    if(argc < 2)
    {
        printf("\nUsage: server <port number> <num threads> <dir>\n");
        return 0;
    }
    else
    {
        nHostPort=atoi(argv[1]);
        num_threads=atoi(argv[2]); // added for lab 3
        baseDir = argv[3];
        std::cout << "Base Directory: " << baseDir << std::endl;
    }


    ///////////////////////////////////////////////

    std::cout << "threads hello!" << std::endl;
    pthread_t threads[ num_threads];

    for(threadid = 0; threadid < num_threads; threadid++) {
      pthread_create(&threads[threadid], NULL, serve, (void *)threadid);
   }
    /////////////////////////////////////////////////////







    printf("\n%d\n",num_threads);

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

    int optval = 1;
    setsockopt (hServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    for(;;)
    {
        printf("\nWaiting for a connection\n");
        /* get the connected socket */


        hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);
        sockqueue.push(hSocket);

    }





}// end of main function

