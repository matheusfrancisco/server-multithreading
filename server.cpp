#include "server.h"

void Servidor::createSocket(){

    int addrlen = sizeof(address);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0))==0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);

    }

    //Forcefully attaching socket to the port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

      
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
   

}


void Servidor::listeningSocket(){
    char buffer[1024];
    int valread;
    //char *hello = "Hello from server"; 
	while(true){
         

           
        if (listen(server_fd, 3) < 0) { 
                    perror("listen"); 
                    exit(EXIT_FAILURE); 
                } 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) 
				{ 
				    perror("accept"); 
				    exit(EXIT_FAILURE); 
				} 
			//pthread_create(&threads[i], NULL, task );
        strcpy(buffer, "Conexão realizada!!!\n");
        send(new_socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));

        strcpy(buffer, "Você esta na pasta:\n");
        send(new_socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));

        getcwd(buffer, sizeof(buffer) );
        send(new_socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "\n");

        send(new_socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));

        

        while(1){
                
                //printf("teste %s\n",buffer ); 
                
                functionSystemFile(new_socket);
        }
	 }
}



int main(){

    Servidor server;
    server.port = 8080;
    server.createSocket();
	server.listeningSocket();
}
