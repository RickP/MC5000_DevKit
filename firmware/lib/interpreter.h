#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "pfs173.h"
#include "delay.h"
#include "serial.h"

#define SLEEP_TICKS 200

#define INTERPRETER_CLOCK_TICK clock_tick++
#define GET_RI get_val(program[current_pos++], program[current_pos++])
#define GET_R program[current_pos++]
#define CHECK_CONDITION(x) if (command_condition != none && current_condition != command_condition) {current_pos += (uint8_t) x; break;}
#define SLEEP(x) clock_tick = 0; sleep_until = x

uint8_t *program;
uint8_t program_size = 0;
uint8_t current_pos = 0;
int16_t ri_1 = 0;
int16_t ri_2 = 0;
uint8_t reg = 0;
int16_t xbus0_data = 0;
uint8_t xbus0_bitcounter = 0;
int16_t xbus1_data = 0;
uint8_t xbus1_bitcounter = 0;
uint8_t xbus_state = 0;
int16_t acc_register = 0;
int16_t dat_register = 0;
uint16_t sleep_until = 0;
volatile uint16_t clock_tick = 0;

typedef enum {
    false=0,
    true=1,
    none=2
} true_or_false;

true_or_false current_condition;

#define XBUS_DATA 0x7FFF

#define XBUS_IDLE 0x00

#define XBUS0_SL 0x01
#define XBUS0_TX_READY 0x02
#define XBUS0_TX_START 0x03
#define XBUS0_TX 0x04
#define XBUS0_TX_DONE 0x05
#define XBUS0_RX_READY 0x06
#define XBUS0_RX_START 0x07
#define XBUS0_RX 0x08

#define XBUS1_SL 0x10
#define XBUS1_TX_READY 0x20
#define XBUS1_TX_START 0x30
#define XBUS1_TX 0x40
#define XBUS1_TX_DONE 0x50
#define XBUS1_RX_READY 0x60
#define XBUS1_RX_START 0x70
#define XBUS1_RX 0x80

#define XBUS_BITTIME 6
#define XBUS_DELAY XBUS_BITTIME/2

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
#define P0_PWM_DUTY_H PWMG2DTH
#define P0_PWM_DUTY_L PWMG2DTL

#define P1_PIN 4
#define P1_ADC ADCC_CH_AD9_PA4
#define P1_PWM PWMG1C
#define P1_PWM_ENABLE PWMG1C_OUT_PA4
#define P1_PWM_DUTY_H PWMG1DTH
#define P1_PWM_DUTY_L PWMG1DTL

#define X0_PIN 6
#define X1_PIN 7

inline uint8_t get_p0_value() {
    P0_PWM &= ~P0_PWM_ENABLE; // Disable PWM output on pin
    PAC &= ~(1 << P0_PIN); //disable GPIO output
    PAPH &= ~(1 << P0_PIN); //disable pull up
    ADCC = ADCC_ADC_ENABLE | P0_ADC | ADCC_ADC_CONV_START; // Enable ADC
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
    P1_PWM &= ~P1_PWM_ENABLE; // Disable PWM output on pin
    PAC &= ~(1 << P1_PIN); //disable GPIO output
    PAPH &= ~(1 << P1_PIN); //disable pull up
    ADCC = ADCC_ADC_ENABLE | P1_ADC | ADCC_ADC_CONV_START; // Enable ADC
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) ); //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
    return ADCR;
}

inline void set_p1_value(uint8_t val) {
    ADCC = 0;
    PAC |= (1 << P1_PIN); // Enable P0 Pin as output
    P1_PWM |= P1_PWM_ENABLE; // Enable PWN output on Pin
    P1_PWM_DUTY_H = val;
    P1_PWM_DUTY_L = 0;
}

