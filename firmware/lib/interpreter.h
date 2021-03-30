#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <stdint.h>
#include "pfs173.h"
#include "delay.h"
#include "ppins.h"
#include "xbus.h"

#define GET_RI(x) x = get_val(program[current_pos], program[current_pos+1]); if (x == WAIT_FOR_VALUE) return 0
#define GET_R program[current_pos++]
#define CHECK_CONDITION(x) if (command_condition != none && current_condition != command_condition) {current_pos += (uint8_t) x; break;}

uint8_t *program;
uint8_t program_size = 0;
uint8_t current_pos = 0;
int16_t ri_1 = 0;
int16_t ri_2 = 0;
uint8_t reg = 0;
int16_t acc_register = 0;
int16_t dat_register = 0;


typedef enum {
    false=0,
    true=1,
    none=2
} true_or_false;

true_or_false current_condition;

#define CMD_NOP 0x04
#define CMD_MOV 0x08
#define CMD_JMP 0x0C
#define CMD_SLP 0x10
#define CMD_SLX 0x14
#define CMD_TEQ 0x18
#define CMD_TGT 0x1C
#define CMD_TLT 0x20
#define CMD_TCP 0x24
#define CMD_ADD 0x28
#define CMD_SUB 0x2C
#define CMD_MUL 0x30
#define CMD_NOT 0x34
#define CMD_DGT 0x38
#define CMD_DST 0x3C
#define CMD_LBL 0x40

#define WAIT_FOR_VALUE 0x7FFF

void setup_interpreter_hardware() {

    setup_ppin_hardware();
    setup_xbus_hardware();
}

void reset_program() {
    // Reset registers and outputs
    current_pos = 0;
    clock_tick = 0;
    acc_register = 0;
    dat_register = 0;
    current_condition = none;
    sleep_until = 0;
    ri_1 = 0;
    ri_2 = 0;
    reg = 0;
    set_p0_value(0);
    set_p1_value(0);
    reset_xbus();
}

inline void set_program(uint8_t *new_program, uint8_t new_program_size) {
    program = new_program;
    program_size = new_program_size;
}

uint8_t find_label(uint8_t label) {
    for (uint8_t i = 0; i < program_size; i++) {
        if (program[i] == CMD_LBL) {
            if (program[i+1] == label) return i+2;
        }
    }
    return 0;
}

// Get value for an R/I type parameter
int16_t get_val(uint8_t argh, uint8_t argl) {

    int16_t arg = 0;

    // analyze first byte
    // Bit 1 of argh defines register (1) or scalar (0)
    if (argh & 0x40) {
        // Bit 2 defines memory (1) or pin (0)
        if (argh & 0x20) {
            current_pos += 2;
            // memory -> bit 3 defines acc (1) or dat(0)
            if (argh & 0x10) {
                return acc_register;
            } else {
                return dat_register;
            }
        } else {
            // pin -> bit 3 defines p(1) or x(0)
            // this misuses the xbus_data and xbus_bitcounter var!
            if (argh & 0x10) {
                uint8_t value;
                if (argh & 0x08) {
                    arg = get_p1_value();
                } else {
                    arg = get_p0_value();
                }
                if (arg == PPIN_WAIT) return WAIT_FOR_VALUE;
                current_pos += 2;
                value = arg;
                arg = ((value >> 1) + (value >> 5)) - (value >> 3);
                // Compensate for analog incorrectness
                if (arg > 100) arg = 100;
                return arg;
            } else {
                // x pin -> bit 4 defines port num
                if (argh & 0x08) {
                    arg = get_x1_value();
                } else {
                    arg = get_x0_value();
                }
                if (arg == XBUS_WAIT) return WAIT_FOR_VALUE;
                current_pos += 2;
                return arg;
            }
        }
    }

    // Else this is a scalar value -> assemble the 11bit value
    arg = argh & 0x1F; // Use the last 5 bits of argh
    arg <<= 6; // shift them 6 positions to the left
    arg |= argl & 0x3F; // add the last 6 bits of argl
    arg -= 1000; // create signed integer

    // Limit arg to -999 to 999
    if (arg < -999) arg = -999;
    else if (arg > 999) arg = 999;

    current_pos += 2;
    return arg;
}

void set_val(int16_t arg, uint8_t reg) {

    // Bit 2 of reg defines memory (1) or pin (0)
    if (reg & 0x20) {
        // memory -> bit 3 defines acc (1) or dat(0)
        if (reg & 0x10) {
            acc_register = arg;
        } else {
            dat_register = arg;
        }
    } else {
        // pin -> bit 3 defines p(1) or x(0)
        if (reg & 0x10) {
            // p pin -> bit 4 defines port num
            if (arg < 0) arg = 0; // Limit arg
            else if (arg > 100) arg = 100;

            if (reg & 0x08) {
                set_p1_value((arg & 0xFF));
            } else {
                set_p0_value((arg & 0xFF));
            }
        } else {
            // x pin -> bit 4 defines port num
            if (reg & 0x08) {
                // put data on xbus 1
                set_x1_value(arg);
            } else {
                // put data on xbus 0
                set_x0_value(arg);
            }
        }
    }
}

