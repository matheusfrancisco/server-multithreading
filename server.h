
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <pthread.h>


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
        
        
        static void functionSystemFile( int socket)
        {
            
                char  command[1024];

				int valread = read( socket , command, 1024); 
                //dar um splict no comando
                //mkdir nomedapasta
                /*
                char mkdir[] = "mkdir";
                cout<<command<<endl;
                printf("%i",strcmp(command, mkdir));
                if(strcmp(command, "mkdir")>0)
                {
                    cout <<"Entrou"<<endl;
                }*/

                if(strncmp(command, "mkdir", 5) == 0){
    
                    mkdirCommand(command, socket);

                }else if(strncmp(command, "touch2", 5) == 0){
                    
                    cout << "touch" <<endl;
                
                }else if(strncmp(command, "cat ", 4) == 0){
                    
                    catCommand(command, socket);

                }else if(strncmp(command, "ls", 5) == 0){
                    //ls
                    cout << "Estamos aqui" <<endl;

                    char ls[1024];
                    char cp[1024];

                    DIR * currentPath = NULL
                    struct dirent *dir =NULL;
                    currentPath =  opendir(".");
                    memset(ls,0, sizeof(ls));
                    strcat(ls,"Path >");
                    strcat(ls,getcwd(cp,sizeof(cp)));
                    strcat(ls,"\n\t");
                    
                    while(dir =readdir(currentPath)){
                        strcat(ls, dir->d_name);
                        strcat(ls, "\t\n\t");
                    }
                    rewinddir(currentPath);	
		            send(socket,ls,strlen(ls),0);

                }else{
                    memset(command, 0, sizeof(command));
                    snprintf(command, sizeof(command),"Error 404.\n");
                    send(socket,command,strlen(command), 0);
                    memset(command, 0, sizeof(command));
                }
                

        
                

        }

        

        static void mkdirCommand(char  command[1024], int socket)   {
                
                
                if(strncmp(command, "mkdir", 5) == 0){

                    if(system(command) == 0)
                    {
                        memset(command, 0, sizeof(command));

			            strcpy(command,  "Successfully created folder \n");
                        send(socket,command,strlen(command), 0);
                        memset(command, 0, sizeof(command));

                    }else{
                        memset(command, 0, sizeof(command));
                        strcpy(command,"Error 404.\n");
                        send(socket,command,strlen(command), 0);

                        memset(command, 0, sizeof(command));
                    }
                }else{
                    memset(command, 0, sizeof(command));
                    strcpy(command,"Error 404.\n");
                    send(socket,command,strlen(command), 0);
                    memset(command, 0, sizeof(command));
                    
                }


        
        }

        static void touchCommand(char  command[1024], int socket)   {
                
                
                if(strncmp(command, "touch", 5) == 0){

                    if(system(command) == 0)
                    {
                        memset(command, 0, sizeof(command));

			            strcpy(command,  "Successufly created file \n");
                        send(socket,command,strlen(command), 0);
                        memset(command, 0, sizeof(command));

                    }else{
                        memset(command, 0, sizeof(command));
                        strcpy(command,"Error 404.\n");
                        send(socket,command,strlen(command), 0);

                        memset(command, 0, sizeof(command));
                    }
                }else{
                    memset(command, 0, sizeof(command));
                    strcpy(command,"Error 404.\n");
                    send(socket,command,strlen(command), 0);

                    memset(command, 0, sizeof(command));
                    
                }


        
        }

        static void catCommand(char command[1024] , int socket)
        {
             if(strncmp(command, "cat ", 4) == 0){

                FILE *file;
                char sendMSG[1024];

                memmove(command, command + 4, strlen(command));

                cout<< "text.txt" <<endl;
                cout<< command<<endl;

                file = fopen(command, "r");

                if(file == NULL)
                {
                    memset(sendMSG, 0, sizeof(sendMSG));
                    strcpy(sendMSG,"Error 404.\n");
                    send(socket,sendMSG,strlen(sendMSG), 0);

                }else{

                    memset(sendMSG, 0, sizeof(sendMSG));
                    fread(sendMSG, sizeof(char), 1024, file);
                    send(socket, sendMSG, strlen(sendMSG), 0);

                }
                fclose(file);

            }
                

        }


    void createSocket();
    void listeningSocket();
    void lsCommand();
    void cdCommand();
    void rmCommnad();

};




