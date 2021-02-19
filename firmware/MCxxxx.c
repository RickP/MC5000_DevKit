#include <stdint.h>
#include "lib/pfs173.h"
#include "lib/pdkcommon.h"
#include "lib/serial.h"
#include "lib/interpreter.h"
#include "lib/delay.h"

#define DEBUG_

// Insert serial number
EASY_PDK_SERIAL(serial_number);

#define PROGSIZE 75

#define START_CHAR 0x7F
#define END_CHAR 0x7E

uint8_t program_buf[PROGSIZE] = {0};
uint8_t program_buf_pos = 0;

typedef enum {
        faulty_prog,
        empty_prog,
        transmission_start,
        retransmission_start,
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
        if (INTRQ & INTEN_ADC) {
            INTRQ &= ~INTEN_ADC;
        }
}

void handle_rx() {
        uint8_t rx_char;
        if (process_serial_rx_byte(&rx_char)) {

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
                        putchar((acc_register+1000) >> 7);
                        putchar((acc_register+1000) & 0x7F);
                        putchar((dat_register+1000) >> 7);
                        putchar((dat_register+1000) & 0x7F);
                        putchar(state == prog_ready ? 0x01 : 0x00);
                } else if (state == transmission_start || state == retransmission_start) {
                        if (rx_char == *serial_number) {
                                program_buf_pos = 0;
                                reset_program();
                                state = line_prog;
                        } else {
                            if (state == transmission_start) {
                                program_buf_pos = 0;
                                reset_program();
                                state = empty_prog;
                            } else {
                                program_buf_pos = 0;
                                state = prog_ready;
                            }
                        }
                } else if (state == line_prog) {
                        if (rx_char == END_CHAR) {
                                if (program_buf_pos > 2) {
                                        state = prog_ready;
                                        set_program(program_buf, program_buf_pos);
                                } else {
                                        program_buf_pos = 0;
                                        reset_program();
                                        state = empty_prog;
                                }
                        } else {
                                program_buf[program_buf_pos++] = rx_char;
                        }
                } else if (rx_char == START_CHAR) { // Start char received
                    if (state == prog_ready) state = retransmission_start;
                    else state = transmission_start;
                }
        }
}

int main(void) {
        // Set up padier register (can't be set seperately due to a hardware bug?)
        PADIER = (1U << SERIAL_RX_PIN) | (1U << X0_PIN) | (1U << X1_PIN);

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