uint8_t run_program_line() {
    // Sleep if we need to
    if (clock_tick < sleep_until) {
        return 0;
    }
    sleep_until = 0;

    // XBus handling
    uint8_t xbus_result = xbus_handler();
    switch (xbus_result) {
    case 1:
        current_pos -= 1; // rewind program counter
    // Fallthrough!
    case 0:
        return 0;
    }

    // Handle end of program buffer
    if (current_pos >= program_size) {
        current_pos = 0;
    }

    // get current command including current_condition
    uint8_t command = program[current_pos];

    // Get current_condition for command
    true_or_false command_condition = none;
    if (command & 0x01) {
        command_condition = true;
    }
    else if (command & 0x02) {
        command_condition = false;
    }

    // Remove current_condition bits
    command &= 0xFC;

    // Increase programm array counter to next byte
    current_pos++;

    switch (command) {
    case 0:
        return 1; // Command not found - return 1 as failure
    case CMD_NOP: // nop
        break;
    case CMD_MOV: // mov R/I R
        CHECK_CONDITION(3);
        GET_RI(ri_1);
        reg = GET_R;
        set_val(ri_1, reg); // set value to register/pin
        break;
    case CMD_JMP: // jmp L
        CHECK_CONDITION(1);
        reg = GET_R;
        current_pos = find_label(reg); // Set position to after label pos
        break;
    case CMD_SLP: // slp R/I
        CHECK_CONDITION(2);
        GET_RI(ri_1);
        SLEEP(ri_1 * SLEEP_TICKS);
        break;
    case CMD_SLX: // slx P
        CHECK_CONDITION(1);
        reg = GET_R;
        // fist two bits of argument encode the XBus port to use
        if (reg == 0x40) {
            PAC &= ~(1 << XBUS0_PIN); // Set pin as input
            PAPH &= ~(1 << XBUS0_PIN); // Disable pullup
            xbus_state = XBUS0_SL;
        } else {
            PAC &= ~(1 << XBUS1_PIN); // Set pin as input
            PAPH &= ~(1 << XBUS1_PIN); // Disable pullup
            xbus_state = XBUS1_SL;
        }
        SLEEP(XBUS_DELAY);
        break;
    case CMD_TEQ: // teq R/I R/I
        CHECK_CONDITION(4);
        GET_RI(ri_1);
        GET_RI(ri_2);
        if (ri_1 == ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TGT: // tgt R/I R/I
        CHECK_CONDITION(4);
        GET_RI(ri_1);
        GET_RI(ri_2);
        if (ri_1 > ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TLT: // tlt R/I R/I
        CHECK_CONDITION(4);
        GET_RI(ri_1);
        GET_RI(ri_2);
        if (ri_1 < ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TCP: // tcp R/I R/I
        CHECK_CONDITION(4);
        GET_RI(ri_1);
        GET_RI(ri_2);
        if (ri_1 > ri_2) {
            current_condition = true;
        } else if (ri_1 < ri_2) {
            current_condition = false;
        } else {
            current_condition = none;
        }
        break;
    case CMD_ADD: // add R/I
        CHECK_CONDITION(2);
        GET_RI(ri_1);
        acc_register += ri_1;
        if (acc_register > 999) {
            acc_register = 999;
        }
        break;
    case CMD_SUB: // sub R/I
        CHECK_CONDITION(2);
        GET_RI(ri_1);
        acc_register -= ri_1;
        if (acc_register < -999) {
            acc_register = -999;
        }
        break;
    case CMD_MUL: // mul R/I
        CHECK_CONDITION(2);
        GET_RI(ri_1);
        acc_register *= ri_1;
        if (acc_register > 999) {
            acc_register = 999;
        } else if (acc_register < -999) {
            acc_register = -999;
        }
        break;
    case CMD_NOT: // not
        CHECK_CONDITION(0);
        if (acc_register == 0) {
            acc_register = 100;
        } else {
            acc_register = 0;
        }
        break;
    case CMD_DGT: // dgt R/I
        CHECK_CONDITION(2);
        GET_RI(ri_1);

        if (ri_1 > 2) {
            ri_1 = 2;
        } else if (ri_1 < 0) {
            ri_1 = 0;
        }

        reg = acc_register/100U;
        if (ri_1 < 2) {
            acc_register -= reg*100U;
            reg = acc_register/10U;
        }
        if (ri_1 == 0) {
            acc_register -= reg*10U;
        } else {
            acc_register = reg;
        }

        break;
    case CMD_DST: // dst R/I R/I
        CHECK_CONDITION(4);
        GET_RI(ri_1);
        GET_RI(ri_2);

        if (ri_1 > 2) {
            ri_1 = 2;
        } else if (ri_1 < 0) {
            ri_1 = 0;
        }

        if (ri_2 > 9) {
            ri_2 = 9;
        } else if (ri_2 < 0) {
            ri_2 = 0;
        }

        if (acc_register < 0) {
            acc_register = -acc_register;
        }

        if (ri_1 == 0) {
            acc_register = ((acc_register / 10U) * 10U) + ri_2;
        } else if (ri_1 == 1) {
            reg = acc_register - (acc_register / 100U * 100U) - (acc_register / 10U * 10U);
            acc_register = ((acc_register / 100U) * 100U) + ri_2 * 10U + reg;
        } else if (ri_1 == 2) {
            reg = acc_register - (acc_register / 100U * 100U);
            acc_register = ri_2 * 100U + reg;
        }

        break;
    case CMD_LBL: // label L
        current_pos++; // Label, just jump to next command
        break;
    }
    return 0;
}

#endif //__INTERPRETER_H__
