#include <stdint.h>
#include "pfs173.h"
#include "pdkcommon.h"
#include "delay.h"
#include "xbus.h"

const uint8_t tones[36] = {
    0,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV32,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV31,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV30,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV29,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV28,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV27,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV26,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV25,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV24,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV23,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV22,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV21,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV20,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV19,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV18,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV17,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV16,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV15,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV14,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV13,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV12,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV11,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV10,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV9,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV8,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV7,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV6,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV5,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV4,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV3,
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV2,
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV7,
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV6,
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV5,
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV4
};

int16_t playval = 0;

void interrupt_routine() __interrupt(0) {
        if (INTRQ & INTRQ_TM3) {  // TM3 interrupt request?
                CLOCK_TICK;
                INTRQ &= ~INTRQ_TM3;
        }
}


int main(void) {
        // Set up padier register (can't be set seperately due to a hardware bug?)
        PADIER = (1U << XBUS0_PIN) | (1U << XBUS1_PIN);

        // Initialize hardware
        setup_xbus_hardware();

        // Set up Timer2 PWM
        PAC |= (1 << 3); // Enable P0 Pin as output
        TM2C = TM2C_CLK_SYSCLK | TM2C_OUT_PA3 | TM2C_MODE_PWM;
        TM2B = 5;

        // Set up timer 3 for systick
        TM3C = TM3C_CLK_IHRC; // Use IHRC -> 8 Mhz
        TM3S = TM3S_PRESCALE_DIV16 | TM3S_SCALE_DIV4;
        TM3B = 180;
        INTEN |= INTEN_TM3; // Enable TM3 interrupt

        INTRQ = 0;
        __engint();                 // Enable global interrupts

        // Main processing loop
        while (1) {
            // Sleep if we need to
            if (clock_tick < sleep_until) {
                continue;
            }
            SLEEP(0);
            TM2S = 0;

            switch (xbus_handler()) {
                 case 1:
                    playval = get_x1_value();
                    uint8_t length = (playval/100)+1;
                    if (length < 10) {
                        playval -= (playval/100*100);
                        if (playval < 0) {
                            playval = 0;
                        }
                        if (playval > 36) {
                            playval = 35;
                        }
                        TM2S = tones[playval];
                        SLEEP(length * SLEEP_TICKS);
                    }
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
