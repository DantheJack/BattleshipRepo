#ifndef SERVER_H
#define SERVER_H

void* clientHandler(void* param);
void* startServer(void* param);
void displayMatrice(char** matrice);
int processRequest(char* buffer);


#endif // SERVER_H
