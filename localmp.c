#include "localmp.h"
#include "HomeBanner.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>

#else
    #include <termios.h>
    #include <unistd.h>

#endif

char *PGN;

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
int nmove=0, is_selected, is_validmove;
int movepiece(char ChessB[8][8], int Cur1[2], int Cur2[2]) {

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
    makePGN(ChessB, Cur1, Cur2, 'G');
    nmove++;
    return 0;

}

/******************************************************************************************************** */

void makePGN(char ChessB[8][8], const int Cur1[2], const int Cur2[2], char promo) {}

void stampaPGN(void) {
    if (PGN != NULL) {
        printf("PGN Corrente:\n%s\n", PGN);
    } else {
        printf("Nessun PGN generato.\n");
    }
}

/*********************************************************************************************************** */
int checkmove(char ChessB[8][8], int Cur1[2], int Cur2[2]) {
    int isWhiteTurn;
    char destPiece;

    isWhiteTurn = !(nmove & 1);

    /* Controllo che il giocatore muova i suoi pezzi */
    if ((isWhiteTurn && ChessB[Cur1[0]][Cur1[1]] >= 'A' && ChessB[Cur1[0]][Cur1[1]] <= 'Z') ||
        (!isWhiteTurn && ChessB[Cur1[0]][Cur1[1]] >= 'a' && ChessB[Cur1[0]][Cur1[1]] <= 'z')) {
        
        destPiece = ChessB[Cur2[0]][Cur2[1]];

        /* Impedisce di muovere/mangiare sopra un pezzo alleato */
        if (isWhiteTurn && destPiece >= 'A' && destPiece <= 'Z') return 0;
        if (!isWhiteTurn && destPiece >= 'a' && destPiece <= 'z') return 0;

        switch(ChessB[Cur1[0]][Cur1[1]]) {
            case 'p':   /* pedone nero */
                if(Cur2[0]==Cur1[0]+1 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.') return 1;
                else if(Cur2[0]==Cur1[0]+2 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.' && ChessB[Cur2[0]-1][Cur2[1]]=='.' && Cur1[0]==1) return 1;
                else if(Cur2[0]==Cur1[0]+1 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1) && ChessB[Cur2[0]][Cur2[1]]>='A' && ChessB[Cur2[0]][Cur2[1]]<='Z') return 1;
                break;
            case 'P':   /* pedone bianco */
                if(Cur2[0]==Cur1[0]-1 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.') return 1;
                else if(Cur2[0]==Cur1[0]-2 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.' && ChessB[Cur2[0]+1][Cur2[1]]=='.' && Cur1[0]==6) return 1;
                else if(Cur2[0]==Cur1[0]-1 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1) && ChessB[Cur2[0]][Cur2[1]]>='a' && ChessB[Cur2[0]][Cur2[1]]<='z') return 1;
                break;
            case 'r':   /* torre nera */
            case 'R':   /* torre bianca */
                if(Cur2[0]==Cur1[0] || Cur2[1]==Cur1[1]) return 1;
                break;
            case 'n':   /* cavallo nero */
            case 'N':   /* cavallo bianco */
                if((Cur2[0]==Cur1[0]+2 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1)) || (Cur2[0]==Cur1[0]-2 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1)) || 
                    (Cur2[0]==Cur1[0]+1 && (Cur2[1]==Cur1[1]+2 || Cur2[1]==Cur1[1]-2)) || (Cur2[0]==Cur1[0]-1 && (Cur2[1]==Cur1[1]+2 || Cur2[1]==Cur1[1]-2))) return 1;
                break;
            case 'b':   /* alfiere nero */
            case 'B':   /* alfiere bianco */
                if(abs(Cur2[0]-Cur1[0])==abs(Cur2[1]-Cur1[1])) return 1;
                break;
            case 'q':   /* regina nera */
            case 'Q':   /* regina bianca */
                if(Cur2[0]==Cur1[0] || Cur2[1]==Cur1[1] || abs(Cur2[0]-Cur1[0])==abs(Cur2[1]-Cur1[1])) return 1;
                break;
            case 'k':   /* re nero */
            case 'K':   /* re bianco */
                if(abs(Cur2[0]-Cur1[0])<=1 && abs(Cur2[1]-Cur1[1])<=1) return 1;
                break;
            default:
                return 0;
        }
        return 0;
    }
    else return 0;
}
/*********************************************************************************************************** */

