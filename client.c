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

//client
int main(int argc, char **argv)
{
    char *server_name;
    int server_port;
    int client_sock;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char message[MSG_SIZE];
    int size;

    if (argc != 3){
        printf("Usage:\n %s \n", argv[0]);
        return 0;
    }

    server_name = argv[1];
    //get port from command line arg
    server_port = atoi(argv[2]);
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock == -1){
        fprintf(stderr, "Error creating client socket\n");
        exit(1);
    }

    //initialize socket to 0
    memset(&server_addr, 0, sizeof(server_addr));

    //set connection to TCP
    server_addr.sin_family = AF_INET;

    //set the server port number
    server_addr.sin_port = htons(server_port);

    //get host using the server name
    host = gethostbyname(server_name);

    if (host == NULL){
        fprintf(stderr, "Error getting the server address\n");
        exit(1);
    }

    //copy address
    memmove(&server_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    // connect to server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        fprintf(stderr, "Error connecting to server\n");
        exit(1);
    }

    while (1)
    {
        printf("Message to send: ");
        fgets(message, MSG_SIZE, stdin);
        message[strlen(message) - 1] = 0;

        //send the message back to server
        send(client_sock, message, strlen(message), 0);

        //receive echo from server
        size = recv(client_sock, message, MSG_SIZE, 0);
        message[size] = 0;
        printf("Echoed message: %s\n", message);

        //if the echo message is fin close connection
        if (!strcmp(message, "nif")){
            break;
        }
    }

    //close socket
    close(client_sock);
    return 0;
}
