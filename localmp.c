#include "localmp.h"
#include "homebanner.h"

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

char *PGN = NULL;

char readinput(void) {

    #ifdef _WIN32
        return _getch();

    #else
        struct termios oldterm, newterm;
        char ch;
        tcgetattr(STDIN_FILENO, &oldterm); // Save the current settings
        newterm = oldterm;
        newterm.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo on screen
        tcsetattr(STDIN_FILENO, TCSANOW, &newterm); // Apply the new settings
        ch = getchar(); // Now getchar() reads IMMEDIATELY
        tcsetattr(STDIN_FILENO, TCSANOW, &oldterm); // Restore the original settings
        return ch;

    #endif
    }

/***************************************************************************************************************** */
int nmove = 0;

int movepiece(char ChessB[8][8], int Cur1[2], int Cur2[2]) {

    ChessB[ Cur2[0] ][ Cur2[1] ]=ChessB[ Cur1[0] ][ Cur1[1] ];
    ChessB[ Cur1[0] ][ Cur1[1] ]='.';
    //reset the cursors: nmove is still the counter BEFORE this move
    if(nmove & 1) {//odd -> black has just moved, so white plays next
        Cur1[0]=7;  Cur1[1]=0;
        Cur2[0]=7;  Cur2[1]=0;
    }
    else {//even -> white has just moved, so black plays next
        Cur1[0]=0;  Cur1[1]=7;
        Cur2[0]=0;  Cur2[1]=7;
    }
    makePGN(ChessB, Cur1, Cur2, 'G');
    nmove++;
    return 0;

}

/******************************************************************************************************** */

void makePGN(char ChessB[8][8], const int Cur1[2], const int Cur2[2], char promo) {
    /* Not implemented yet: the parameters are voided to keep the build warning free */
    (void)ChessB;
    (void)Cur1;
    (void)Cur2;
    (void)promo;
}

void printPGN(void) {
    if (PGN != NULL) {
        printf("Current PGN:\n%s\n", PGN);
    } else {
        printf("No PGN generated.\n");
    }
}

/*********************************************************************************************************** */

int isPathClear(char ChessB[8][8], int r1, int c1, int r2, int c2) {
    /* We work out the direction of the step (1, -1 or 0) for rows and columns */
    int row_step = (r2 > r1) ? 1 : ((r2 < r1) ? -1 : 0);
    int col_step = (c2 > c1) ? 1 : ((c2 < c1) ? -1 : 0);

    int r = r1 + row_step;
    int c = c1 + col_step;

    /* We walk over the squares in between until we reach the destination */
    while(r != r2 || c != c2) {
        if(ChessB[r][c] != '.') {
            return 0; /* Obstacle found, the piece cannot jump over it! */
        }
        r += row_step;
        c += col_step;
    }

    return 1; /* Path clear */
}

/*********************************************************************************************************** */