int checkcheck(char ChessB[8][8], int Cur1[2], int Cur2[2]) {
    /* 1. Dichiarazione di TUTTE le variabili in cima al blocco (Regola C89) */
    char ChessBcopy[8][8];
    int kingpos[2];
    int kingiswhite;
    char targetKing;
    int found;
    int i, j, d, step, r, c; /* Contatori e coordinate riutilizzabili */
    char piece;
    
    /* Vettori di direzione dichiarati e inizializzati prima del codice eseguibile */
    int diagX[4] = {1, 1, -1, -1};
    int diagY[4] = {1, -1, 1, -1};
    int orthoX[4] = {1, -1, 0, 0};
    int orthoY[4] = {0, 0, 1, -1};
    int knightX[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    int knightY[8] = {1, -1, 1, -1, 2, -2, 2, -2};

    /* --------------------------------------------------------- */
    /* 2. Inizio delle istruzioni eseguibili                     */
    /* --------------------------------------------------------- */

    /* Copio la scacchiera e applico la mossa */
    memcpy(ChessBcopy, ChessB, sizeof(ChessBcopy));
    ChessBcopy[Cur2[0]][Cur2[1]] = ChessBcopy[Cur1[0]][Cur1[1]];
    ChessBcopy[Cur1[0]][Cur1[1]] = '.'; /* Svuoto la casella di partenza */

    /* Capisco di chi e' il turno e cerco il Re corretto */
    kingiswhite = (nmove & 1) ? 0 : 1; 
    targetKing = kingiswhite ? 'K' : 'k';
    found = 0;

    for (i = 0; i < 8 && !found; i++) {
        for (j = 0; j < 8 && !found; j++) {
            if (ChessBcopy[i][j] == targetKing) {
                kingpos[0] = i;
                kingpos[1] = j;
                found = 1;
            }
        }
    }

    /* Controllo Diagonali per Alfieri (B/b) e Regine (Q/q) */
    for (d = 0; d < 4; d++) {
        for (step = 1; step < 8; step++) {
            r = kingpos[0] + (diagX[d] * step);
            c = kingpos[1] + (diagY[d] * step);

            if (r < 0 || r >= 8 || c < 0 || c >= 8) break;

            piece = ChessBcopy[r][c];
            if (piece != '.') {
                if (kingiswhite && (piece == 'b' || piece == 'q')) return 0;
                if (!kingiswhite && (piece == 'B' || piece == 'Q')) return 0;
                break;
            }
        }
    }

    /* Controllo Linee Rette per Torri (R/r) e Regine (Q/q) */
    for (d = 0; d < 4; d++) {
        for (step = 1; step < 8; step++) {
            r = kingpos[0] + (orthoX[d] * step);
            c = kingpos[1] + (orthoY[d] * step);

            if (r < 0 || r >= 8 || c < 0 || c >= 8) break;

            piece = ChessBcopy[r][c];
            if (piece != '.') {
                if (kingiswhite && (piece == 'r' || piece == 'q')) return 0;
                if (!kingiswhite && (piece == 'R' || piece == 'Q')) return 0;
                break;
            }
        }
    }

    /* Controllo Cavalli (N/n) */
    for (i = 0; i < 8; i++) {
        r = kingpos[0] + knightX[i];
        c = kingpos[1] + knightY[i];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            piece = ChessBcopy[r][c];
            if (kingiswhite && piece == 'n') return 0;
            if (!kingiswhite && piece == 'N') return 0;
        }
    }

    /* Controllo Pedoni (P/p) */
    if (kingiswhite) {
        if (kingpos[0] > 0) {
            if (kingpos[1] > 0 && ChessBcopy[kingpos[0]-1][kingpos[1]-1] == 'p') return 0;
            if (kingpos[1] < 7 && ChessBcopy[kingpos[0]-1][kingpos[1]+1] == 'p') return 0;
        }
    } else {
        if (kingpos[0] < 7) {
            if (kingpos[1] > 0 && ChessBcopy[kingpos[0]+1][kingpos[1]-1] == 'P') return 0;
            if (kingpos[1] < 7 && ChessBcopy[kingpos[0]+1][kingpos[1]+1] == 'P') return 0;
        }
    }

    /* Controllo del Re avversario (K/k) */
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            r = kingpos[0] + i;
            c = kingpos[1] + j;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                piece = ChessBcopy[r][c];
                if (kingiswhite && piece == 'k') return 0;
                if (!kingiswhite && piece == 'K') return 0;
            }
        }
    }

    /* Se arrivo fin qui, significa che nessuno minaccia il Re */
    return 1;
}
/***************************************** */

