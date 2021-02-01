#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "pfs173.h"
#include "delay.h"
#include "serial.h"
#include "manchester.h"

#define SLEEP_TICKS     500

#define XBUS_DELAY          10
#define XBUS_SHORT_DELAY    XBUS_DELAY/5
#define XBUS_SIGNAL_TX  ((uint8_t) 0xC000)
#define XBUS_TX         ((uint8_t) 0x8000)
#define XBUS_SLX        ((uint8_t) 0x7000)
#define XBUS_SIGNAL_RX  ((uint8_t) 0x6000)
#define XBUS_RX_START   ((uint8_t) 0x5000)
#define XBUS_RX         ((uint8_t) 0x4000)



#define INTERPRETER_CLOCK_TICK clock_tick++
#define GET_RI get_val(program[current_pos++], program[current_pos++])
#define GET_R program[current_pos++]
#define CHECK_CONDITION(x) if (command_condition != none && current_condition != command_condition) {current_pos += (uint8_t) x; break;}
#define SLEEP(x) clock_tick = 0; sleep_until = x

uint8_t *program;
uint8_t program_size = 0;
uint8_t current_pos = 0;
uint16_t ri_1;
uint16_t ri_2;
uint8_t reg;
int16_t xbus_data[2] = {0, 0};
uint8_t xbus_ticks[2] = {0, 0};
int16_t acc_register = 0;
int16_t dat_register = 0;
volatile uint16_t clock_tick = 0;
uint16_t sleep_until = 0;

typedef enum {
        none=0,
        true=1,
        false=2
} true_or_false;

true_or_false current_condition;

#define NUM_COMMANDS 17 // MAX 31!
const uint8_t commands[NUM_COMMANDS] = {
        0x0,   // failure
        0x01 << 2,   // nop
        0x02 << 2,   // mov R/I R
        0x03 << 2,   // jmp L
        0x04 << 2,   // slp R/I
        0x05 << 2,   // slx P
        0x06 << 2,   // teq R/I R/I
        0x07 << 2,   // tgt R/I R/I
        0x08 << 2,   // tlt R/I R/I
        0x09 << 2,   // tcp R/I R/I
        0x0A << 2,   // add R/I
        0x0B << 2,   // sub R/I
        0x0C << 2,   // mul R/I
        0x0D << 2,   // not
        0x0E << 2,   // dgt R/I
        0x0F << 2,   // dst R/I R/I
        0x10 << 2,   // label L
};

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
    P0_PWM &= ~P0_PWM_ENABLE; // Disable PWM output on pin
    PAC &= ~(1 << P0_PIN);      // Enable P0 Pin as input
    ADCC = P0_ADC;                             // Set ADC for pin
    ADCC |= ADCC_ADC_ENABLE;                    // Enable ADC
    delay_us(400);                              // wait to settle
    ADCC |= ADCC_ADC_CONV_START;                //start ADC conversion
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) );  //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
    return ADCR;
}

inline void set_p0_value(uint8_t val) {
    ADCC = 0;
    PAC |= (1 << P0_PIN);    // Enable P0 Pin as output
    P0_PWM |= P0_PWM_ENABLE; // Enable PWN output on Pin
    P0_PWM_DUTY_H = val;
    P0_PWM_DUTY_L = 0;
}

inline uint8_t get_p1_value() {
    P1_PWM &= ~P1_PWM_ENABLE; // Disable PWM output on pin
    PAC |= (1 << P1_PIN);      // Enable P1 Pin as input
    ADCC = P1_ADC;                             // Set ADC for pin
    ADCC |= ADCC_ADC_ENABLE;                    // Enable ADC
    delay_us(400);                                // wait to settle
    ADCC |= ADCC_ADC_CONV_START;                //start ADC conversion
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) );  //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
    return ADCR;
}

inline void set_p1_value(uint8_t val) {
    if (!(PAC & (1 << P1_PIN))) {
        ADCC = 0;
        PAC |= (1 << P1_PIN);    // Enable P0 Pin as output
        P1_PWM |= P1_PWM_ENABLE; // Enable PWN output on Pin
    }
    P1_PWM_DUTY_H = val;
    P1_PWM_DUTY_L = 0;
}

