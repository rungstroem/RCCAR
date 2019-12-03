#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESC ((F_CPU/(16UL*BAUDRATE))-1)

#include <avr/io.h>

// Timer output pins 
/* Timer0 OC0A-PD6 OC0B-PD5   TCCR0A-OCR0A for OC0A, TCCR0B-OCR0B for OC0B*/
/* Timer1 OC1A-PB1 OC1B-PB2   TCCR1A-OCR1A for OC1A, TCCR1B-OCR1B for OC1B*/
/* Timer2 OC2A-PB3 OC2B-PD3   TCCR2A-OCR2A for OC2A, TCCR2B-OCR2B for OC2B*/

unsigned char UARTREAD[2] = {0,0};
short int UARTREADCOUNT = 0;

//Interrupt service routine for USART receive
ISR(USART_RX_vect){
  UARTREAD[UARTREADCOUNT] = UDR0;
  UARTREADCOUNT++;
  if(UARTREADCOUNT > 1) UARTREADCOUNT = 0;
}

//Interrupt service routine for timer 1 overflow - overflow every 0.95s
ISR(TIMER1_OVF_vect){
  PORTB ^= (1<<5);  //I'm alive led
}


int main(){

  //Serial setup - 8DataBits, 0 parity, Baudrate 9800, interrupt enabled
  UCSR0A = 0b00000000;
  UCSR0B = 0b11011000; //UCSR0B = (1<<7)|(1<<6)|(1<<4)|(1<<3);
  UCSR0C = 0b00000110; //UCSR0C = (1<<2)|(1<<1);
  UBRR0H = (unsigned char)(BAUD_PRESC >> 8);
  UBRR0L = (unsigned char)(BAUD_PRESC);
  
  //Pin setup
  DDRD |= (1<<6); //D6 output - OCR0A
  DDRD |= (1<<5); //D5 output - OCR0B
  DDRD |= (1<<3); //D3 output - OCR2B
  DDRB |= (1<<3); //B3 output - OCR2A
  DDRB |= (1<<5); //output with led for im alive
  DDRB |= (1<<4); //output for headlights
  
  //Timer setup
  //Timet 0
  /* TCCR0A 0b0000xx00 */
  TCCR0A |= 0b10100001; //OC0A & OC0B clear on match + Phase correct mode.
  /* TCCR0B 0b00xx0000 */
  TCCR0B |= 0b00000001; //Further phase correct mode + no prescaler, internal clock selected
  //Timer 2 - Same setup as timer0
  TCCR2A |= (1<<COM2A1)|(1<<COM2B1)|(1<<WGM20);
  TCCR2B |= (1<<CS20);  //No prescaler
  //Timer 1
  TCCR1A = 0b00000000;
  TCCR1B |= (1<<CS12);  //256 prescaler
  TIMSK1 |= (1<<0); //interrupt on overflow enabled
  
  //Enable global interrupt
  sei();

  //Variable for headlights toggle
  bool isSet = false;
  
  while(1){
    //UARTREAD[0] enables if = 1 - forward, 2 - backward enable, 3 - left, 4 - right, 5 - headlights 
    
    //Everything is transmittet as ASCII - fix!
    switch(UARTREAD[0]){
      case '1':
        OCR0A = 0;
        if(OCR0B != UARTREAD[1]){
          OCR0B = UARTREAD[1];  
        }
        UDR0 = UARTREAD[1]; //This shows that the internal crystal is not accurate
        break;
      case '2' :
        OCR0B = 0;
        if(OCR0A != UARTREAD[1]){
          OCR0A = UARTREAD[1];  
        }
        break;
      case '3' :
        OCR2A = 0;
        if(OCR2B != UARTREAD[1]){
          OCR2B = UARTREAD[1]; 
        }
        break;
      case '4' :
        OCR2B = 0;
        if(OCR2A != UARTREAD[1]){
          OCR2A = UARTREAD[1]; 
        }
        break;
      case '5' :
        PORTB ^= 0b00010000;
        UARTREAD[0] = 0;
        UARTREAD[1] = 0;
        break;
      default :
        OCR0A = 0, OCR0B = 0, OCR2A = 0, OCR2B = 0;
        break;
    }
  }
  
  return 0;
}
