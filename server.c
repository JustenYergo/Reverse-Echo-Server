#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MSG_SIZE 100

//reverse message function
void reverse(char *msg, char *rev)
{
    int i, l;
    l = strlen(msg);
    for (i = 0; i < l; i++)
        rev[i] = msg[l - i - 1];
}

//server
int main(int argc, char **argv)
{
    int server_sock;
    int server_port;
    int client_sock;
    unsigned int len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char message[MSG_SIZE], revmsg[MSG_SIZE];
    int size;

    if (argc != 2){
        printf("Usage:\n %s \n", argv[0]);
        return 0;
    }

    //getting port from command line arg
    server_port = atoi(argv[1]);
    printf("Starting the server\n");

    //create socket server
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1){
        fprintf(stderr, "Error creating server socket\n");
        exit(1);
    }
 
    //initialize socket to 0
    memset(&server_addr, 0, sizeof(server_addr));

    //set the connection type to TCP
    server_addr.sin_family = AF_INET;

    //set server address to an interface
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //set server port
    server_addr.sin_port = htons(server_port);

    //bind the socket to the address
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
        fprintf(stderr, "Error binding server socket\n");
        exit(1);
    }

    //listen for client connections
    printf("Waiting for client connections\n");
    if (listen(server_sock, 5) < 0){
        fprintf(stderr, "Error while listening for connections\n");
        exit(1);
    }
    len = sizeof(struct sockaddr_in);

    //accept connection from client
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);
    if (client_sock == -1){
        fprintf(stderr, "Error accepting client\n");
        exit(1);
    }
    printf("\nClient connected!\n");
    while (1){
        size = recv(client_sock, message, MSG_SIZE, 0);
        message[size] = 0;

        //reverse the message
        reverse(message, revmsg);

        //echo the reversed message back to client
        send(client_sock, revmsg, size, 0);

        //end if client message is fin
        if (!strcmp(message, "fin"))
            break;
    }

    //close socket
    close(client_sock);
    close(server_sock);
    printf("Terminating server\n");
    return 0;
}