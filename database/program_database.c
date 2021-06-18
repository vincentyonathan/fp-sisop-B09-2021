#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h> 
#include <limits.h>
#define PORT 8080  

void clear_buffer(char* b) 
{
    int i;
    for (i = 0; i < BUFSIZ; i++)
        b[i] = '\0';
}

// void handleQuery(int socketfd) {
//     char buffer[BUFSIZ];
//     while (1)
//     {
//         clearBuffer(buffer);
//         read(socketfd, buffer, BUFSIZ);
//         printf("%s\n", buffer);

//         int res = 0;

//         if (buffer[strlen(buffer)-1] == ';')
//         {
//             buffer[strlen(buffer)-1] = '\0';

//             if (authInterface(buffer))
//                 res = 1;
//         }

//         if (res)
//             send(socketfd, OK, strlen(OK), 0);
//         else 
//             send(socketfd, FAIL, strlen(FAIL), 0);
//     } 
// }

void client(int socketfd)
{
    int valread;
    char command[BUFSIZ];
    char userpass[100];

    // clear_buffer(command);
    // valread = read(socketfd , command, BUFSIZ);
    // fprintf(stdout, "Receive connection handshake %s\n", command);

    clear_buffer(command);
    valread = read(socketfd, command, BUFSIZ);

    // printf("Ini ngeprint while client\n");
    FILE *fdir;
    fdir = fopen("akun.txt","a+"); 
    char temp[BUFSIZ];
    char userada[10];
    int flag = 0;

    printf("Ini command --> %s\n",command);
    strcpy(userpass,command);
    while(fgets(temp, BUFSIZ, fdir) != NULL) 
    {
        // printf("%s",temp);
        if((strstr(temp, command)) != NULL) 
        {
            printf("Login Berhasil\n");
            flag = 1;
            char kata[20] = "Berhasil";
            send(socketfd,kata, strlen(kata),0);
            break;
        }   
    }
    // while (flag == 1)
    // {
        
    // }
       
               
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    client(new_socket);
    return 0;
}

