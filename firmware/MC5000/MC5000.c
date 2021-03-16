#include <stdint.h>
#include "pfs173.h"
#include "pdkcommon.h"
#include "serial.h"
#include "interpreter.h"
#include "delay.h"

#define DEBUG_

// Insert serial number
EASY_PDK_SERIAL(serial_number);

#define PROGSIZE 75U

#define START_CHAR 0x7F
#define END_CHAR 0x7E

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
prog_state prev_state;

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
                CLOCK_TICK;
                INTRQ &= ~INTRQ_TM3;
        }
        if (INTRQ & INTEN_ADC) {
            INTRQ &= ~INTEN_ADC;
        }
}

inline uint8_t checksum (uint8_t *ptr, uint8_t size) {
    unsigned char chk = 0;
    while (size-- != 0)
        chk -= *ptr++;
    return chk >> 2;
}

void handle_rx() {

        if (process_serial_rx_byte()) {

#ifdef DEBUG
                switch (state) {
                    case empty_prog:
                        dat_register = 1;
                        break;
                    case line_prog:
                        dat_register = 2;
                        break;
                    case transmission_start:
                        dat_register = 3;
                        break;
                    case retransmission_start:
                        dat_register = 4;
                        break;
                    case faulty_prog:
                        dat_register = 5;
                        break;
                    case prog_ready:
                        dat_register = 6;
                        break;
                }
#endif

                if ((state == prog_ready || state == empty_prog) && rx_char == *serial_number) { // serial num char received
                        uint8_t tx_data[4];
                        putchar(serial_number[0]);
                        tx_data[0] = (acc_register+1000) >> 7;
                        tx_data[1] = (acc_register+1000) & 0x7F;
                        tx_data[2] = (dat_register+1000) >> 7;
                        tx_data[3] = (dat_register+1000) & 0x7F;
                        for (int i=0; i<4; i++) {
                            putchar(tx_data[i]);
                        }
                        if (state == prog_ready) {
                            putchar(checksum(tx_data, 4) | 0x40);
                        } else {
                            putchar(checksum(tx_data, 4));
                        }
                } else if (rx_char == START_CHAR) { // Start char received
                        prev_state = state;
                        state = transmission_start;
                } else if (state == transmission_start) {
                        if (rx_char == *serial_number) {
                                program_buf_pos = 0;
                                state = line_prog;
                        } else {
                                reset_program();
                                state = prev_state;
                        }
                } else if (state == line_prog) {
                        if (rx_char == END_CHAR) {
                                putchar(0x7F); // signal programming done
                                putchar(serial_number[0]);
                                if (program_buf_pos > 2) {
                                        program_buf_pos--;
                                        if (checksum(program_buf, program_buf_pos) == program_buf[program_buf_pos]) {
                                            putchar(1); // programming success
                                            reset_program();
                                            set_program(program_buf, program_buf_pos);
                                            state = prog_ready;
                                        } else {
                                            program_buf_pos = 0;
                                            reset_program();
                                            state = empty_prog;
                                            putchar(0); // checksum fail
                                        }
                                } else {
                                        program_buf_pos = 0;
                                        reset_program();
                                        state = empty_prog;
                                        putchar(1); // programming success
                                }
                        } else {
                                program_buf[program_buf_pos++] = rx_char;
                        }
                }
        }
}

int main(void) {
        // Set up padier register (can't be set seperately due to a hardware bug?)
        PADIER = (1U << SERIAL_RX_PIN) | (1U << XBUS0_PIN) | (1U << XBUS1_PIN);

        // Initialize hardware
        serial_setup();
        setup_interpreter_hardware();

        INTRQ = 0;
        __engint();                 // Enable global interrupts

        // Main processing loop
        while (1) {
                handle_rx();
                if (state == prog_ready) {
                        if (run_program_line()) { // Program failed in interpreter - reset it
                                program_buf_pos = 0;
                                reset_program();
                                state = empty_prog;
                        }
                }
        }
}

uint8_t _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000, 5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
