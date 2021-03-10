#include <stdint.h>
#include "pfs173.h"
#include "pdkcommon.h"
#include "delay.h"
#include "xbus.h"

#define SDA_PIN 4
#define SCL_PIN 3
#define I2C_DELAY 0

const uint8_t segment_numbers[10] = {
    0x03,
    0x9F,
    0x25,
    0x0D,
    0x99,
    0x49,
    0x41,
    0x1F,
    0x01,
    0x09
};

int16_t xval;

uint8_t is_negative = 0;
uint8_t current_numbers[3] = {0,0,0};
uint16_t multiplex_counter = 0;
uint8_t digit = 0;

void interrupt_routine() __interrupt(0) {
        if (INTRQ & INTRQ_TM3) {  // TM3 interrupt request?
                CLOCK_TICK;
                INTRQ &= ~INTRQ_TM3;
        }
}

static void i2cByteOut(uint8_t b)
{
     for (uint8_t i=0; i<8; i++)
     {
         if (b & 0x80)
           __set1(PA, SDA_PIN); // set data line to 1
         else
           __set0(PA, SDA_PIN); // set data line to 0
         __set1(PA, SCL_PIN); // clock high (slave latches data)
         // sleep_us(I2C_DELAY);
         __set0(PA, SCL_PIN); // clock low
         b <<= 1;
     } // for i
     __set1(PA, SDA_PIN);
     __set1(PA, SCL_PIN);
     // sleep_us(I2C_DELAY);
     __set0(PA, SCL_PIN);
     // sleep_us(I2C_DELAY);
     __set0(PA, SDA_PIN);
} /* i2cByteOut() */

static void i2cStop() {
    __set0(PA, SDA_PIN); // data line low
    // sleep_us(I2C_DELAY);
    __set1(PA, SCL_PIN); // clock high
    // sleep_us(I2C_DELAY);
    __set1(PA, SDA_PIN); // data high
    // sleep_us(I2C_DELAY);
}

static void i2cBegin() {
   __set0(PA, SDA_PIN); // data line low first
   // sleep_us(I2C_DELAY);
   __set0(PA, SCL_PIN); // then clock line low is a START signal
} /* i2cBegin() */

int main(void) {
        // Set up padier register (can't be set seperately due to a hardware bug?)
        PADIER = (1U << XBUS0_PIN) | (1U << XBUS1_PIN);

        // Initialize hardware
        setup_xbus_hardware();

        // Set up I2C
        PAC |= (1 << SDA_PIN); // Set pin as output
        PAPH &= ~(1 << SDA_PIN); // Disable pullup
        __set1(PA, SDA_PIN); // Set pin high

        PAC |= (1 << SCL_PIN); // Set pin as output
        PAPH &= ~(1 << SCL_PIN); // Disable pullup
        __set1(PA, SCL_PIN); // Set pin high

        // Set up timer 3 for systick
        TM3C = TM3C_CLK_IHRC; // Use IHRC -> 8 Mhz
        TM3S = TM3S_PRESCALE_DIV16 | TM3S_SCALE_DIV4;
        TM3B = 180;
        INTEN |= INTEN_TM3; // Enable TM3 interrupt

        // Set multiplexer pins to output
        i2cBegin();
        i2cByteOut(0x40);
        i2cByteOut(6);
        i2cByteOut(0);
        i2cByteOut(0);
        i2cStop();

        INTRQ = 0;
        __engint();                 // Enable global interrupts

        // Main processing loop
        while (1) {

            // Multiplex display
            if (++multiplex_counter > 2500) {
                i2cBegin();
                i2cByteOut(0x40);
                i2cByteOut(2);
                i2cByteOut(segment_numbers[current_numbers[digit]]);
                i2cByteOut((0x01 << digit) | (is_negative ? 0x08 : 0x00));
                i2cStop();
                if (++digit > 2) {
                    digit = 0;
                }
                multiplex_counter = 0;
            }


            // Sleep if we need to
            if (clock_tick < sleep_until) {
                continue;
            }
            SLEEP(0);

            switch (xbus_handler()) {
                 case 1:
                    xval = get_x1_value();

                    if (xval < 0) {
                        is_negative = 1;
                        xval = -xval;
                    } else {
                        is_negative = 0;
                    }

                    current_numbers[0] = xval/100L;
                    current_numbers[1] = xval/10L - (current_numbers[0] * 10L);
                    current_numbers[2] = xval - (current_numbers[1]*10L + current_numbers[0]*100L);

                    break;
                 case 2:
                    get_x1_value();
                    break;
            }
        }
}

uint8_t _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000, 5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
