#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "pfs173.h"
#include "delay.h"
#include "serial.h"

#define SLEEP_TICKS     500

#define INTERPRETER_CLOCK_TICK clock_tick++
#define GET_RI get_val(program[current_pos], program[current_pos+1]); current_pos += 2
#define GET_R program[current_pos++]
#define CHECK_CONDITION(x) if (command_condition != none && current_condition != command_condition) {current_pos += (uint8_t) x; break;}
#define SLEEP(x) clock_tick = 0; sleep_until = x

uint8_t *program;
uint8_t program_size = 0;
uint8_t current_pos = 0;
int16_t ri_1 = 0;
int16_t ri_2 = 0;
uint8_t reg;
uint16_t xbus_data_0 = 0;
uint16_t xbus_data_1 = 0;
uint8_t xbus_state_0 = 0;
uint8_t xbus_state_1 = 0;
int16_t acc_register = 0;
int16_t dat_register = 0;
uint16_t sleep_until = 0;
volatile uint16_t clock_tick = 0;

typedef enum {
    none=0,
    true=1,
    false=2
} true_or_false;

true_or_false current_condition;

#define XBUS_SLX 0x01
#define XBUS_RX 0x02
#define XBUS_TX 0x03

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


#define P0_PIN 3
#define P0_ADC ADCC_CH_AD8_PA3
#define P0_PWM PWMG2C
#define P0_PWM_ENABLE PWMG2C_OUT_PA3
#define P0_PWM_DUTY_H PWMG0DTH
#define P0_PWM_DUTY_L PWMG0DTL

#define P1_PIN 4
#define P1_ADC ADCC_CH_AD9_PA4
#define P1_PWM PWMG1C
#define P1_PWM_ENABLE PWMG1C_OUT_PA4
#define P1_PWM_DUTY_H PWMG1DTH
#define P1_PWM_DUTY_L PWMG1DTL

#define X0_PIN 5
#define X1_PIN 6

inline uint8_t get_p0_value() {
    P0_PWM &= ~P0_PWM_ENABLE;   // Disable PWM output on pin
    PAC &= ~(1 << P0_PIN);      // Enable P0 Pin as input
    ADCC = P0_ADC;              // Set ADC for pin
    ADCC |= ADCC_ADC_ENABLE;    // Enable ADC
    delay_us(400);              // wait to settle
    ADCC |= ADCC_ADC_CONV_START; //start ADC conversion
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) ); //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
    return ADCR;
}

inline void set_p0_value(uint8_t val) {
    ADCC = 0;
    PAC |= (1 << P0_PIN); // Enable P0 Pin as output
    P0_PWM |= P0_PWM_ENABLE; // Enable PWN output on Pin
    P0_PWM_DUTY_H = val;
    P0_PWM_DUTY_L = 0;
}

inline uint8_t get_p1_value() {
    P1_PWM &= ~P1_PWM_ENABLE;   // Disable PWM output on pin
    PAC |= (1 << P1_PIN);       // Enable P1 Pin as input
    ADCC = P1_ADC;              // Set ADC for pin
    ADCC |= ADCC_ADC_ENABLE;    // Enable ADC
    delay_us(400);              // wait to settle
    ADCC |= ADCC_ADC_CONV_START; //start ADC conversion
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) ); //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
    return ADCR;
}

inline void set_p1_value(uint8_t val) {
    if (!(PAC & (1 << P1_PIN))) {
        ADCC = 0;
        PAC |= (1 << P1_PIN); // Enable P0 Pin as output
        P1_PWM |= P1_PWM_ENABLE; // Enable PWN output on Pin
    }
    P1_PWM_DUTY_H = val;
    P1_PWM_DUTY_L = 0;
}

uint8_t get_x0_value(uint8_t pull_up) {
    if (pull_up) {
        PAPH |= (1 << X0_PIN); // Enable pullup
    } else {
        PAPH &= ~(1 << X0_PIN); // Disable pullup
    }
    PAC &= ~(1 << X0_PIN); // Set pin as input
    return PA & (1 << X0_PIN);
}

