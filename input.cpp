#include <iostream>
#include <stdio.h>

/*
* Linux input library
* #include <input.h>
* use EV_KEY
*/

using namespace std;
int main(){
    //This open the ḱeyboard file. Use this file to check for inputs. Maybe
    //allow 2 inputs at the same time "fx. left and forward" to move and turn
    //at the same time
   
    unsigned short c = 0;

    //FILE *kbd = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");    
    FILE *kbd = fopen("/dev/input/event4", "r");    
/*    while(1){   //Game-loop
    
        c = fgetc(kbd);
        if(c != EOF){
            cout << c << "\n";
        }
    }
*/
    while(1){
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);
        c = fgetc(kbd);     //The key-code is here - all above is garbelled data
        if(c != EOF){
            break;
        }
    }
    cout << c;
    return 0;
}
