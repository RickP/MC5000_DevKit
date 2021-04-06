#include <stdint.h>
#include "pfs173.h"
#include "pdkcommon.h"
#include "delay.h"
#include "xbus.h"

#define DUTY_CYCLE 5

const uint8_t tones[76] = {
    0,
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV14, // 70
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV13, // 75
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV12, // 81
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV11, // 89
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV10, // 98
    TM2S_PRESCALE_DIV64 | TM2S_SCALE_DIV9,  // 108

    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV32, // 122
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV31, // 126
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV30, // 130
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV29, // 134
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV28, // 139
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV27, // 144
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV26, // 150
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV25, // 156
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV24, // 163
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV23, // 170
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV22, // 177
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV21, // 186
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV20, // 195
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV19, // 205
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV18, // 217
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV17, // 229
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV16, // 244
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV15, // 260
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV14, // 279
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV13, // 300
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV12, // 325
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV11, // 355
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV10, // 390
    TM2S_PRESCALE_DIV16 | TM2S_SCALE_DIV9,  // 433

    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV32,  // 488
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV31,  // 503
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV30,  // 520
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV29,  // 538
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV28,  // 557
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV27,  // 578
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV26,  // 600
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV25,  // 624
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV24,  // 650
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV23,  // 678
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV22,  // 709
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV21,  // 743
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV20,  // 780
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV19,  // 821
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV18,  // 867
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV17,  // 918
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV16,  // 975
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV15,  // 1040
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV14,  // 1114
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV13,  // 1200
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV12,  // 1300
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV11,  // 1418
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV10,  // 1560
    TM2S_PRESCALE_DIV4 | TM2S_SCALE_DIV9,   // 1733

    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV32,  // 1950
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV31,  // 2013
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV30,  // 2080
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV29,  // 2152
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV28,  // 2229
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV27,  // 2311
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV26,  // 2400
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV25,  // 2496
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV24,  // 2600
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV23,  // 2713
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV22,  // 2836
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV21,  // 2971
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV20,  // 3120
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV19,  // 3284
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV18,  // 3467
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV17,  // 3671
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV16,  // 3900
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV15,  // 4160
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV14,  // 4457
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV13,  // 4800
    TM2S_PRESCALE_NONE | TM2S_SCALE_DIV12,  // 5200
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
        TM2B = DUTY_CYCLE;

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
            sleep_until = 0;
            TM2C = 0;

            switch (xbus_handler()) {
                 case 1:
                    playval = get_x1_value();
                    uint8_t length = (playval/100)+1;
                    if (length < 10) {
                        playval -= (playval/100*100);
                        if (playval <= 0) {
                            playval = 0;
                        }
                        if (playval > 75) {
                            playval = 75;
                        }
                        TM2S = tones[playval];
                        if (playval) TM2C = TM2C_CLK_IHRC | TM2C_OUT_PA3 | TM2C_MODE_PWM;
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
