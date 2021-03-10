#ifndef XBUS_H
#define XBUS_H

#include <stdint.h>
#include "pfs173.h"
#include "delay.h"

#define XBUS0_PIN 7
#define XBUS1_PIN 6

#define XBUS_WAIT 0x7FFF

#define XBUS_IDLE 0x00

#define XBUS0_SL 0x01
#define XBUS0_TX_READY 0x02
#define XBUS0_TX_START 0x03
#define XBUS0_TX 0x04
#define XBUS0_RX_READY 0x06
#define XBUS0_RX_START 0x07
#define XBUS0_RX 0x08
#define XBUS0_GOT_DATA 0x09

#define XBUS1_SL 0x10
#define XBUS1_TX_READY 0x20
#define XBUS1_TX_START 0x30
#define XBUS1_TX 0x40
#define XBUS1_RX_READY 0x60
#define XBUS1_RX_START 0x70
#define XBUS1_RX 0x80
#define XBUS1_GOT_DATA 0x90

#define XBUS_BITTIME 16U
#define XBUS_DELAY XBUS_BITTIME/2U

uint16_t xbus_data = 0;
uint8_t xbus_bitcounter = 0;
uint8_t xbus_state = 0;

inline void reset_xbus() {
    xbus_data = 0;
    xbus_bitcounter = 0;
    xbus_state = XBUS_IDLE;

    PA &= ~(1 << XBUS0_PIN); // Set low
    PA &= ~(1 << XBUS1_PIN); // Set low

    PAC &= ~(1 << XBUS0_PIN); // Set pin as input
    PAC &= ~(1 << XBUS1_PIN); // Set pin as input

    PAPH &= ~(1 << XBUS0_PIN); // Disable pullup
    PAPH &= ~(1 << XBUS1_PIN); // Disable pullup
}


inline void setup_xbus_hardware() {
    PAC &= ~(1 << XBUS0_PIN); // Enable X0 Pin as input
    PAPH &= ~(1 << XBUS0_PIN); // Disable X0 pullup

    reset_xbus();
}

inline int16_t get_x0_value() {
    if (xbus_state == XBUS0_GOT_DATA) {
        xbus_state = XBUS_IDLE;
        return xbus_data - 1000;
    } else {
        xbus_state = XBUS0_RX_READY;
        PAPH &= ~(1 << XBUS0_PIN); // Disable pullup
        PAC &= ~(1 << XBUS0_PIN); // Set pin as input
        xbus_data = 0;
        xbus_bitcounter = 0;
        return XBUS_WAIT;
    }
}

inline int16_t get_x1_value() {
    if (xbus_state == XBUS1_GOT_DATA) {
        xbus_state = XBUS_IDLE;
        return xbus_data - 1000;
    } else {
        // get data from xbus 1
        xbus_state = XBUS1_RX_READY;
        PAPH &= ~(1 << XBUS1_PIN); // Disable pullup
        PAC &= ~(1 << XBUS1_PIN); // Set pin as input
        xbus_data = 0;
        xbus_bitcounter = 0;
        return XBUS_WAIT;
    }
}

inline void set_x0_value(int16_t arg) {
    xbus_state = XBUS0_TX_READY;
    PAC &= ~(1 << XBUS0_PIN); // Set pin as input
    PAPH |= (1 << XBUS0_PIN); // Enable pullup
    xbus_data = arg + 1000;
    xbus_bitcounter = 0;
}

inline void set_x1_value(int16_t arg) {
    xbus_state = XBUS1_TX_READY;
    PAC &= ~(1 << XBUS1_PIN); // Set pin as input
    PAPH |= (1 << XBUS1_PIN); // Enable pullup
    xbus_data = arg + 1000;
    xbus_bitcounter = 0;
}

