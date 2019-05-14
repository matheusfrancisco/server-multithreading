
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>
#include <boost/algorithm/string.hpp> 
#include <bits/stdc++.h> 

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
        
        
        static void functionSystemFile(char  command[1024], int socket)
        {
            

				int valread = read( socket , command, 1024); 
                //dar um splict no comando
                //mkdir nomedapasta
                if(!strcmp(command,"mkdir")){
                    cout <<"Entrou"<<endl;
                }

                
                send(socket, command, strlen(command), 0);
                memset(command, 0, sizeof(command));


        }

        




    void createSocket();
    void listeningSocket();
    
};




class FileSystemFunction
{
    public:
        int a;

    void mkdirCommand(char mkdir[1024], int socket);
    void lsCommand(char mkdir[1024], int socket);
    void cdCommand(char mkdir[1024], int socket);
    void touchCommand(char mkdir[1024], int socket);
    void rmCommnad(char mkdir [1024], int socket);
    

};