inline void set_x0_value(uint8_t pin_state, uint8_t pull_up) {
    if (pull_up) {
        PAPH |= (1 << X0_PIN); // Enable pullup
    } else {
        PAPH &= ~(1 << X0_PIN); // Disable pullup
    }
    PAC &= ~(1 << X0_PIN); // Set pin as output
    if (pin_state) {
        PA |= (1 << X0_PIN);
    } else {
        PA &= ~(1 << X0_PIN);
    }
}

uint8_t get_x1_value(uint8_t pull_up) {
    if (pull_up) {
        PAPH |= (1 << X1_PIN); // Enable pullup
    } else {
        PAPH &= ~(1 << X1_PIN); // Disable pullup
    }
    PAC &= ~(1 << X1_PIN); // Set pin as input
    return PA & (1 << X1_PIN);
}

inline void set_x1_value(uint8_t pin_state, uint8_t pull_up) {
    if (pull_up) {
        PAPH |= (1 << X1_PIN); // Enable pullup
    } else {
        PAPH &= ~(1 << X1_PIN); // Disable pullup
    }
    PAC &= ~(1 << X1_PIN); // Set pin as output
    if (pin_state) {
        PA |= (1 << X1_PIN);
    } else {
        PA &= ~(1 << X1_PIN);
    }
}

void setup_interpreter_hardware() {
    PAC &= ~(1 << P0_PIN); // Enable P0 Pin as input
    PAPH &= ~(1 << P0_PIN); // Disable P0 pullup

    PAC &= ~(1 << P1_PIN); // Enable P1 Pin as input
    PAPH &= ~(1 << P1_PIN); // Disable P1 pullup

    PAC &= ~(1 << X0_PIN); // Enable X0 Pin as input
    PAPH &= ~(1 << X0_PIN); // Disable X0 pullup

    PAC &= ~(1 << X1_PIN); // Enable X1 Pin as input
    PAPH &= ~(1 << P1_PIN); // Disable P1 pullup

    ADCRGC = ADCRG_ADC_REF_VDD; // VCC reference for ADC
    ADCM = ADCM_CLK_SYSCLK_DIV16; // ADC divider 16

    // Enable PWMG1 and PWMG2 for p port output
    PWMGCLK = PWMGCLK_PWMG_ENABLE;
    PWMG1C = PWMG1C_ENABLE;
    PWMG2C = PWMG2C_ENABLE;
    PWMGCUBL = 0x00;
    PWMGCUBH = 0x80;
}

