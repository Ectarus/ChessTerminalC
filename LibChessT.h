//Function readinput
#ifndef LIBCHESS_H
#define LIBCHESS_H

char readinput();

void clearterm1();

//Funzioni delle rispettive modalità di gioco
int checkmove(char[][8], int[2], int[2]);
int movepiece(char[][8], int[2], int[2]);
void localmultiplayer();
void challengebot();
void learn();

#endif