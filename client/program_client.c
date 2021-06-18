#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#define PORT 8080

void clear_buffer(char* b) 
{
    int i;
    for (i = 0; i < BUFSIZ ; i++)
        b[i] = '\0';
}

int auth(int sockets , char* uname, char* pass)
{
    char buffers[BUFSIZ];
    clear_buffer(buffers);
    char senduser[BUFSIZ];
    sprintf(senduser,"%s:%s",uname,pass);
    send(sockets,senduser,strlen(senduser),0);

    read(sockets,buffers,BUFSIZ);
    if(strcmp(buffers,"Berhasil")==0)
    {
        printf("Login berhasil!\n");
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    int var;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    var = auth(sock,argv[2],argv[4]);
    return 0;
}