void reset_program() {
    // Reset registers and outputs
    current_pos = 0;
    clock_tick = 0;
    acc_register = 0;
    dat_register = 0;
    current_condition = none;
    xbus_data_0 = 0;
    xbus_data_1 = 0;
    xbus_state_0 = 0;
    xbus_state_1 = 0;
    sleep_until = 0;
    set_p0_value(0);
    set_p1_value(0);
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

    // analyze first byte
    // Bit 1 of argh defines register (1) or scalar (0)
    if (argh & 0x40) {
        // Bit 2 defines memory (1) or pin (0)
        if (argh & 0x20) {
            // memory -> bit 3 defines acc (1) or dat(0)
            if (argh & 0x10) {
                return acc_register;
            } else {
                return dat_register;
            }
        } else {
            // pin -> bit 3 defines p(1) or x(0)
            if (argh & 0x10) {
                // p pin -> bit 4 defines port num
                if (argh & 0x08) {
                    return get_p1_value();
                } else {
                    return get_p0_value();
                }
            } else {
                // x pin -> bit 4 defines port num
                if (argh & 0x08) {
                    // @ToDo: get data from xbus 1
                } else {
                    // @ToDo: get data from xbus 0
                }
            }
        }
    }

    // Else this is a scalar value -> assemble the 11bit value
    int16_t arg = argh & 0x1F; // Use the last 5 bits of argh
    arg <<= 6; // shift them 6 positions to the left
    arg |= argl & 0x3F; // add the last 6 bits of argl
    arg -= 1000; // create signed integer

    // Limit arg to -999 to 999
    if (arg < -999) arg = -999;
    else if (arg > 999) arg = 999;

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
                set_p1_value(arg & 0xFF);
            } else {
                set_p0_value(arg & 0xFF);
            }
        } else {
            // x pin -> bit 4 defines port num
            if (reg & 0x08) {
                // @ToDo: write data on XBus1
            } else {
                // @ToDo: write data on XBus0
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

    if (xbus_state_0 == XBUS_SLX) {
        if (get_x0_value(0)) {
            xbus_state_0 = 0;
        } else {
            SLEEP(10);
            return 0;
        }
    } else if (xbus_state_1 == XBUS_SLX) {
        if (get_x1_value(0)) {
            xbus_state_1 = 0;
        } else {
            SLEEP(10);
            return 0;
        }
    }

    // Handle end of program buffer
    if (current_pos >= program_size-1) current_pos = 0;

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
        ri_1 = GET_RI;
        if (ri_1 == 0x0FFF) {
            current_pos -= 3;
            break;
        }
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
        SLEEP(GET_RI * SLEEP_TICKS);
        break;
    case CMD_SLX: // slx P
        CHECK_CONDITION(1);
        reg = GET_R;
        // fist two bits of argument encode the XBus port to use
        if (reg == 0x30) {
            xbus_state_0 = XBUS_SLX;
        } else {
            xbus_state_1 = XBUS_SLX;
        }
        break;
    case CMD_TEQ: // teq R/I R/I
        CHECK_CONDITION(4);
        ri_1 = GET_RI;
        ri_2 = GET_RI;
        if (ri_1 == ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TGT: // tgt R/I R/I
        CHECK_CONDITION(4);
        ri_1 = GET_RI;
        ri_2 = GET_RI;
        if (ri_1 > ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TLT: // tlt R/I R/I
        CHECK_CONDITION(4);
        ri_1 = GET_RI;
        ri_2 = GET_RI;
        if (ri_1 < ri_2) {
            current_condition = true;
        } else {
            current_condition = false;
        }
        break;
    case CMD_TCP: // tcp R/I R/I
        CHECK_CONDITION(4);
        ri_1 = GET_RI;
        ri_2 = GET_RI;
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
        ri_1 = GET_RI;
        acc_register += ri_1;
        if (acc_register > 999) {
            acc_register = 999;
        }
        break;
    case CMD_SUB: // sub R/I
        CHECK_CONDITION(2);
        ri_1 = GET_RI;
        acc_register -= ri_1;
        if (acc_register < -999) {
            acc_register = -999;
        }
        break;
    case CMD_MUL: // mul R/I
        CHECK_CONDITION(2);
        ri_1 = GET_RI;
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
        ri_1 = GET_RI;
        if (ri_1 > 2) ri_1 = 2;
        else if (ri_1 < 0) ri_1 = 0;

        acc_register = acc_register < 0 ? -acc_register : acc_register;
        reg = acc_register/100;
        if (ri_1 < 2) {
            acc_register -= reg*100;
            reg = acc_register/10;
        }
        if (ri_1 == 0) acc_register -= reg*10;
        else acc_register = reg;

        break;
    case CMD_DST: // dst R/I R/I
        CHECK_CONDITION(4);
        ri_1 = GET_RI;
        ri_2 = GET_RI;

        if (ri_1 > 2) ri_1 = 2;
        else if (ri_1 < 0) ri_1 = 0;
        if (ri_2 > 9) ri_2 = 9;
        else if (ri_2 < 0) ri_2 = 0;

        sleep_until = 0; // Reuse sleep_until to mark negative acc
        if (acc_register < 0) {
            acc_register = -acc_register;
            sleep_until = 1;
        }

        if (ri_1 == 0) {
            acc_register = ((acc_register / 10) * 10) + ri_2;
        } else if (ri_1 == 1) {
            reg = acc_register - (acc_register / 100 * 100) - (acc_register / 10 * 10);
            acc_register = ((acc_register / 100) * 100) + ri_2 * 10 + reg;
        } else if (ri_1 == 2) {
            reg = acc_register - (acc_register / 100 * 100);
            acc_register = ri_2 * 100 + reg;
        }

        if (sleep_until) {
            acc_register = -acc_register;
            sleep_until = 0;
        }

        break;
    case CMD_LBL: // label L
        current_pos++; // Label, just jump to next command
        break;
    }
    return 0;
}

#endif //__INTERPRETER_H__
