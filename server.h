
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>

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
        




    void createSocket();
    void listeningSocket();
    static void functionSystemFile(char  command[1024], int socket){
		printf("testando %s\n",command ); 
        send(socket , command , strlen(command) , 0 ); 

    }
};




