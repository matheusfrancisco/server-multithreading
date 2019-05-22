#include "server.h"



void* Servidor::resolveRequest(void* args){
    char buffer[1024];
    reqArgs request = *((reqArgs*) args);
    int socket = request.socket;

    if (socket < 0){ 
        perror("accept"); 
        pthread_exit(NULL); 
    } 

    strcpy(buffer, "\033[0;32mConnection successfull!\033[0m\n");
    sendToClient(buffer, socket);

    strcpy(buffer, "\033[0;34mCurrent directory:\n");
    sendToClient(buffer, socket);

    strcpy(buffer, request.thDir);
    sendToClient(buffer, socket);

    strcpy(buffer, "\033[0m\n");
    sendToClient(buffer, socket);

    while(1){
        sendString(">> ", buffer, socket);
        functionSystemFile(socket, request.mutex, request.thDir);
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
    getcwd(stdDir, sizeof(stdDir));

	while(true){

        if (listen(server_fd, 3) < 0) { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        }

        for(int i = 0; i < N; i++){
            new_req.socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen);
            strcpy(new_req.thDir, stdDir);
            pthread_create(&threads[i], NULL, resolveRequest, (void*) &new_req);
        }

        for(int i = 0; i < N; i++){
            pthread_join(threads[i], NULL);
        }
	 }
}

void Servidor::functionSystemFile( int socket, pthread_mutex_t* mutex, char dir_atual[1024]){
    char  command[1024];

    int valread = read( socket , command, 1024);
    chdir(dir_atual);

    if(strncmp(command, "mkdir", 5) == 0){
        pthread_mutex_lock(mutex);
        mkdirCommand(command, socket);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else if(strncmp(command, "touch", 5) == 0){
        pthread_mutex_lock(mutex);
        touchCommand(command, socket);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else if(strncmp(command, "cat ", 4) == 0) {
        pthread_mutex_lock(mutex);
        catCommand(command, socket);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else if(strncmp(command, "ls", 2) == 0){
        pthread_mutex_lock(mutex);
        lsCommand(command, socket, dir_atual);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else if(strncmp(command, "exit", 4) == 0)
        exitCommand(command, socket);
    else if(strncmp(command, "cd ",3) == 0)
        cdCommand(command, socket, dir_atual);
    else if(strncmp(command, "rm -r",5) ==0){
        pthread_mutex_lock(mutex);
        rmCommand(command, socket);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else if(strncmp(command, "echo", 4)==0){
        pthread_mutex_lock(mutex);
        writCommand(command,socket);
        memset(command, 0, sizeof(command));
        pthread_mutex_unlock(mutex);
    }
    else
        sendString("\033[0;31mError 404.\033[0m\n", command, socket);
}

void Servidor::mkdirCommand(char command[1024], int socket){
    if(strncmp(command, "mkdir", 5) == 0){
        if(system(command) == 0)
            sendString("\033[0;32mSuccessufly created folder \033[0m\n", command, socket);
        else
            sendString("\033[0;31mError 404.\033[0m\n", command, socket);
    }
    else
        sendString("\033[0;31mError 404.\033[0m\n", command, socket);
}

void Servidor::touchCommand(char command[1024], int socket){
    if(strncmp(command, "touch", 5) == 0){
        if(system(command) == 0)
            sendString("\033[0;32mSuccessufly created file \033[0m\n", command, socket);
        else
            sendString("\033[0;31mError 404.\033[0m\n", command, socket);
    }
    else
        sendString("\033[0;31mError 404.\033[0m\n", command, socket);
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

        strcat(path,"/");
        strcat(path, command);

        file = fopen(command, "r");
        
        if(file == NULL)
            sendString("\033[0;31mError 404.\033[0m\n", sendMSG, socket);
        else{
            memset(sendMSG, 0, sizeof(sendMSG));
            fread(sendMSG, sizeof(char), 1024, file);
            sendToClient(sendMSG, socket);
        }
        fclose(file);
    }
}

void Servidor::cdCommand(char command[1024], int socket, char dir[1024]){
    char buffer[1024];
    if(command[strlen(command)-1] == 10)
        command[strlen(command)-1] = 0x00;

    if(strncmp(command, "cd ", 3) == 0){

            memmove(command, command + 3, strlen(command));
            chdir(command);

            sendString("\033[1;32mNew directory:\n", command, socket);
            memset(dir, 0, sizeof(dir));
            getcwd(buffer, sizeof(buffer));
            cout<<buffer<<"\n";
            
            strcpy(dir, buffer);
            sendToClient(buffer, socket);
 
            sendString("\n\033[0m", command, socket);     
    }
    else
        sendString("\033[0;31mError 404.\033[0m\n", command, socket);
}
void Servidor::rmCommand(char command[1024], int socket){
        if(command[strlen(command)-1] == 10)
            command[strlen(command)-1] = 0x00;

        if(strncmp(command, "rm -r", 5) == 0){
            if(system(command)==0)
                sendString("\033[0;32mSuccesfully removed file. \033[0m\n", command, socket);
            else
                sendString("\033[0;31mError removing file. \033[0m\n", command, socket);
        }
}

void Servidor::writCommand(char command[1024], int socket){
        if(command[strlen(command)-1] == 10)
            command[strlen(command)-1] = 0x00;

        if(strncmp(command, "echo ", 5) == 0){
            if(system(command)==0)
                sendString("\033[0;32mSuccessfully wrote in file. \033[0m\n", command, socket);
            else
                sendString("\033[0;31mError removing file. \033[0m\n", command, socket);
        }
}

void Servidor::lsCommand(char command[1024], int socket, char dir[1024]){
    char ls[1024];
    char cp[1024];

    DIR * dir_p = NULL;
    struct dirent *directory = NULL;
    dir_p =  opendir(".");
    memset(ls,0, sizeof(ls));
    strcat(ls,"\033[1;34mPath >");
    strcat(ls, dir);
    strcat(ls,"/\n\t\033[0m");
    
    while(directory = readdir(dir_p)){
        if(directory->d_type == 4)
            strcat(ls, "\033[0;33m");
        strcat(ls, directory->d_name);
        if(directory->d_type == 4)
            strcat(ls, "/\033[0m");
        strcat(ls, "\t\n\t");
    }
    strcat(ls, "\n");
    rewinddir(dir_p);

    sendToClient(ls, socket);
}

void Servidor::exitCommand(char command[1024], int socket){
    if(strncmp(command, "exit", 4) == 0){
        sendString("\033[0;32mSuccesfully disconnected from the server. \033[0m\n", command, socket);
        pthread_exit(NULL);
    }
    else
        sendString("\033[0;31mError 404.\033[0m\n", command, socket);
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
    getcwd(server.stdDir, sizeof(server.stdDir));
    server.new_req.mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(server.new_req.mutex, NULL);
    server.port = 8080;
    server.createSocket();
	server.listeningSocket();
}
