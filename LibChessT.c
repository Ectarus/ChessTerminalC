#include "LibCHessT.h"
#include "HomeBanner.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>

#else
    #include <termios.h>
    #include <unistd.h>

#endif

char readinput() {

    #ifdef _WIN32
        return getch();

    #else
        struct termios vecchio, nuovo;
        char ch;
        tcgetattr(STDIN_FILENO, &vecchio); // Salva le impostazioni attuali
        nuovo = vecchio;
        nuovo.c_lflag &= ~(ICANON | ECHO); // Disabilita buffering e la stampa a schermo
        tcsetattr(STDIN_FILENO, TCSANOW, &nuovo); // Applica le nuove impostazioni
        ch = getchar(); // Ora getchar() legge IMMEDIATAMENTE
        tcsetattr(STDIN_FILENO, TCSANOW, &vecchio); // Ripristina le impostazioni originali
        return ch;

    #endif
    }

/***************************************************************************************************************** */
int nmove=0, is_selected;
int movepece(char ChessB[8][8], int Cur1[2], int Cur2[2]) {

    ChessB[ Cur2[0] ][ Cur2[1] ]=ChessB[ Cur1[0] ][ Cur1[1] ];
    ChessB[ Cur1[0] ][ Cur1[1] ]='.';
    //resetto i cursori
    if(nmove & 1) {//se il numero di mosse è pari gioca il bianco
        Cur1[0]=7;  Cur1[1]=0;
        Cur2[0]=7;  Cur2[1]=0;
    }
    else {
        Cur1[0]=0;  Cur1[1]=7;
        Cur2[0]=0;  Cur2[1]=7;
    }
    
    nmove++;
    return 0;

}

void clearterm1() { //pulisce lo schermo

    #ifdef _WIN32
        system("cls");
        return;

    #else 
        printf("\033[2J\033[H");
        return;
    
    #endif
}

/*************************************************************************************************************** */

void clearterm2() { //riporta il cursore in alto a sinistra

    #ifdef _WIN32
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        return;

    #else 
        printf("\033[H");
        return;
    
    #endif
}

/*************************************************************************************************************** */

void localmultiplayer() {
    clearterm1();

    int i,j,x,y;
    int Cur1[2] = {7,0};
    int Cur2[2] = {7,0};
    char input, firstC=0;
    //Definisco la cacchiera e ci varico i pezzi
    char ChessB[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}, // Riga 0: Pezzi neri
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, // Riga 1: Pedoni neri
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Riga 2: Vuota
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Riga 3: Vuota
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Riga 4: Vuota
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Riga 5: Vuota
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}, // Riga 6: Pedoni bianchi
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}  // Riga 7: Pezzi bianchi
    };
    
    char ChessC[8][8] = 
    {
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}, 
    {'_', '_', '_', '_', '_', '_', '_', '_'}  
    };

    while(1) {

        clearterm2();

        /*
        //funzione provvisoria stampa scacchiera
        for(i=0;i<8;i++) {

            printf("\t");
            for(j=0;j<8;j++) {
                printf("| %c |", ChessB[i][j]);
            }
            printf("\n");

        }
        printf("\n");
        //funzione provvisoria stampa cursore 1
        for(i=0;i<8;i++) {

            printf("\t");
            for(j=0;j<8;j++) {
                if(i==Cur1[0] && j==Cur1[1])   printf("| X |");
                printf("| %c |", ChessC[i][j]);
            }
            printf("\n");

        }
        printf("\n");
        //funzione provvisoria stampa cursore 2
        for(i=0;i<8;i++) {

            printf("\t");
            for(j=0;j<8;j++) {
                if(i==Cur2[0] && j==Cur2[1])   printf("| X |");
                printf("| %c |", ChessC[i][j]);
            }
            printf("\n");

        }
        */
        //////////////////////////////////// CICLO DI STAMPA SCACCHIERA GRANDE /////////////////////////////////////ù

printf("-----------------------------------------------------------------\n");
for(i = 0; i < 8; i++) {
    for(x = 0; x < 5; x++) {
        for(j = 0; j < 8; j++) {
            printf("|");
            
            // Determina se la cella corrente è selezionata dal cursore
            int is_selected = (!firstC && Cur1[0] == i && Cur1[1] == j) || 
                              (firstC && Cur2[0] == i && Cur2[1] == j);

            switch (ChessB[i][j]) {
                case 'p': printf("%s", is_selected ? PawnBC[x] : PawnB[x]); break;
                case 'r': printf("%s", is_selected ? RockBC[x] : RockB[x]); break;
                case 'n': printf("%s", is_selected ? KnightBC[x] : KnightB[x]); break;
                case 'b': printf("%s", is_selected ? BishopBC[x] : BishopB[x]); break;
                case 'q': printf("%s", is_selected ? QueenBC[x] : QueenB[x]); break;
                case 'k': printf("%s", is_selected ? KingBC[x] : KingB[x]); break;
                case 'P': printf("%s", is_selected ? PawnWC[x] : PawnW[x]); break;
                case 'R': printf("%s", is_selected ? RockWC[x] : RockW[x]); break;
                case 'N': printf("%s", is_selected ? KnightWC[x] : KnightW[x]); break;
                case 'B': printf("%s", is_selected ? BishopWC[x] : BishopW[x]); break;
                case 'Q': printf("%s", is_selected ? QueenWC[x] : QueenW[x]); break;
                case 'K': printf("%s", is_selected ? KingWC[x] : KingW[x]); break;
                case '.': printf("%s", is_selected ? EmptySC[x] : EmptyS[x]); break;
                default:  printf("       "); break;
            }
        }
        printf("|\n");
    }
    printf("-----------------------------------------------------------------\n");
}



















        input = readinput();

        //x e y sono le coordiante del cursore, ogni volta che mi muovo negono aggiornate
        if(!firstC) {//modifico il primo cursore

            if(input=='w' || input=='W') Cur1[0]--;
            else if(input=='s' || input=='S') Cur1[0]++;
            else if(input=='d' || input=='D')   Cur1[1]++;
            else if(input=='a' || input=='A')   Cur1[1]--;
            else if(input=='\r' || input=='\n')     firstC++;
            Cur2[0]=Cur1[0];    Cur2[1]=Cur1[1]; //aggiorno in automatico anche il cursore2

        }
        else {//modifico il secondo curosre

            if(input=='w' || input=='W') Cur2[0]--;
            else if(input=='s' || input=='S') Cur2[0]++;
            else if(input=='d' || input=='D')   Cur2[1]++;
            else if(input=='a' || input=='A')   Cur2[1]--;
            else if(input=='\r' || input=='\n')     firstC = movepece(ChessB, Cur1, Cur2); //resetto anche la condizione

        }

    }

}

/************************************************************************************************************************ */

    void challengebot() {
        
        clearterm1();
        printf("challenge bot");
        getchar();
        return;
        
    }

/********************************************************************************************************************* */

    void learn() {
        
        clearterm1();
        printf("learn");
        getchar();
        return;

    }