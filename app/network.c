//Network file for UDP transfer
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "network.h"

static int recSock;
static int serveSock;

void setUpRecSide(void){
    recSock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockadddr *) &server_address, sizeof(server_address));
    while(connection_status == -1){//should only error if server isn't set up
        connection_status = connect(network_socket, (struct sockadddr *) &server_address, sizeof(server_address));
    }
}

void setUpServerSide(void){
    serveSock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(serveSock, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(serveSock, 1);
}

char* readFromSock(void){
    char result[256];
    recv(recSock, &result, sizeof(result));
    return result;
}

void sendToSock(char* msg){
    int clientSock = accept(serveSock, NULL, NULL);

    send(clientSock, msg, sizeof(msg), 0);
}

void closeServer(void){
    close(serveSock);
}

void closeClient(void){
    close(recSock);
}