#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
int main(){

    int serial_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(serial_port < 0){
        cout << "Error opening serialport";
    }else{
        fcntl(serial_port, F_SETFL, 0);
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if(tcgetattr(serial_port, &tty) != 0){
        cout << "Error reading tcgetattr";
    }

    //Set BAUD-Rate
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    //Enable receiver and set local
    tty.c_cflag |= (CLOCAL | CREAD);

    //Set parity
    tty.c_cflag &= ~PARENB; //No parity

    //Set stop bits
    tty.c_cflag &= ~CSTOPB; //1 stopbit

    //Set character size
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; //8 databits

    //Hardware flow control
    tty.c_cflag &= ~CRTSCTS; //disable

    //Set local control - interpretation of characters
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHONL); //Set to raw data
    
    //Input parity options
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); //Disabled special handling of received bytes
    
    //Software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Disabled

    //Output processed options
    tty.c_oflag &= ~OPOST; //Disabled - raw output
    
    //Set time to wait and bytes to receive
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
        cout << "Error setting tcsetattr";
    }
    char F = '5';
    char FF = '1';
    char NF = '0';
    
    int n = 0;
    n = write(serial_port, &F, 1);
    sleep(1);
    n = write(serial_port, &FF, 1);
    sleep(1);
    n = write(serial_port, &F, 1);
    sleep(1);
    n = write(serial_port, &NF, 1);

    close(serial_port);

    return 0;
}
