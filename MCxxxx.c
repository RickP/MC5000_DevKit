//adopted from: https://github.com/joshgerdes/arduino-led-matrix
#include <stdint.h>
#include "easypdk/pdk.h"
#include "easypdk/pdkcommon.h"
#include "serial.h"
#include "interpreter.h"

// Insert serial number
EASY_PDK_SERIAL(serial_number);

#define LED_PIN 4

#define PROGSIZE 100

#define START_CHAR '$'
#define STOP_CHAR '&'

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

void interrupt(void) __interrupt(0) {
        if( INTRQ & INTRQ_PA0 ) {   //PA0 interrupt request?
                serial_rx_pin_irq_handler();
                INTRQ &= ~INTRQ_PA0;
        }
        if (INTRQ & INTRQ_TM3) {  // TM3 interrupt request?
                serial_rx_irq_handler();     // Assemble RXed byte
                INTRQ &= ~INTRQ_TM3;
        }
        if (INTRQ & INTRQ_TM2) {  // TM2 interrupt request?
                serial_tx_irq_handler();     // Process next Serial Bit
                TICK_INTERPRETER_CLOCK;
                INTRQ &= ~INTRQ_TM2;
        }
}

void reset_prog() {
    state = empty_prog;
    program_buf_pos = 0;
}

void handle_rx() {
        uint8_t rx_char;
        if (process_serial_rx_byte(&rx_char)) {
                putchar(rx_char);
                // Start char received
                if (rx_char == START_CHAR) {
                        state = transmission_start;
                        program_buf_pos = 0;
                } else if (state == transmission_start) {
                        if (rx_char == *serial_number) {
                                state = line_prog;
                                PA |= (1 << LED_PIN);
                        } else {
                                reset_prog();
                        }
                } else if (state == line_prog) {
                        if (rx_char == STOP_CHAR) {
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
                }
        }
}

void main(void) {
        // Initialize hardware
        serial_setup();
        PAC |= (1 << LED_PIN);     // Enable LED Pin as output

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

unsigned char _sdcc_external_startup(void) {
        EASY_PDK_INIT_SYSCLOCK_8MHZ();                                          //use 8MHz sysclock
        EASY_PDK_CALIBRATE_IHRC(8000000,5000);                                  //tune SYSCLK to 8MHz @ 5.000V
        return 0;                                                               //perform normal initialization
}
