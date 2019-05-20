
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <fstream>


#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;
#define PORT 8080
#define N 5

class Servidor
{
    typedef struct{
        int socket;
        pthread_mutex_t* mutex;
    } reqArgs;

    public:
 	    struct sockaddr_in address;
        int port;
        reqArgs new_req;
        int server_fd;
        int opt = 1; 
   	    int addrlen = sizeof(address);
        pthread_t thr;
        pthread_t threads[N];
        
        
        static void functionSystemFile( int socket, pthread_mutex_t* mutex);

        //commands
        static void mkdirCommand(char command[1024], int socket);
        static void touchCommand(char command[1024], int socket);
        static void catCommand(char command[1024] , int socket);
        static void exitCommand(char command[1024], int socket);
        static void cdCommand(char command[1024], int socket);
        static void rmCommand(char command[1024], int socket);
        static void writCommand(char command[1024], int socket);
        static void lsCommand(char command[1024], int socket);
        
    void createSocket();
    void listeningSocket();
    static void sendToClient(char* buffer, int socket);
    static void sendString(const char* string, char* buffer, int socket);
    static void* resolveRequest(void* args);

};




