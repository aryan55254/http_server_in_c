#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int server_fd; //just a number that represents our socket 
    struct sockaddr_in address; //struct tells that it is a structure sockaddr tells that it is socket adress
    int opt = 1; //integer used to set socket options 
    int addrlen = sizeof(address);
    //lenght of adress structure 

    server_fd = socket(AF_INET , SOCK_STREAM , 0);
    //creates socket ,af_inet is IPv4 , everydevice needs unique ip adress to send and recieve data , ipv4 gives us way to assign these adresses , sock stream means u wanna use tcp , its a prototocol tcp creates connection b/w two devices before transmitting data  it has rules data arrives in order data isn't lost no duplicates it checks error it also prevents 
   if (server_fd < 0){//IF -1 WE CHECK 
    perror("socket failed");
    exit(EXIT_FAILURE);
   }
   
   
    if (setsockopt(server_fd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt))){
        //setsockopt set options on socket solsocket sets level of option soreuseaddr is name of option &opt is poiter to the value u want to set option to size of size of opt variable  this returns int if it is non 0 it means there was an error 
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
        //what this whole thing does is basically allow to reuse the socket
    }
address.sin_family = AF_INET;//our socket uses ipv4 
address.sin_addr.s_addr = INADDR_ANY;//it binds to any local ip 
address.sin_port = htons(8080);//sets port 8080 which server will listen to 
//bind means associating a socket with specififc ip address and port number this is how os know where your application will listen for incoming connections 

if (bind(server_fd,(struct sockaddr *)&address , sizeof(address))<0){
    perror("bind failed");
    exit(EXIT_FAILURE);
}
//BIND IS TRYING TO ASSOCIATE SERVER_FD WITH SPECIFIED IP ADRESS AND  PORT NUMBER IN ADRESS returns 0 on success -1 on failure

if(listen(server_fd , 3) < 0){
    perror("listen failure");
    exit(EXIT_FAILURE);  //this basically makes the socket listen on the port and shit and says there can be max 3 connection req that can be queued  at a time
}
printf("server is running on http://localhost:8080\n");
return 0;
}