int checkcheckmate(char ChessB[8][8]) {
    int i, j, targetR, targetC;
    int cur1[2], cur2[2];
    int dummyCur1[2];
    int dummyCur2[2];
    int isWhiteTurn;
    char p;
    int isMyPiece;

    isWhiteTurn = !(nmove & 1);

    /* Trova una casella vuota per fare un controllo a vuoto (no-op) tramite checkcheck */
    dummyCur1[0] = 0; dummyCur1[1] = 0;
    dummyCur2[0] = 0; dummyCur2[1] = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (ChessB[i][j] == '.') {
                dummyCur1[0] = i; dummyCur1[1] = j;
                dummyCur2[0] = i; dummyCur2[1] = j;
                break;
            }
        }
        if (ChessB[dummyCur1[0]][dummyCur1[1]] == '.') break;
    }

    /* 1. Il re deve essere attualmente sotto scacco. 
       Se checkcheck restituisce 1, significa che NON c'è scacco, quindi non può essere matto. */
    if (checkcheck(ChessB, dummyCur1, dummyCur2)) {
        return 0; 
    }

    /* 2. Scansiona tutta la scacchiera per trovare i pezzi del giocatore di turno */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            p = ChessB[i][j];
            
            isMyPiece = isWhiteTurn ? (p >= 'A' && p <= 'Z') : (p >= 'a' && p <= 'z');
            
            if (isMyPiece) {
                /* 3. Per ogni pezzo, prova a muoverlo in ogni casella della scacchiera */
                for (targetR = 0; targetR < 8; targetR++) {
                    for (targetC = 0; targetC < 8; targetC++) {
                        cur1[0] = i;
                        cur1[1] = j;
                        cur2[0] = targetR;
                        cur2[1] = targetC;
                        
                        /* 4. Verifica se la mossa è geometricamente valida e non amichevole */
                        if (checkmove(ChessB, cur1, cur2)) {
                            
                            /* 5. Simula la mossa ed esegui checkcheck.
                               Se checkcheck restituisce 1, la mossa ha salvato il re -> Non è matto. */
                            if (checkcheck(ChessB, cur1, cur2)) {
                                return 0; 
                            }
                        }
                    }
                }
            }
        }
    }

    /* 6. Se il ciclo finisce senza trovare alcuna mossa legale salvifica, è scacco matto. */
    return 1;
}

