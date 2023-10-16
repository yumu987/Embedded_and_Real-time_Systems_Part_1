//******************************************************************************
// Original code from
// https://dev.ti.com/tirex/explore/node?node=A__ADHqgHaJKFPoqxRJWM7H4g__com.ti.MSP430_ACADEMY__bo90bso__LATEST
// Modified by Roshan Weerasekera
//******************************************************************************

#include <msp430.h>

const char string[] = { "Hello World\r\n" };
unsigned int i; //Counter

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;                           // P1.0 out low

    // Configure UART pins
    P1SEL0 |= BIT4 | BIT5;                    // set 2-UART pin as second function

    // Configure UART for Power UP default clock rate
    UCA0CTLW0 |= UCSWRST;                     // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // set equal to SMCLK

    // Baud Rate calculation 115200 baud
    UCA0BR0 = 8;                           // 1000000/115200 = 8.68
    UCA0MCTLW = 0xD600;                    // Remainder - 0.68  Table 21.4 & 21.5

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

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
                P1OUT ^= BIT0; // turn off the RED LED
                UCA0TXBUF = '*';
                break;
            case USCI_UART_UCTXIFG: break;
            case USCI_UART_UCSTTIFG: break;
            case USCI_UART_UCTXCPTIFG: break;
        }

}



