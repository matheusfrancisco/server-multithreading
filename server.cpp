#include "server.h"



void* Servidor::resolveRequest(void* args){
    char buffer[1024];
    int socket = *((int*) args);

    if (socket < 0){ 
        perror("accept"); 
        pthread_exit(NULL); 
    } 

    strcpy(buffer, "Connection successfull!\n");
    sendToClient(buffer, socket);

    strcpy(buffer, "Current directory:\n");
    sendToClient(buffer, socket);

    getcwd(buffer, sizeof(buffer));
    sendToClient(buffer, socket);

    strcpy(buffer, "\n");
    sendToClient(buffer, socket);

    while(1){
        sendString(">> ", buffer, socket);
        functionSystemFile(socket);
    }

    pthread_exit(NULL);
}

void Servidor::createSocket(){

    int addrlen = sizeof(address);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0))==0){
        perror("socket failed");
        exit(EXIT_FAILURE);

    }

    //Forcefully attaching socket to the port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
      
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

}

void Servidor::listeningSocket(){

	while(true){

        if (listen(server_fd, 3) < 0) { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        }

        for(int i = 0; i < N; i++){
            new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen);
            pthread_create(&threads[i], NULL, resolveRequest, (void*) &new_socket);
        }

        for(int i = 0; i < N; i++){
            pthread_join(threads[i], NULL);
        }
	 }
}

void Servidor::functionSystemFile( int socket){
    
    char  command[1024];

    int valread = read( socket , command, 1024);

    if(strncmp(command, "mkdir", 5) == 0){
        mkdirCommand(command, socket);
        memset(command, 0, sizeof(command));
    }
    else if(strncmp(command, "touch", 5) ==0){
        touchCommand(command, socket);
        memset(command, 0, sizeof(command));
    }
    else if(strncmp(command, "cat ", 4) == 0) 
        catCommand(command, socket);
    else if(strncmp(command, "ls", 2) == 0){

        char ls[1024];
        char cp[1024];

        DIR * dir_p = NULL;
        struct dirent *dir =NULL;
        dir_p =  opendir(".");
        memset(ls,0, sizeof(ls));
        strcat(ls,"Path >");
        strcat(ls,getcwd(cp,sizeof(cp)));
        strcat(ls,"\n\t");
        
        while(dir =readdir(dir_p)){
            strcat(ls, dir->d_name);
            strcat(ls, "\t\n\t");
        }
        strcat(ls, "\n");
        rewinddir(dir_p);

        send(socket,ls,strlen(ls),0);

    }
    else if(strncmp(command, "exit", 4) == 0)
        exitCommand(command, socket);
    else if(strncmp(command, "cd ",3) == 0){
        cout<<"Aqui"<<endl;

        cdCommand(command, socket);
        memset(command, 0, sizeof(command));

    }else if(strncmp(command, "rm -r",5) ==0){
        rmCommand(command, socket);
        memset(command, 0, sizeof(command));

    }else if(strncmp(command, "echo", 4)==0){
        writCommand(command,socket);
        memset(command, 0, sizeof(command));

    }else{
        sendString("Error 404.\n", command, socket);
    
    }
}

void Servidor::mkdirCommand(char command[1024], int socket){             
    if(strncmp(command, "mkdir", 5) == 0){
        if(system(command) == 0)
            sendString("Successufly created folder \n", command, socket);
        else
            sendString("Error 404.\n", command, socket);
    }
    else
        sendString("Error 404.\n", command, socket);
}

void Servidor::touchCommand(char command[1024], int socket){
    if(strncmp(command, "touch", 5) == 0){
        if(system(command) == 0)
            sendString("Successufly created file \n", command, socket);
        else
            sendString("Error 404.\n", command, socket);
    }
    else
        sendString("Error 404.\n", command, socket);
}

void Servidor::catCommand(char command[1024], int socket){
    
    // REMOVENDO ENTER DO FINAL
    if(command[strlen(command)-1] == 10)
        command[strlen(command)-1] = 0x00;
    if(strncmp(command, "cat ", 4) == 0){

        FILE * file;
        char sendMSG[1024];
        char path[1024];
        char test[1024];

        getcwd(path, sizeof(path));
        
        memmove(command, command + 4, strlen(command));

        cout<< "text.txt" <<endl;
        cout<< command<<endl;
        strcat(path,"/");
        strcat(path, command);
        cout<<"Aqui"<<endl;
        cout<<path<<endl;

        file = fopen(command, "r");
        
        if(file == NULL){
            sendString("Error 404.\n", sendMSG, socket);
            memset(command, 0, sizeof(command));
        }
        else{
            memset(sendMSG, 0, sizeof(sendMSG));
            fread(sendMSG, sizeof(char), 1024, file);
            sendToClient(sendMSG, socket);
            memset(sendMSG, 0, sizeof(sendMSG));

        }
        fclose(file);
    }
}

void Servidor::cdCommand(char command[1024], int socket){
    if(command[strlen(command)-1] == 10)
        command[strlen(command)-1] = 0x00;

    if(strncmp(command, "cd ", 3) == 0){
            char dir[1024];
            memset(dir, 0, sizeof(dir));

            cout<<"Aqui"<<endl;
            memmove(command, command + 3, strlen(command));
            cout <<command<<endl;
            chdir(command);

            memset(command, 0, sizeof(command));
            strcpy(command, "New directory:\n");
            sendToClient(command, socket);
            
            getcwd(dir, sizeof(dir));
            
            cout<<dir<<endl;
            sendToClient(dir, socket);

            strcpy(command, "\n");
            send(socket, command, strlen(command), 0);
            memset(command, 0, sizeof(command));
            //sendString("Você entro\n", buffer, socket);
        
    }
    else{
        sendString("Error 404.\n", command, socket);
        memset(command, 0, sizeof(command));
    }
}
void Servidor::rmCommand(char command[1024], int socket){
        if(command[strlen(command)-1] == 10)
            command[strlen(command)-1] = 0x00;

        if(strncmp(command, "rm -r", 5) == 0){
            if(system(command)==0){
                sendString("Succesfully remove file. \n", command, socket);
                memset(command, 0, sizeof(command));

            }
        }
}

void Servidor::writCommand(char command[1024], int socket){
        if(command[strlen(command)-1] == 10)
            command[strlen(command)-1] = 0x00;

        if(strncmp(command, "echo ", 5) == 0){
            if(system(command)==0){
                memset(command, 0, sizeof(command));

                sendString("Successfully write in file. \n", command, socket);
                memset(command, 0, sizeof(command));

            }
        }
}

void Servidor::exitCommand(char command[1024], int socket){
    if(strncmp(command, "exit", 4) == 0){
        sendString("Succesfully disconnected from the server. \n", command, socket);
        pthread_exit(NULL);
    }
    else
        sendString("Error 404.\n", command, socket);
}

void Servidor::sendToClient(char* buffer, int socket){
    send(socket, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
}

void Servidor::sendString(const char* string, char* buffer, int socket){
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, string);
    sendToClient(buffer, socket);
    memset(buffer, 0, sizeof(buffer));

}

int main(){
    Servidor server;
    server.port = 8080;
    server.createSocket();
	server.listeningSocket();
}
