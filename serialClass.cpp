#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

class serial_interface{
    protected:
        int serial_port;
        struct termios tty;
	    char writeBuf = 0b00110101;
    public:
        //No default constructer at the moment
        void setup();
        void serial_write();
        ~serial_interface() { close(serial_port); }
};

void serial_interface::setup(){
    serial_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(serial_port < 0){
        cout << "Error opening serialport";
    }else{
        fcntl(serial_port, F_SETFL, 0);
    }

    tcgetattr(serial_port, &tty);

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

    //Hangup on last close (Drop DTR)
    tty.c_cflag &= ~HUPCL; //Disable    --Stops AtMega from resetting on serial

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

    tcsetattr(serial_port, TCSANOW, &tty);
}

void serial_interface::serial_write(){
	write(serial_port, &writeBuf, 1);
}

