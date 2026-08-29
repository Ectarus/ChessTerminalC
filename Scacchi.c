#include <stdio.h>

#include "LibChessT.h"

#include "HomeBanner.h"

int main() {

    int i;
    char input='z', t;
    char Selection[3] = {1,0,0};

    //SCHERMATA HOME
    while(1) {
        //pulisco lo schermo
        clearterm1();
        
        //print up bunner
        printf("%s\n", HomeBanner);
        
        //print mode game
        printf("%s\n", PreCh);

        //print cursor
        if(Selection[0])    printf("%s\n", Print1Ch);
        else if(Selection[1])    printf("%s\n", Print2Ch);
        else if(Selection[2])    printf("%s\n", Print3Ch);

        //lettura input
        input = readinput();

        //circular shift on Array
        if(input=='w' || input=='W' || input=='d' || input=='D') {

            t=Selection[2];
            for(i=1; i>=0; i--)    Selection[i+1]=Selection[i];
            Selection[0]=t;

        }
        else if(input=='s' || input=='S' || input=='A' || input=='a') {

            t=Selection[0];
            for(i=0; i<=2; i++) Selection[i]=Selection[i+1];
            Selection[i-1]=t;
            
        }
        else if(input=='\n' || input=='\r')    {

            if(Selection[0])    localmultiplayer();
            else if(Selection[1])   challengebot();
            else if(Selection[2])   learn();

            return 0;
        }
    }


    return 0;
}