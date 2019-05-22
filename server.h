
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
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
        pthread_t thread;
        int busy;
        sem_t* semaphore;
    } connection_t;

    typedef struct{
        int socket;
        pthread_mutex_t* mutex;
        char thDir[1024];
        connection_t* myThread;
    } reqArgs;

    public:
 	    struct sockaddr_in address;
        int port;
        reqArgs new_req;
        int server_fd;
        int opt = 1; 
   	    int addrlen = sizeof(address);
        connection_t threads[N];
        char stdDir[1024];
        sem_t* thread_controller;
        
        static void functionSystemFile( int socket, pthread_mutex_t* mutex, char dir_atual[1024], connection_t* thread);

        //commands
        static void mkdirCommand(char command[1024], int socket);
        static void touchCommand(char command[1024], int socket);
        static void catCommand(char command[1024] , int socket);
        static void exitCommand(char command[1024], int socket, connection_t* thread);
        static void cdCommand(char command[1024], int socket, char dir[1024]);
        static void rmCommand(char command[1024], int socket);
        static void writCommand(char command[1024], int socket);
        static void lsCommand(char command[1024], int socket, char dir[1024]);
        
    void createSocket();
    void listeningSocket();
    void setThreads();
    static void sendToClient(char* buffer, int socket);
    static void sendString(const char* string, char* buffer, int socket);
    static void* resolveRequest(void* args);

    int getAvailableThread();

};




