#ifndef LOCALMP_H
#define LOCALMP_H
 
//Input reading
char readinput(void);
 
//Screen handling
void clearterm1(void);
void clearterm2(void);
 
//PGN handling
void makePGN(char[8][8], const int[2], const int[2], char);
void printPGN(void);
 
//Board rules and move handling
int isPathClear(char[8][8], int, int, int, int);
int checkmove(char[8][8], int[2], int[2]);
int checkcheck(char[8][8], int[2], int[2]);
int checkcheckmate(char[8][8]);
int movepiece(char[8][8], int[2], int[2]);
 
//Functions of the different game modes
int localmultiplayer(void);
void challengebot(void);
void learn(void);
 
#endif
 