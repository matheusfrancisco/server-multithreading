
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 

using namespace std;

class Servidor
{
    public:
 	struct sockaddr_in address;
        int port;
        int server_fd, new_socket, valread;
    	int opt = 1; 
   	int addrlen = sizeof(address);  
       


    void createSocket();
    void listeningSocket();
    void sendResponse(FILE *file);

};