void setup_interpreter_hardware() {
    PAC &= ~(1 << P0_PIN); // Enable P0 Pin as input
    PAPH &= ~(1 << P0_PIN); // Disable P0 pullup

    PAC &= ~(1 << P1_PIN); // Enable P1 Pin as input
    PAPH &= ~(1 << P1_PIN); // Disable P1 pullup

    PAC &= ~(1 << X0_PIN); // Enable X0 Pin as input
    PAPH &= ~(1 << X0_PIN); // Disable X0 pullup

    PAC &= ~(1 << X1_PIN); // Enable X1 Pin as input
    PAPH &= ~(1 << X1_PIN); // Disable P1 pullup

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
    xbus0_data = 0;
    xbus0_bitcounter = 0;
    xbus1_data = 0;
    xbus1_bitcounter = 0;
    xbus_state = XBUS_IDLE;
    sleep_until = 0;
    set_p0_value(0);
    set_p1_value(0);
    PAPH &= ~(1 << X1_PIN); // Disable pullup
    PAPH &= ~(1 << X0_PIN); // Disable pullup
    PAC &= ~(1 << X0_PIN); // Set pin as input
    PAC &= ~(1 << X1_PIN); // Set pin as input
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
                // reusing sleep unitl var here because we don't need it currently
                if (argh & 0x08) {
                    sleep_until = get_p1_value();
                } else {
                    sleep_until = get_p0_value();
                }
                sleep_until = sleep_until >> 1;
                if (sleep_until > 95) sleep_until = 100;
                if (sleep_until < 5) sleep_until = 0;
                return sleep_until;
            } else {
                // x pin -> bit 4 defines port num
                if (argh & 0x08) {
                    // get data from xbus 1
                    xbus_state = XBUS1_RX_READY;
                    PAPH &= ~(1 << X1_PIN); // Disable pullup
                    PAC &= ~(1 << X1_PIN); // Set pin as input
                    xbus1_data = 0;
                    xbus1_bitcounter = 0;
                } else {
                    xbus_state = XBUS0_RX_READY;
                    PAPH &= ~(1 << X0_PIN); // Disable pullup
                    PAC &= ~(1 << X0_PIN); // Set pin as input
                    xbus0_data = 0;
                    xbus0_bitcounter = 0;
                }
                return XBUS_DATA;
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
                set_p1_value((arg & 0xFF));
            } else {
                set_p0_value((arg & 0xFF));
            }
        } else {
            // x pin -> bit 4 defines port num
            if (reg & 0x08) {
                // put data on xbus 1
                xbus_state = XBUS1_TX_READY;
                PAC &= ~(1 << X1_PIN); // Set pin as input
                PAPH |= (1 << X1_PIN); // Enable pullup
                xbus1_data = arg;
                xbus1_bitcounter = 0;
            } else {
                // put data on xbus 0
                xbus_state = XBUS0_TX_READY;
                PAC &= ~(1 << X0_PIN); // Set pin as input
                PAPH |= (1 << X0_PIN); // Enable pullup
                xbus0_data = arg;
                xbus0_bitcounter = 0;
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
    switch (xbus_state) {
    case XBUS0_SL:
        if (PA & (1 << X0_PIN)) {
            xbus_state = XBUS_IDLE;
        }
        return 0;
        break;
    case XBUS0_TX_READY:
        if (!(PA & (1 << X0_PIN))) {
            PAPH &= ~(1 << X0_PIN); // Disable pullup
            xbus_state = XBUS0_TX_START;
            SLEEP(XBUS_BITTIME);
        };
        return 0;
        break;
    case XBUS0_TX_START:
        PAC |= (1 << X0_PIN); // Set pin as output
    // Fallthrough!
    case XBUS0_TX:
        // Send one bit and wait XBUS_BITTIME ticks
        if ((xbus0_data >> xbus0_bitcounter++) & 0x01) __set1(PA, X0_PIN); // Set pin high
        else __set0(PA, X0_PIN); // Set pin low
        if (xbus0_bitcounter == 0x0F) { // Transmission done
            xbus_state = XBUS0_TX_DONE;
        };
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    case XBUS0_TX_DONE:
        PAC &= ~(1 << X0_PIN); // Set pin as input
        xbus_state = XBUS_IDLE;
        return 0;
        break;
    case XBUS0_RX_READY:
        if (PA & (1 << X0_PIN)) {
            PAPH &= ~(1 << X0_PIN); // Disable pullup
            PAC |= (1 << X0_PIN); // Set pin as output
            __set0(PA, X0_PIN); // Set pin low
            xbus_state = XBUS0_RX_START;
        }
        SLEEP(XBUS_DELAY);
        return 0;
        break;
    case XBUS0_RX_START:
        PAC &= ~(1 << X0_PIN); // Set pin as input
        xbus_state = XBUS0_RX;
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    case XBUS0_RX:
        // Receive one bit and wait XBUS_BITTIME ticks
        if (PA & (1 << X0_PIN)) {
            xbus1_data = 1; // Reusing a global var
            xbus0_data |= (xbus1_data << xbus0_bitcounter);
        };
        xbus0_bitcounter++;
        if (xbus0_bitcounter == 0x0F) {// Transmission done
            set_val(xbus0_data, program[current_pos-1]); // set received value
            xbus_state = XBUS_IDLE; // reset xbus state
        };
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    case XBUS1_SL:
        if (PA & (1 << X1_PIN)) {
            xbus_state = XBUS_IDLE;
        }
        return 0;
        break;
    case XBUS1_TX_READY:
        if (!(PA & (1 << X1_PIN))) {
            PAPH &= ~(1 << X1_PIN); // Disable pullup
            xbus_state = XBUS1_TX_START;
            SLEEP(XBUS_BITTIME);
        };
        return 0;
        break;
    case XBUS1_TX_START:
        PAC |= (1 << X1_PIN); // Set pin as output
    // Fallthrough!
    case XBUS1_TX:
        // Send one bit and wait XBUS_BITTIME ticks
        if ((xbus1_data >> xbus1_bitcounter++) & 0x01) __set1(PA, X1_PIN); // Set pin high
        else __set0(PA, X1_PIN); // Set pin low
        if (xbus1_bitcounter == 0x0F) { // Transmission done
            xbus_state = XBUS1_TX_DONE;
        };
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    case XBUS1_TX_DONE:
        PAC &= ~(1 << X1_PIN); // Set pin as input
        xbus_state = XBUS_IDLE;
        return 0;
        break;
    case XBUS1_RX_READY:
        if (PA & (1 << X1_PIN)) {
            PAPH &= ~(1 << X1_PIN); // Disable pullup
            PAC |= (1 << X1_PIN); // Set pin as output
            __set0(PA, X1_PIN); // Set pin low
            xbus_state = XBUS1_RX_START;
        }
        SLEEP(XBUS_DELAY);
        return 0;
        break;
    case XBUS1_RX_START:
        PAC &= ~(1 << X1_PIN); // Set pin as input
        xbus_state = XBUS1_RX;
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    case XBUS1_RX:
        // Receive one bit and wait XBUS_BITTIME ticks
        if (PA & (1 << X1_PIN)) {
            xbus0_data = 1; // Reusing a global var
            xbus1_data |= (xbus0_data << xbus1_bitcounter);
        };
        xbus1_bitcounter++;
        if (xbus1_bitcounter == 0x0F) {// Transmission done
            set_val(xbus1_data, program[current_pos-1]); // set received value
            xbus_state = XBUS_IDLE; // reset xbus state
        };
        SLEEP(XBUS_BITTIME);
        return 0;
        break;
    }

    // Handle end of program buffer
    if (current_pos >= program_size) current_pos = 0;

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
        reg = GET_R;
        if (ri_1 != XBUS_DATA) set_val(ri_1, reg); // set value to register/pin
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
        if (reg == 0x40) {
            PAPH &= ~(1 << X0_PIN); // Disable pullup
            PAC &= ~(1 << X0_PIN); // Set pin as input
            xbus_state = XBUS0_SL;
        } else {
            PAPH &= ~(1 << X1_PIN); // Disable pullup
            PAC &= ~(1 << X1_PIN); // Set pin as input
            xbus_state = XBUS1_SL;
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

        if (acc_register < 0) {
            acc_register = -acc_register;
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

        break;
    case CMD_LBL: // label L
        current_pos++; // Label, just jump to next command
        break;
    }
    return 0;
}

#endif //__INTERPRETER_H__
