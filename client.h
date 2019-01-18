#ifndef CLIENT_H
#define CLIENT_H

void startClient(void);
int establishConnection(char* ip_address);
void playPvPGame(int socket);

#endif // CLIENT_H