void clearterm1() { //pulisce lo schermo

    #ifdef _WIN32
        system("cls");
        return;

    #else 
        system("clear");
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

    int endgame[4] = {1,0,0,0};
    int i,j,x,y,t;
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

printf("    -----------------------------------------------------------------\n");
for(i = 0; i < 8; i++) {
    for(x = 0; x < 5; x++) {
        printf("    ");
        for(j = 0; j < 8; j++) {
            printf("|");
            
            // Determina se la cella corrente è selezionata dal cursore
            int is_selected = (Cur1[0] == i && Cur1[1] == j) || (Cur2[0] == i && Cur2[1] == j);

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
        printf("|");
        if(x==1 && i==0) printf("     --------------------------------------------");
        if(x==2 && i==0) printf("     |          LOCAL MULTIPLAYER MODE          |");
        if(x==3 && i==0) printf("     | Use wasd keys to move the cursor and     |");
        if(x==4 && i==0) printf("     | press enter to select/confirm your move  |");
        if(x==0 && i==1) printf("     | Press 'q' to quit the game               |");
        if(x==1 && i==1) printf("     --------------------------------------------");
        printf("\n");
        

    }
    printf("    -----------------------------------------------------------------\n");
}

        //controllo lo scacco matto
        if( checkcheckmate(ChessB) ) {
            clearterm1();
            while(1) {

                clearterm2();
                printf("\n\n\n\n");
                if(nmove & 1) printf(CheckMateBannerW);
                else printf(CheckMateBannerB);

                menuendgame:
                if(endgame[0])    printf("%s\n", postmate1);
                else if(endgame[1])    printf("%s\n", postmate2);
                else if(endgame[2])    printf("%s\n", postmate3);
                else if(endgame[3])    printf("%s\n", postmate4);

                input = readinput();

                if (input == 'w' || input == 'W' || input == 'd' || input == 'D') {
                    t = endgame[3];
                    for (i = 2; i >= 0; i--) {
                        endgame[i + 1] = endgame[i];
                    }
                    endgame[0] = t;
                }
                else if (input == 's' || input == 'S' || input == 'A' || input == 'a') {
                    t = endgame[0];
                    for (i = 0; i <= 2; i++) {
                        endgame[i] = endgame[i + 1];
                    }
                    endgame[3] = t; // Corretto l'indice finale a 3
                }
                else if (input == '\n' || input == '\r') {
                    if (endgame[0]) {
                        stampaPGN();
                        goto menuendgame;
                    }
                    if (endgame[1]) return 1;
                    if (endgame[2]) return 1;
                    if (endgame[3]) return 1;
                    return 0;
                }
            
            }
            
                return;
            }

        input = readinput();

        if(input=='q' || input=='Q') { //condizione di uscita dalla modalità
            return;
        }

        //x e y sono le coordiante del cursore, ogni volta che mi muovo negono aggiornate
        if(!firstC) {//modifico il primo cursore

            if((input=='w' || input=='W') && Cur1[0] > 0) Cur1[0]--;
            else if((input=='s' || input=='S') && Cur1[0] < 7) Cur1[0]++;
            else if((input=='d' || input=='D') && Cur1[1] < 7)   Cur1[1]++;
            else if((input=='a' || input=='A') && Cur1[1] > 0)   Cur1[1]--;
            else if(input=='\r' || input=='\n')     firstC++;
            Cur2[0]=Cur1[0];    Cur2[1]=Cur1[1]; //aggiorno in automatico anche il cursore2

        }
        else {//modifico il secondo curosre

            if((input=='w' || input=='W') && Cur2[0] > 0) Cur2[0]--;
            else if((input=='s' || input=='S') && Cur2[0] < 7) Cur2[0]++;
            else if((input=='d' || input=='D') && Cur2[1] < 7)   Cur2[1]++;
            else if((input=='a' || input=='A') && Cur2[1] > 0)   Cur2[1]--;
            else if(input=='\r' || input=='\n')     {
                /*controllo la validità della mossa*/
                if(checkmove(ChessB, Cur1, Cur2) && checkcheck(ChessB, Cur1, Cur2)) firstC=movepiece(ChessB, Cur1, Cur2);
                else firstC=0;
            }

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