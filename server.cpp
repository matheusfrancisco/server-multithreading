#include "server.h"

void Servidor::createSocket(){

    int addrlen = sizeof(address);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0))==0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);

    }

    //Forcefully attaching socket to the port
    if(setsockopt(server_fd, SQL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    cout << "Test port is:" << port;

}

void Servidor::listeningSocket(){
	 while(true){
			 if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
				                   (socklen_t*)&addrlen))<0) 
				{ 
				    perror("accept"); 
				    exit(EXIT_FAILURE); 
				} 
				valread = read( new_socket , buffer, 1024); 
				printf("%s\n",buffer ); 
				send(new_socket , hello , strlen(hello) , 0 ); 
				printf("Hello message sent\n"); 	
	 }
}


int main(){

    Servidor server;
    server.port = 8080;
    server.createSocket();
	  //server.listeningSocket();
}
