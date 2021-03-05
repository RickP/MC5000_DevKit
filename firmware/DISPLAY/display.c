#include <stdint.h>
#include "pfs173.h"
#include "pdkcommon.h"
#include "delay.h"
#include "xbus.h"


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

        INTRQ = 0;
        __engint();                 // Enable global interrupts

        // Main processing loop
        while (1) {
        }
}

uint8_t _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000, 5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