uint8_t xbus_handler() {
    switch (xbus_state) {
    case XBUS0_SL:
        if (PA & (1 << XBUS0_PIN)) {
            xbus_state = XBUS_IDLE;
        }
        return 0; // skip futher execution this cycle
        break;
    case XBUS0_TX_READY:
        if (!(PA & (1 << XBUS0_PIN))) {
            PAPH &= ~(1 << XBUS0_PIN); // Disable pullup
            xbus_state = XBUS0_TX_START;
            SLEEP(XBUS_BITTIME);
        };
        return 0; // skip futher execution this cycle
        break;
    case XBUS0_TX_START:
        PAC |= (1 << XBUS0_PIN); // Set pin as output
        xbus_state = XBUS0_TX;
    // Fallthrough!
    case XBUS0_TX:
        // Send one bit and wait XBUS_BITTIME ticks
        if ((xbus_data >> xbus_bitcounter++) & 0x01) __set1(PA, XBUS0_PIN); // Set pin high
        else __set0(PA, XBUS0_PIN); // Set pin low
        if (xbus_bitcounter > 11) { // Transmission done
            PAC &= ~(1 << XBUS0_PIN); // Set pin as input
            xbus_state = XBUS_IDLE;
        };
        SLEEP(XBUS_BITTIME);
        return 0; // skip futher execution this cycle
        break;
    case XBUS0_RX_READY:
        if (PA & (1 << XBUS0_PIN)) {
            PAC |= (1 << XBUS0_PIN); // Set pin as output
            __set0(PA, XBUS0_PIN); // Set pin low
            xbus_state = XBUS0_RX_START;
        }
        SLEEP(XBUS_DELAY);
        return 0; // skip futher execution this cycle
        break;
    case XBUS0_RX_START:
        PAC &= ~(1 << XBUS0_PIN); // Set pin as input
        xbus_state = XBUS0_RX;
        SLEEP(XBUS_BITTIME);
        return 0; // skip futher execution this cycle
        break;
    case XBUS0_RX:
        // Receive one bit and wait XBUS_BITTIME ticks
        if (PA & (1 << XBUS0_PIN)) {
            sleep_until = 1; // Reusing a global var
            xbus_data |= (sleep_until << xbus_bitcounter);
            sleep_until = 0;
        };
        xbus_bitcounter++;
        SLEEP(XBUS_BITTIME);
        if (xbus_bitcounter == 11) {// Transmission done
            xbus_state = XBUS0_GOT_DATA; // set xbus state
            return 1; // roll back program counter
        } else {
            return 0; // skip futher execution this cycle
        }
        break;
    case XBUS1_SL:
        if (PA & (1 << XBUS1_PIN)) {
            xbus_state = XBUS_IDLE;
        }
        return 0; // skip futher execution this cycle
        break;
    case XBUS1_TX_READY:
        if (!(PA & (1 << XBUS1_PIN))) {
            PAPH &= ~(1 << XBUS1_PIN); // Disable pullup
            xbus_state = XBUS1_TX_START;
            SLEEP(XBUS_BITTIME);
        };
        return 0; // skip futher execution this cycle
        break;
    case XBUS1_TX_START:
        PAC |= (1 << XBUS1_PIN); // Set pin as output
        xbus_state = XBUS1_TX;
    // Fallthrough!
    case XBUS1_TX:
        // Send one bit and wait XBUS_BITTIME ticks
        if ((xbus_data >> xbus_bitcounter++) & 0x01) __set1(PA, XBUS1_PIN); // Set pin high
        else __set0(PA, XBUS1_PIN); // Set pin low
        if (xbus_bitcounter > 11) { // Transmission done
            PAC &= ~(1 << XBUS1_PIN); // Set pin as input
            xbus_state = XBUS_IDLE;
        };
        SLEEP(XBUS_BITTIME);
        return 0; // skip futher execution this cycle
        break;
    case XBUS1_RX_READY:
        if (PA & (1 << XBUS1_PIN)) {
            PAC |= (1 << XBUS1_PIN); // Set pin as output
            __set0(PA, XBUS1_PIN); // Set pin low
            xbus_state = XBUS1_RX_START;
        }
        SLEEP(XBUS_DELAY);
        return 0; // skip futher execution this cycle
        break;
    case XBUS1_RX_START:
        PAC &= ~(1 << XBUS1_PIN); // Set pin as input
        xbus_state = XBUS1_RX;
        SLEEP(XBUS_BITTIME);
        return 0; // skip futher execution this cycle
        break;
    case XBUS1_RX:
        // Receive one bit and wait XBUS_BITTIME ticks
        if (PA & (1 << XBUS1_PIN)) {
            sleep_until = 1; // Reusing a global var
            xbus_data |= (sleep_until << xbus_bitcounter);
            sleep_until = 0;
        };
        xbus_bitcounter++;
        SLEEP(XBUS_BITTIME);
        if (xbus_bitcounter == 11) {// Transmission done
            xbus_state = XBUS1_GOT_DATA; // set xbus state
            return 1; // roll back program counter
        } else {
            return 0; // skip futher execution this cycle
        }
        break;
    }
    return 2; // no action
}


#endif // XBUS_H
