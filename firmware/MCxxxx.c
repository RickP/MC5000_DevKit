#include <stdint.h>
#include "lib/pfs173.h"
#include "lib/pdkcommon.h"
#include "lib/serial.h"
#include "lib/interpreter.h"
#include "lib/delay.h"

#if __INTELLISENSE__
#pragma diag_suppress 40
#pragma diag_suppress 79
#pragma diag_suppress 144
#pragma diag_suppress 169
#pragma diag_suppress 411
#endif

// Insert serial number
EASY_PDK_SERIAL(serial_number);

#define PROGSIZE 60

#define SIGNAL_CHAR 0x7F

uint8_t program_buf[PROGSIZE] = {0};
uint8_t program_buf_pos = 0;

typedef enum {
        faulty_prog,
        empty_prog,
        transmission_start,
        line_prog,
        prog_ready
} prog_state;

prog_state state = empty_prog;

void interrupt_routine() __interrupt(0) {
        if (INTRQ & INTRQ_TM2) {  // TM2 interrupt request?
                serial_tx_irq_handler();     // Process next Serial Bit
                INTRQ &= ~INTRQ_TM2;
        }
        if( INTRQ & INTRQ_PA0 ) {   //PA0 interrupt request?
                serial_rx_pin_irq_handler();
                INTRQ &= ~INTRQ_PA0;
        }
        if (INTRQ & INTRQ_TM3) {  // TM3 interrupt request?
                serial_rx_irq_handler();     // Assemble RXed byte
                INTERPRETER_CLOCK_TICK;
                INTRQ &= ~INTRQ_TM3;
        }
}

void reset_prog() {
    state = empty_prog;
    program_buf_pos = 0;
    reset_program();
}

void handle_rx() {
        uint8_t rx_char;
        if (process_serial_rx_byte(&rx_char)) {
                // Start char received
                if ((state == prog_ready || state == empty_prog) && rx_char == *serial_number) {
                        putchar((acc_register+1000) >> 7);
                        putchar((acc_register+1000) & 0x7F);
                        putchar((dat_register+1000) >> 7);
                        putchar((dat_register+1000) & 0x7F);
                } else if (state == transmission_start) {
                        if (rx_char == *serial_number) {
                                state = line_prog;
                        } else {
                                reset_prog();
                        }
                } else if (state == line_prog) {
                        if (rx_char == SIGNAL_CHAR) {
                                if (program_buf_pos > 2) {
                                        state = prog_ready;
                                        set_program(program_buf, program_buf_pos);
                                } else {
                                        reset_prog();
                                }
                        } else {
                                if (program_buf_pos > PROGSIZE-2) {
                                        reset_prog();
                                } else {
                                        program_buf[program_buf_pos++] = rx_char;
                                }
                        }
                } else if (rx_char == SIGNAL_CHAR) {
                    reset_prog();
                    state = transmission_start;
                }
        }
}

void main(void) {
        // Disable wake-up on un-used pins to save power
        PADIER = 0x00;

        // Initialize hardware
        serial_setup();
        setup_interpreter_hardware();

        INTRQ = 0;
        __engint();                 // Enable global interrupts

        // Main processing loop
        while (1) {
                handle_rx();
                if (state == prog_ready) {
                        if (run_program_line()) {
                                reset_prog(); // Program failed in interpreter - reset it
                        }
                }
        }
}

uint8_t _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000, 5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