void setup_interpreter_hardware() {
    PAC &= ~(1 << P0_PIN);     // Enable P0 Pin as input
    PAPH &= ~(1 << P0_PIN);    // Disable P0 pullup

    PAC &= ~(1 << P1_PIN);     // Enable P1 Pin as input
    PAPH &= ~(1 << P1_PIN);    // Disable P1 pullup

    PAC &= ~(1 << X0_PIN);      // Enable X0 Pin as input
    PAPH &= ~(1 << X0_PIN);    // Disable X0 pullup

    PAC &= ~(1 << X1_PIN);     // Enable X1 Pin as input
    PAPH &= ~(1 << P1_PIN);    // Disable P1 pullup

    ADCRGC = ADCRG_ADC_REF_VDD;                // VCC reference for ADC
    ADCM = ADCM_CLK_SYSCLK_DIV16;             // ADC divider 16

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
        xbus_data[0] = 0;
        xbus_ticks[0] = 0;
        xbus_data[1] = 0;
        xbus_ticks[1] = 0;
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
        if (program[i] == commands[16]) {
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
                    // Set X1 as waiting for data
                    xbus_data[1] = XBUS_SIGNAL_RX;
                } else {
                    // Set X0 as waiting for data
                    xbus_data[0] = XBUS_SIGNAL_RX;
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

    // Bit 1 of reg defines memory (1) or pin (0)
    if (reg & 0x40) {
        // memory -> bit 2 defines acc (1) or dat(0)
        if (reg & 0x20) {
            acc_register = arg;
        } else {
            dat_register = arg;
        }
    } else {
        // pin -> bit 2 defines p(1) or x(0)
        if (reg & 0x20) {
            // p pin -> bit 3 defines port num
            if (arg < 0) arg = 0; // Limit arg
            else if (arg > 100) arg = 100;

            if (reg & 0x10) {
                set_p1_value(arg & 0xFF);
            } else {
                set_p0_value(arg & 0xFF);
            }
        } else {
            // x pin -> bit 3 defines port num
            if (reg & 0x10) {
                // Set X1 as waiting to send arg
                xbus_data[1] = XBUS_TX & arg;
            } else {
                // Set X0 as waiting to send arg
                xbus_data[0] = XBUS_TX & arg;
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

        uint8_t i = 0;

        // Hadnle XBUS transmission
        uint8_t xpin;
        for (i=0; i < 2; i++) {
            xpin = i ? X1_PIN : X0_PIN;
            if ((xbus_data[i] & 0xE000) == XBUS_SLX) {
                // XBus is in slx mode -> set port as input and check for a high signal
                PAPH &= ~(1 << xpin);     // Disable pullup
                PAC &= ~(1 << xpin);      // Enable pin as input
                if (PA & (1 << xpin)) {
                    xbus_data[i] = 0; // Sender is ready to send, go on
                } else {
                    return 0;             // idle while pin is low
                }
            } else if ((xbus_data[i] & 0xE000) == XBUS_SIGNAL_RX) {
                // XBus is in read mode -> set port as output and display readyness by pulling the pin low
                PAPH &= ~(1 << xpin);      // Disable pullup
                PAC |= (1 << xpin);        // Enable pin as output
                PA |= (1 << xpin);         // Set pin to low
                xbus_data[i] = XBUS_RX;
                SLEEP(XBUS_DELAY);         // wait a bit
                return 0;
            } else if ((xbus_data[i] & 0xE000) == XBUS_RX_START) {
                // XBus is in read mode -> set port as imput and trigger transmission start by enabling pullup
                PAC &= ~(1 << xpin);       // Enable pin as input
                PAPH |= (1 << xpin);       // Enable pullup to signal transmission start
                xbus_data[i] = XBUS_RX;
                SLEEP(XBUS_DELAY);         // wait a bit
                return 0;
            } else if ((xbus_data[i] & 0xC000) == XBUS_RX) {
                xbus_data[i] = 0;
                return 0;
            } else if ((xbus_data[i] & 0xC000) == XBUS_SIGNAL_TX) {
                PAC &= ~(1 << xpin);       // Enable pin as input
                PAPH |= (1 << xpin);       // Enable pullup to signal transmission ready
                if (PA & (1 << xpin)) {
                    return 0;               // noone is ready to receive, just idle
                } else {
                    xbus_data[i] = XBUS_TX;     // Sender is ready to send, go on
                    SLEEP(XBUS_SHORT_DELAY);    // wait a bit
                    return 0;                   
                }
            } else if ((xbus_data[i] & 0xC000) == XBUS_TX) {
     
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

        // Get command number from static list
        uint8_t command_num = 0;
        for (i = 1; i < NUM_COMMANDS; i++) {
                if (commands[i] == command) {
                    command_num = i;
                    break;
                }
        }

        // Increase programm array counter to next byte
        current_pos++;

        switch (command_num) {
            case 0:
                return 1; // Command not found - return 1 as failure
            case 1: // nop
                break;
            case 2: // mov R/I R
                CHECK_CONDITION(3);
                ri_1 = GET_RI;
                reg = GET_R;
                set_val(ri_1, reg); // set value to register/pin
                break;
            case 3: // jmp L
                CHECK_CONDITION(1);
                reg = GET_R;
                current_pos = find_label(reg); // Set position to after label pos
                break;
            case 4: // slp R/I
                CHECK_CONDITION(2);
                SLEEP(GET_RI * SLEEP_TICKS);
                break;
            case 5: // slx P
                CHECK_CONDITION(1);
                reg = GET_R;
                // fist two bits of argument encode the XBus port to use
                if (reg & 0x4800 == 0x4800) {
                    // mark X1 as waiting for ready
                    xbus_data[1] = XBUS_SLX;
                } else {
                    // mark X0 as waiting for ready
                    xbus_data[0] = XBUS_SLX;
                }
                break;
            case 6: // teq R/I R/I
                CHECK_CONDITION(4);
                ri_1 = GET_RI;
                ri_2 = GET_RI;
                if (ri_1 == ri_2) {
                    current_condition = true;
                } else {
                    current_condition = false;
                }
                break;
            case 7: // tgt R/I R/I
                CHECK_CONDITION(4);
                ri_1 = GET_RI;
                ri_2 = GET_RI;
                if (ri_1 > ri_2) {
                    current_condition = true;
                } else {
                    current_condition = false;
                }
                break;
            case 8: // tlt R/I R/I
                CHECK_CONDITION(4);
                ri_1 = GET_RI;
                ri_2 = GET_RI;
                if (ri_1 < ri_2) {
                    current_condition = true;
                } else {
                    current_condition = false;
                }
                break;
            case 9: // tcp R/I R/I
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
            case 10: // add R/I
                CHECK_CONDITION(2);
                ri_1 = GET_RI;
                acc_register += ri_1;
                if (acc_register > 999) {
                    acc_register = 999;
                }
                break;
            case 11: // sub R/I
                CHECK_CONDITION(2);
                ri_1 = GET_RI;
                acc_register -= ri_1;
                if (acc_register < -999) {
                    acc_register = -999;
                }
                break;
            case 12: // mul R/I
                CHECK_CONDITION(2);
                ri_1 = GET_RI;
                acc_register *= ri_1;
                if (acc_register > 999) {
                    acc_register = 999;
                } else if (acc_register < -999) {
                    acc_register = -999;
                }
                break;
            case 13: // not
                CHECK_CONDITION(0);
                if (acc_register == 0) {
                    acc_register = 100;
                } else {
                    acc_register = 0;
                }
                break;
            case 14: // dgt R/I
                CHECK_CONDITION(2);
                ri_1 = GET_RI;
                // @ToDo: isolate a digit from acc and store it in acc
                break;
            case 15: // dst R/I R/I
                CHECK_CONDITION(4);
                ri_1 = GET_RI;
                ri_2 = GET_RI;
                // @ToDo: set the digit from the first operant in acc to the scond opernat
                break;
            case 16: // label L
                current_pos++; // Label, just jump to next command
                break;
        }
        return 0;
}

#endif //__INTERPRETER_H__
