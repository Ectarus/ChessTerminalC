#include <stdio.h>

#include "localmp.h"

#include "homebanner.h"

int main(void) {

    int i, temp;
    char input='z', t;
    char Selection[3] = {1,0,0};

    //HOME SCREEN
    startmenu:
    while(1) {
        //I clear the screen
        clearterm1();

        //print up banner
        printf("%s\n", HomeBanner);

        //print game modes
        printf("%s\n", PreCh);

        //print cursor
        if(Selection[0])    printf("%s\n", Print1Ch);
        else if(Selection[1])    printf("%s\n", Print2Ch);
        else if(Selection[2])    printf("%s\n", Print3Ch);

        //input reading
        input = readinput();

        //circular shift on Array
        if(input=='w' || input=='W' || input=='d' || input=='D') {

            t=Selection[2];
            for(i=1; i>=0; i--)    Selection[i+1]=Selection[i];
            Selection[0]=t;

        }
        else if(input=='s' || input=='S' || input=='A' || input=='a') {

            t=Selection[0];
            for(i=0; i<=1; i++) Selection[i]=Selection[i+1];
            Selection[2]=t;

        }
        else if(input=='q' || input=='Q') return 0;
        else if(input=='\n' || input=='\r')    {

            if(Selection[0]) {
                startlocalmp:
                temp = localmultiplayer();
                if(temp==1)  goto startlocalmp;
                else if(temp==2)  goto startmenu;
                else return 0;
            }
            else if(Selection[1])   challengebot();
            else if(Selection[2])   learn();

            return 0;
        }
    }


    return 0;
}