int checkmove(char ChessB[8][8], int Cur1[2], int Cur2[2]) {
    int isWhiteTurn;
    char destPiece;

    /* Prevents "moving" a piece onto the square it already sits on */
    if (Cur1[0] == Cur2[0] && Cur1[1] == Cur2[1]) return 0;

    isWhiteTurn = !(nmove & 1);

    /* Check that the player is moving one of his own pieces */
    if ((isWhiteTurn && ChessB[Cur1[0]][Cur1[1]] >= 'A' && ChessB[Cur1[0]][Cur1[1]] <= 'Z') ||
        (!isWhiteTurn && ChessB[Cur1[0]][Cur1[1]] >= 'a' && ChessB[Cur1[0]][Cur1[1]] <= 'z')) {

        destPiece = ChessB[Cur2[0]][Cur2[1]];

        /* Prevents moving onto / capturing a friendly piece */
        if (isWhiteTurn && destPiece >= 'A' && destPiece <= 'Z') return 0;
        if (!isWhiteTurn && destPiece >= 'a' && destPiece <= 'z') return 0;

        switch(ChessB[Cur1[0]][Cur1[1]]) {
            case 'p':   /* black pawn */
                if(Cur2[0]==Cur1[0]+1 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.') return 1;
                else if(Cur2[0]==Cur1[0]+2 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.' && ChessB[Cur2[0]-1][Cur2[1]]=='.' && Cur1[0]==1) return 1;
                else if(Cur2[0]==Cur1[0]+1 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1) && ChessB[Cur2[0]][Cur2[1]]>='A' && ChessB[Cur2[0]][Cur2[1]]<='Z') return 1;
                break;
            case 'P':   /* white pawn */
                if(Cur2[0]==Cur1[0]-1 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.') return 1;
                else if(Cur2[0]==Cur1[0]-2 && Cur2[1]==Cur1[1] && ChessB[Cur2[0]][Cur2[1]]=='.' && ChessB[Cur2[0]+1][Cur2[1]]=='.' && Cur1[0]==6) return 1;
                else if(Cur2[0]==Cur1[0]-1 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1) && ChessB[Cur2[0]][Cur2[1]]>='a' && ChessB[Cur2[0]][Cur2[1]]<='z') return 1;
                break;
            case 'r':   /* black rook */
            case 'R':   /* white rook */
                if(Cur2[0]==Cur1[0] || Cur2[1]==Cur1[1]) {
                    return isPathClear(ChessB, Cur1[0], Cur1[1], Cur2[0], Cur2[1]);
                }
                break;
            case 'n':   /* black knight */
            case 'N':   /* white knight */
                /* The knight JUMPS, so we do not use isPathClear() */
                if((Cur2[0]==Cur1[0]+2 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1)) || (Cur2[0]==Cur1[0]-2 && (Cur2[1]==Cur1[1]+1 || Cur2[1]==Cur1[1]-1)) ||
                    (Cur2[0]==Cur1[0]+1 && (Cur2[1]==Cur1[1]+2 || Cur2[1]==Cur1[1]-2)) || (Cur2[0]==Cur1[0]-1 && (Cur2[1]==Cur1[1]+2 || Cur2[1]==Cur1[1]-2))) return 1;
                break;
            case 'b':   /* black bishop */
            case 'B':   /* white bishop */
                if(abs(Cur2[0]-Cur1[0])==abs(Cur2[1]-Cur1[1])) {
                    return isPathClear(ChessB, Cur1[0], Cur1[1], Cur2[0], Cur2[1]);
                }
                break;
            case 'q':   /* black queen */
            case 'Q':   /* white queen */
                if(Cur2[0]==Cur1[0] || Cur2[1]==Cur1[1] || abs(Cur2[0]-Cur1[0])==abs(Cur2[1]-Cur1[1])) {
                    return isPathClear(ChessB, Cur1[0], Cur1[1], Cur2[0], Cur2[1]);
                }
                break;
            case 'k':   /* black king */
            case 'K':   /* white king */
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

/* Returns 1 if the king of the side to move is SAFE after the move, 0 if it is in check */
int checkcheck(char ChessB[8][8], int Cur1[2], int Cur2[2]) {
    /* 1. Declaration of ALL the variables at the top of the block (C89 rule) */
    char ChessBcopy[8][8];
    int kingpos[2];
    int kingiswhite;
    char targetKing;
    int found;
    int i, j, d, step, r, c; /* Reusable counters and coordinates */
    char piece;

    /* Direction vectors declared and initialised before the executable code */
    int diagX[4] = {1, 1, -1, -1};
    int diagY[4] = {1, -1, 1, -1};
    int orthoX[4] = {1, -1, 0, 0};
    int orthoY[4] = {0, 0, 1, -1};
    int knightX[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    int knightY[8] = {1, -1, 1, -1, 2, -2, 2, -2};

    /* --------------------------------------------------------- */
    /* 2. Start of the executable statements                     */
    /* --------------------------------------------------------- */

    /* I copy the board and apply the move */
    memcpy(ChessBcopy, ChessB, sizeof(ChessBcopy));
    if (Cur1[0] != Cur2[0] || Cur1[1] != Cur2[1]) { /* same square = no move, nothing to apply */
        ChessBcopy[Cur2[0]][Cur2[1]] = ChessBcopy[Cur1[0]][Cur1[1]];
        ChessBcopy[Cur1[0]][Cur1[1]] = '.'; /* I empty the starting square */
    }

    /* I work out whose turn it is and I look for the right King */
    kingiswhite = (nmove & 1) ? 0 : 1;
    targetKing = kingiswhite ? 'K' : 'k';
    found = 0;
    kingpos[0] = 0;
    kingpos[1] = 0;

    for (i = 0; i < 8 && !found; i++) {
        for (j = 0; j < 8 && !found; j++) {
            if (ChessBcopy[i][j] == targetKing) {
                kingpos[0] = i;
                kingpos[1] = j;
                found = 1;
            }
        }
    }

    if (!found) return 1; /* No king on the board: nothing can be attacked */

    /* Check of the diagonals for Bishops (B/b) and Queens (Q/q) */
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

    /* Check of the straight lines for Rooks (R/r) and Queens (Q/q) */
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

    /* Check of the Knights (N/n) */
    for (i = 0; i < 8; i++) {
        r = kingpos[0] + knightX[i];
        c = kingpos[1] + knightY[i];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            piece = ChessBcopy[r][c];
            if (kingiswhite && piece == 'n') return 0;
            if (!kingiswhite && piece == 'N') return 0;
        }
    }

    /* Check of the Pawns (P/p) */
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

    /* Check of the enemy King (K/k) */
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

    /* If I get this far, it means that nobody is threatening the King */
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

    /* Find an empty square to run an idle check (no-op) through checkcheck */
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

    /* 1. The king must currently be in check.
       If checkcheck returns 1 it means there is NO check, so it cannot be mate. */
    if (checkcheck(ChessB, dummyCur1, dummyCur2)) {
        return 0;
    }

    /* 2. Scan the whole board looking for the pieces of the player to move */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            p = ChessB[i][j];

            isMyPiece = isWhiteTurn ? (p >= 'A' && p <= 'Z') : (p >= 'a' && p <= 'z');

            if (isMyPiece) {
                /* 3. For every piece, try to move it to every square of the board */
                for (targetR = 0; targetR < 8; targetR++) {
                    for (targetC = 0; targetC < 8; targetC++) {
                        cur1[0] = i;
                        cur1[1] = j;
                        cur2[0] = targetR;
                        cur2[1] = targetC;

                        /* 4. Check whether the move is geometrically valid and not onto a friendly piece */
                        if (checkmove(ChessB, cur1, cur2)) {

                            /* 5. Simulate the move and run checkcheck.
                               If checkcheck returns 1 the move has saved the king -> it is not mate. */
                            if (checkcheck(ChessB, cur1, cur2)) {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    /* 6. If the loop ends without finding any legal saving move, it is checkmate. */
    return 1;
}

void clearterm1(void) { //clears the screen

    #ifdef _WIN32
        system("cls");
        return;

    #else
        system("clear");
        return;

    #endif
}

/*************************************************************************************************************** */

void clearterm2(void) { //brings the cursor back to the top left corner

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

int localmultiplayer(void) {
    int endgame[3] = {1,0,0};
    int i,j,x,t;
    int Cur1[2] = {7,0};
    int Cur2[2] = {7,0};
    int is_selected;
    int whiteWins;
    char input, firstC=0;
    //I define the chessboard and I place the pieces on it
    char ChessB[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}, // Row 0: black pieces
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, // Row 1: black pawns
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Row 2: empty
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Row 3: empty
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Row 4: empty
    {'.', '.', '.', '.', '.', '.', '.', '.'}, // Row 5: empty
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}, // Row 6: white pawns
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}  // Row 7: white pieces
    };

    clearterm1();

    while(1) {

        clearterm2();

        /*
        //temporary function, prints the board
        for(i=0;i<8;i++) {

            printf("\t");
            for(j=0;j<8;j++) {
                printf("| %c |", ChessB[i][j]);
            }
            printf("\n");

        }
        printf("\n");
        //temporary function, prints the two cursors
        for(i=0;i<8;i++) {

            printf("\t");
            for(j=0;j<8;j++) {
                if(i==Cur1[0] && j==Cur1[1])        printf("| 1 |");
                else if(i==Cur2[0] && j==Cur2[1])   printf("| 2 |");
                else                                printf("| _ |");
            }
            printf("\n");

        }
        */
        //////////////////////////////////// BIG CHESSBOARD PRINTING LOOP /////////////////////////////////////

        printf("    -----------------------------------------------------------------\n");
        for(i = 0; i < 8; i++) {
            for(x = 0; x < 5; x++) {
                printf("    ");
                for(j = 0; j < 8; j++) {
                    printf("|");

                    // Works out whether the current cell is selected by one of the cursors
                    is_selected = (Cur1[0] == i && Cur1[1] == j) || (Cur2[0] == i && Cur2[1] == j);

                    switch (ChessB[i][j]) {
                        case 'p': printf("%s", is_selected ? PawnBC[x] : PawnB[x]); break;
                        case 'r': printf("%s", is_selected ? RookBC[x] : RookB[x]); break;
                        case 'n': printf("%s", is_selected ? KnightBC[x] : KnightB[x]); break;
                        case 'b': printf("%s", is_selected ? BishopBC[x] : BishopB[x]); break;
                        case 'q': printf("%s", is_selected ? QueenBC[x] : QueenB[x]); break;
                        case 'k': printf("%s", is_selected ? KingBC[x] : KingB[x]); break;
                        case 'P': printf("%s", is_selected ? PawnWC[x] : PawnW[x]); break;
                        case 'R': printf("%s", is_selected ? RookWC[x] : RookW[x]); break;
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

        //I check for the checkmate
        if( checkcheckmate(ChessB) ) {
            //the side to move is mated, so the other one wins: I read the winner BEFORE resetting nmove
            whiteWins = (nmove & 1);
            //I reset the variable for the next game
            nmove=0;
            clearterm1();
            while(1) {

                clearterm2();
                printf("\n\n\n\n");
                if(whiteWins) printf("%s", CheckMateBannerW);
                else printf("%s", CheckMateBannerB);

                if(endgame[0])    printf("%s\n", postmate1);
                else if(endgame[1])    printf("%s\n", postmate2);
                else if(endgame[2])    printf("%s\n", postmate3);
                input = readinput();

                if (input == 's' || input == 'S' || input == 'a' || input == 'A') {
                    t = endgame[2];
                    for (i = 1; i >= 0; i--) {
                        endgame[i + 1] = endgame[i];
                    }
                    endgame[0] = t;
                }
                else if (input == 'w' || input == 'W' || input == 'd' || input == 'D') {
                    t = endgame[0];
                    for (i = 0; i <= 1; i++) {
                        endgame[i] = endgame[i + 1];
                    }
                    endgame[2] = t;
                }
                else if (input == '\n' || input == '\r') {
                    if (endgame[0]) return 1;
                    if (endgame[1]) return 2;
                    if (endgame[2]) return 0;
                    return 0;
                }

            }

        }

        input = readinput();

        if(input=='q' || input=='Q') { //exit condition of the game mode
            return 0;
        }

        //the cursors hold the coordinates of the selected cell, they are updated on every move
        if(!firstC) {//I modify the first cursor

            if((input=='w' || input=='W') && Cur1[0] > 0) Cur1[0]--;
            else if((input=='s' || input=='S') && Cur1[0] < 7) Cur1[0]++;
            else if((input=='d' || input=='D') && Cur1[1] < 7)   Cur1[1]++;
            else if((input=='a' || input=='A') && Cur1[1] > 0)   Cur1[1]--;
            else if(input=='\r' || input=='\n')     firstC++;
            Cur2[0]=Cur1[0];    Cur2[1]=Cur1[1]; //I automatically update the second cursor too

        }
        else {//I modify the second cursor

            if((input=='w' || input=='W') && Cur2[0] > 0) Cur2[0]--;
            else if((input=='s' || input=='S') && Cur2[0] < 7) Cur2[0]++;
            else if((input=='d' || input=='D') && Cur2[1] < 7)   Cur2[1]++;
            else if((input=='a' || input=='A') && Cur2[1] > 0)   Cur2[1]--;
            else if(input=='\r' || input=='\n')     {
                /*I check that the move is valid*/
                if(checkmove(ChessB, Cur1, Cur2) && checkcheck(ChessB, Cur1, Cur2)) firstC=movepiece(ChessB, Cur1, Cur2);
                else firstC=0;
            }

        }

    }

}

/************************************************************************************************************************ */

    void challengebot(void) {

        clearterm1();
        printf("challenge bot");
        getchar();
        return;

    }

/********************************************************************************************************************* */

    void learn(void) {

        clearterm1();
        printf("learn");
        getchar();
        return;

    }