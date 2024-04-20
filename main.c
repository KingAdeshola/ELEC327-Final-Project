#include <msp430.h>
#include <stdint.h>
#include <string.h> // Include for strlen function

#define sensor_pin BIT3  // Define sensor pin as P1.3

void setup_adc() {
    ADC10CTL1 = INCH_3;  // Select channel 3 (P1.3) for ADC
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; // ADC settings
    ADC10AE0 |= BIT3;  // Enable analog input on P1.3
}

void setup_uart() {
    P1SEL |= BIT1 + BIT2;  // P1.1 = RXD, P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;  // P1.1 = RXD, P1.2 = TXD
    UCA0CTL1 |= UCSWRST + UCSSEL_2;  // SMCLK
    UCA0BR0 = 52;  // 19200 baud rate (for SMCLK at 1 MHz)
    UCA0BR1 = 0;
    UCA0MCTL |= UCBRS_0;  // Modulation UCBRSx = 0
    UCA0CTL1 &= ~UCSWRST;  // Initialize USCI state machine
}

void ser_output(char *str) {
    while (*str != '\0') {
        while (!(IFG2 & UCA0TXIFG));  // Wait for TX buffer to be ready
        UCA0TXBUF = *str++;
    }
}

void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;  // Make n positive
    i = 0;
    do {  // Generate digits in reverse order
        s[i++] = n % 10 + '0';  // Get next digit
    } while ((n /= 10) > 0);
    if (sign < 0) s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) {
    int i, j;
    char c;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    setup_adc();  // Initialize ADC
    setup_uart();  // Initialize UART
    __bis_SR_register(GIE);  // Enable global interrupts
    while (1) {
        ADC10CTL0 |= ENC + ADC10SC;  // Start ADC conversion
        while (ADC10CTL1 & ADC10BUSY);  // Wait for conversion to complete
        int sensor_analog = ADC10MEM;  // Read ADC value
        float moisture_percentage = 100.0 - ((sensor_analog / 1023.0) * 100.0);
        char moisture_str[10];
        itoa((int)moisture_percentage, moisture_str);
        ser_output("Moisture Percentage = ");
        ser_output(moisture_str);
        ser_output("%\r\n");
        __delay_cycles(1000000);  // Delay before next reading (adjust as needed)
    }
}
