#include "server.h"

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
    char buffer[1024];
    int valread;

	while(true){

        if (listen(server_fd, 3) < 0) { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
		
        //pthread_create(&threads[i], NULL, task );
        strcpy(buffer, "Conexão realizada!!!\n");
        sendToClient(buffer, new_socket);

        strcpy(buffer, "Você esta na pasta:\n");
        sendToClient(buffer, new_socket);

        getcwd(buffer, sizeof(buffer));
        sendToClient(buffer, new_socket);

        strcpy(buffer, "\n");
        sendToClient(buffer, new_socket);

        while(1){
            functionSystemFile(new_socket);
        }
	 }
}

void Servidor::functionSystemFile( int socket){
    
    char  command[1024];

    int valread = read( socket , command, 1024);

    if(strncmp(command, "mkdir", 5) == 0){
        mkdirCommand(command, socket);
    }
    else if(strncmp(command, "touch", 5) ==0){
        touchCommand(command, socket);
    }
    else if(strncmp(command, "cat ", 4) == 0){  
        catCommand(command, socket);
        //cout<< "Estou aqui" <<endl;
    }
    else if(strncmp(command, "ls", 2) == 0){
        cout << "Estamos aqui" <<endl;

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
        rewinddir(dir_p);	
        cout << "Estamos aqui" <<endl;

        send(socket,ls,strlen(ls),0);

    }else{
        memset(command, 0, sizeof(command));
        snprintf(command, sizeof(command),"Error 404.\n");
        sendToClient(command, socket);
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
    
    // TÁ DANDO SEG FAULT AQUI
    
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

        file = fopen("test.txt", "r");
        
        if(file == NULL)
            sendString("Error 404.\n", sendMSG, socket);
        else{
            memset(sendMSG, 0, sizeof(sendMSG));
            fread(sendMSG, sizeof(char), 1024, file);
            sendToClient(sendMSG, socket);
        }
        fclose(file);
    }
}

void Servidor::sendToClient(char* buffer, int socket){
    send(socket, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
}

void Servidor::sendString(const char* string, char* buffer, int socket){
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, string);
    sendToClient(buffer, socket);
}

int main(){
    Servidor server;
    server.port = 8080;
    server.createSocket();
	server.listeningSocket();
}
