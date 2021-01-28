#include <stdint.h>
#include "easypdk/pfs173.h"
#include "easypdk/pdkcommon.h"
#include "serial.h"
#include "interpreter.h"
#include "delay.h"

#define TEST_PROGRAM_
#define TEST_OUTPUT_
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

int8_t ret[4] = {0, 0, 0, 0};

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
                        if (prog_ready) handle_interpreter_status_request(ret);
                        for (int i=0; i<4; i++) {
                            serial_printhex(ret[i]);
                        }
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

#ifdef TEST_PROGRAM

        program_buf[0] = 0x08;
        program_buf[1] = 0x00;
        program_buf[2] = 0x1F;
        program_buf[3] = 0x40;

        program_buf[4] = 0x08;
        program_buf[5] = 0xC7;
        program_buf[6] = 0xFF;
        program_buf[7] = 0x50;

        program_buf[8] = 0x10;
        program_buf[9] = 0x00;
        program_buf[10] = 0x09;

        program_buf[11] = 0x08;
        program_buf[12] = 0x00;
        program_buf[13] = 0x00;
        program_buf[14] = 0xC0;

        program_buf[15] = 0x08;
        program_buf[16] = 0xC7;
        program_buf[17] = 0xFF;
        program_buf[18] = 0x50;

        program_buf[19] = 0x10;
        program_buf[20] = 0x00;
        program_buf[21] = 0x09;

        program_buf[22] = 0x08;
        program_buf[23] = 0x00;
        program_buf[24] = 0x3F;
        program_buf[25] = 0xC0;

        program_buf_pos = 26;
        set_program(program_buf, program_buf_pos);
        state = prog_ready;
#endif

        delay_ms(100);

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

unsigned char _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000,5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
