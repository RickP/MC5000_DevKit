#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>
#include "pfs173.h"

// Serial TX is on PA7
#define SERIAL_TX_PIN       5
#define SERIAL_RX_PIN       0

#define TX_BYTE_CLK_COUNTS 206
#define RX_BYTE_CLK_COUNTS 180
#define RX_INTERVAL 12
#define CNT_BUF_MAX 11

volatile uint8_t cnt_buf[CNT_BUF_MAX];
volatile uint16_t rxdata = 0;  // Serial data bit array
volatile uint16_t txdata = 0; // Serial data shift register
volatile uint16_t bit_counter = 0;  // bit counter
volatile uint8_t byte_needs_processing = 0;

const uint8_t hex_lookup[] = "0123456789ABCDEF";

void serial_setup() {

    INTEGS = INTEGS_PA0_BOTH; //Trigger PA0 interrupt on both edges
    INTEN |= INTEN_PA0; //Enable PA0 interrupt

    // Setup timer2 (TM2) interrupt for 19200 baud tx
    TM2C = TM2C_CLK_IHRC; // Use IHRC -> 8 Mhz
    TM2S = TM3S_PRESCALE_NONE | TM2S_SCALE_DIV4; // No prescale, scale 8 ~> 1MHz
    TM2B = TX_BYTE_CLK_COUNTS; // Divide by 138 - serial triggered every 5 clock interrupts ~> 19417 Hz (apx. 19200 baud)

    // Setup timer3 (TM3) interrupt for 19200 baud rx
    TM3C = TM3C_CLK_IHRC; // Use IHRC -> 8 Mhz
    TM3S = TM3S_PRESCALE_DIV16 | TM3S_SCALE_DIV4; // No prescale, scale 8 ~> 1MHz
    TM3B = RX_BYTE_CLK_COUNTS; // Divide by 138 - serial triggered every 5 clock interrupts ~> 19417 Hz (apx. 19200 baud)

    PAC |= (1 << SERIAL_TX_PIN); // Enable TX Pin as output
    PAPH |= (1 << SERIAL_TX_PIN); // Enable pullup on TX Pin
    __set1(PA, SERIAL_TX_PIN); // Make TX pin High Z
    // txdata = 0xD55F; // Setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
    INTEN |= INTEN_TM2; // Enable TM2 interrupt, send out initial stop bits and autobaud char
    INTEN |= INTEN_TM3; // Enable TM3 interrupt

}

uint8_t process_serial_rx_byte(uint8_t *c) {
    uint8_t ret = 0;
    if (byte_needs_processing && rxdata) {
        *c = 0;

        for (uint8_t i = 2; i < CNT_BUF_MAX; ++i) {
            uint8_t bit = (rxdata & (1 << i)) !=0;

            for (uint8_t j = 0; j < cnt_buf[i]/RX_INTERVAL; ++j) {
                *c = (*c >> 1);
                if (bit == 0) *c |= 0x80;
            }
        }

        rxdata = 0;
        for (uint8_t i = 0; i < CNT_BUF_MAX; ++i) cnt_buf[i] = 0;
        byte_needs_processing = 0;
        ret = 1;
    }
    return ret;
}

void serial_tx_irq_handler() {
    if (txdata) { // Does txdata contains bits to send?
        if (txdata & 0x01) // Check bit (1/0) for sending
            __set1(PA, SERIAL_TX_PIN); // Send 1 on TX Pin
        else
            __set0(PA, SERIAL_TX_PIN); // Send 0 on TX Pin
        txdata >>= 1; // Shift
    }
}

void serial_rx_irq_handler() {
    if (bit_counter) {
        byte_needs_processing = 1;
        bit_counter = 0;
    }
}

void serial_rx_pin_irq_handler() {
    bit_counter++;
    if(bit_counter>=CNT_BUF_MAX) {
        bit_counter = 0;
    }
    cnt_buf[bit_counter] = TM3CT;
    TM3CT = 0;
    if(PA & (1 << SERIAL_RX_PIN))
        rxdata |= 1 << bit_counter;
}

void putchar(uint8_t c)
{
    while (txdata); // Wait for completion of previous transmission
    uint16_t tx = (c << 1) | 0x200;
    INTEN &= ~INTEN_TM2; // Disable TM2 (setup of 16 bit value txdata is non atomic)
    txdata = tx; // Setup txdata with start and stop bit
    INTEN |= INTEN_TM2; // Enable TM2
}

#endif //__SERIAL_H__
