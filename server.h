
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

class Servidor
{
    public:
 	    struct sockaddr_in address;
        int port;
        int new_socket;
        int server_fd;
        int opt = 1; 
   	    int addrlen = sizeof(address);  
        
        
        static void functionSystemFile( int socket);

        //commands
        static void mkdirCommand(char command[1024], int socket);
        static void touchCommand(char command[1024], int socket);
        static void catCommand(char command[1024] , int socket);

    void createSocket();
    void listeningSocket();
    void lsCommand();
    void cdCommand();
    void rmCommnad();
    static void sendToClient(char* buffer, int socket);
    static void sendString(const char* string, char* buffer, int socket);

};




