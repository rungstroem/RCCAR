#include <iostream>
#include <stdio.h>
using namespace std;
int main(){
    //This open the ḱeyboard file. Use this file to check for inputs. Maybe
    //allow 2 inputs at the same time "fx. left and forward" to move and turn
    //at the same time
    FILE *kbd = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");
    
    while(1){   //Game-loop

    }

    return 0;
}
