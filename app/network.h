//header file for network.c

//sets up the client side of the socket
void setUpRecSide(void);

//sets up the server side of the socket
void setUpServerSide(void);

//reads from the socket
char* readFromSock(void);

//sends a message through the socket
void sendToSock(char* msg);

//closes the server socket
void closeServer(void);

//closes the client side of the socket
void closeClient(void);