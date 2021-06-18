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

int sendQuery(int sock)
{
    char command[BUFSIZ];
    char buffers[BUFSIZ];
    while(1)
    {
        fgets(command,sizeof(command),stdin);
        //agar enter hilang
        command[strcspn(command, "\n")] = 0;

        send(sock,command,strlen(command),0);

        clear_buffer(buffers);
        read(sock,buffers,BUFSIZ);
        if(strstr(buffers,"CREATE"))
        {
            printf("Create berhasil.\n");
        }
        else
        {
            printf("Create gagal.\n");
        }
        
    }
}

int auth(int sockets , char* uname, char* pass, int isroot)
{
    char buffers[BUFSIZ];
    clear_buffer(buffers);
    char senduser[BUFSIZ];
    sprintf(senduser,"%s:%s",uname,pass);
    if(isroot == 1)
    {
        send(sockets,"root",strlen("root"),0);
    }
    else
    {
        send(sockets,senduser,strlen(senduser),0);
    }

    read(sockets,buffers,BUFSIZ);
    if(strcmp(buffers,"Berhasil")==0)
    {
        printf("Login berhasil!\n");
        return 1;
    }
    return 0;
}


int main(int argc, char* argv[]) {
    int isroot;
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


    if (geteuid() != 0) {
        isroot = 0;
        var = auth(sock, argv[2], argv[4], isroot);
    }
    else {
        isroot = 1;
        var = auth(sock, argv[2], argv[4], isroot);
    }

    if (!var) {
        printf("Login anda gagal\n");
        exit(0);
    }
    
    printf("Login berhasil\n");
    sendQuery(sock);
    return 0;
}
