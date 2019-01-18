#ifndef COMMUNICATION_H
#define COMMUNICATION_H

void deserializeMatrice(char** matrice, char* buffer, short row, short col);
char* serializeMatrice(char** matrice, short row, short col);
void sendMatrice(char** matrice, int sock, short row, short col);

#endif // COMMUNICATION_H
