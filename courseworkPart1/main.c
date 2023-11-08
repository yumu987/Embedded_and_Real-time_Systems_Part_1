#include <msp430.h>

char charList[16];
unsigned int index = 0;
const char *PassList = "PASS\n\r";//\n means the line feed, \r means carriage return
const char *FailList = "Fail\n\r";
const char *DeleteList = "\x20\x08";//x20 means the space, \x08 means backspace
volatile int i;
volatile int delay = 100;
void luhnAlgorithm(char *input)
{
    int sum = 0;
    unsigned int j;

   for(j = 16; j >0 ; j--){
        int data = input[j-1] - '0';
      if(input[j-1]>='0' && input[j-1]<='9'){//make sure the input is number not other char
        if ((j-1) % 2 == 0){
           int tmp = data*2;
           if(tmp >= 10){
               sum += (tmp - 9);
           }
           else{
               sum += tmp;
           }
        }
        else{
            sum += data;
        }
      }
      else{
          sum = 1;
      }
    }

   unsigned int k;
   TA0CTL = MC_1 | ID_0 | TASSEL_1 |TACLR;
   if((sum %10) == 0){
       for(k = 0; k <6;k++){
           while(!(UCA0IFG & UCTXIFG));
           UCA0TXBUF = PassList[k];
           for(i=0;i<delay;i++); //make sure the TX has show char in terminal --delay
       }
      P1OUT |= BIT1;
      TA0CCR0 = 16384;
      while(!(TA0CTL & TAIFG)){}
      TA0CTL &= ~TAIFG; // Clear overflow flag
      P1OUT &= ~BIT1;
      }
   else{
      for(k = 0; k <6;k++){
         while(!(UCA0IFG & UCTXIFG));
         UCA0TXBUF = FailList[k];
         for(i=0;i<delay;i++); //make sure the TX has show char in terminal --delay
        }
      P1OUT |= BIT0;
      TA0CCR0 = 32768;
      while(!(TA0CTL & TAIFG)){}
      TA0CTL &= ~TAIFG; // Clear overflow flag
      P1OUT &= ~BIT0;
      }
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;                           // P1.0 out low
    P1DIR |= BIT1;
    P1OUT &= ~BIT1;                           // P1.1 out low
    // Configure UART pins
    P1SEL0 |= BIT4 | BIT5;                    // set 2-UART pin as second function
    // Configure UART for Power UP default clock rate
    UCA0CTLW0 |= UCSWRST;                     // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // set equal to SMCLK
    // Baud Rate calculation 115200 baud
    UCA0BR0 = 8;                           // 1000000/115200 = 8.68
    UCA0MCTLW = 0xD600;                    // Remainder - 0.68  Table 21.4 & 21.5
    UCA0CTLW0 &= ~UCSWRST;                 // Initialize eUSCI
    UCA0IE |= UCRXIE;                      // Enable USCI_A0 RX interrupt

    __bis_SR_register(LPM0_bits|GIE);     // Enter LPM0 CPU off, SMCLK running
    __no_operation();                     // For debugger

}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(UCA0IV)
        {
            case USCI_NONE: break;
            case USCI_UART_UCRXIFG:
                while(!(UCA0IFG & UCTXIFG)); // Wait for TX buffer to be ready for new data
                char receviedChar = UCA0RXBUF;
                UCA0TXBUF = UCA0RXBUF;
                for(i=0;i<delay;i++); //make sure the TX has show char in terminal --delay
                if(index != 16){    //check the 16 number has been stored in list
                    if (receviedChar == '\x08'){// to detect the delete input
                       unsigned int k;
                       for(k = 0; k <2;k++){
                         while(!(UCA0IFG & UCTXIFG));
                         UCA0TXBUF = DeleteList[k];
                         for(i=0;i<delay;i++); //make sure the TX has show char in terminal --delay
                       }
                       if (index > 0){
                           index -= 1;
                       }else{
                           index = 0;
                       }
                    }else{
                       charList[index] = receviedChar;  //update the list
                       index++;
                    }
                }
                if(index == 16){
                    luhnAlgorithm(charList);
                    index = 0;
                }
                break;
            case USCI_UART_UCTXIFG: break;
            case USCI_UART_UCSTTIFG: break;
            case USCI_UART_UCTXCPTIFG: break;
        }

}
