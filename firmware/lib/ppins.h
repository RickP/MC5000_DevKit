#ifndef PPINS_H
#define PPINS_H

#include <stdint.h>
#include "pfs173.h"

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

uint16_t sleep_nops;

inline void setup_ppin_hardware() {
    PAC &= ~(1 << P0_PIN); // Enable P0 Pin as input
    PAPH &= ~(1 << P0_PIN); // Disable P0 pullup

    PAC &= ~(1 << P1_PIN); // Enable P1 Pin as input
    PAPH &= ~(1 << P1_PIN); // Disable P1 pullup


    ADCRGC = ADCRG_ADC_REF_VDD; // VCC reference for ADC
    ADCM = ADCM_CLK_SYSCLK_DIV16; // ADC divider 16

    // Enable PWMG1 and PWMG2 for p port output
    PWMGCLK = PWMGCLK_PWMG_ENABLE | PWMGCLK_CLK_IHRC;
    PWMG1C = PWMG1C_ENABLE;
    PWMG2C = PWMG2C_ENABLE;
    PWMGCUBL = 0x00;
    PWMGCUBH = 0x67;
}

inline uint8_t get_p0_value() {
    P0_PWM &= ~P0_PWM_ENABLE; // Disable PWM output on pin
    PAC &= ~(1 << P0_PIN); //disable GPIO output
    PAPH &= ~(1 << P0_PIN); //disable pull up
    if (!(ADCC & ADCC_ADC_ENABLE)) {
        ADCC = ADCC_ADC_ENABLE; // Enable ADC
        sleep_nops = 0xC800;
        for( ; sleep_nops>0; sleep_nops-- ); // wait 400 us
    }
    ADCC = ADCC_ADC_ENABLE | P0_ADC | ADCC_ADC_CONV_START; // Start ADC
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) ); //busy wait for ADC conversion to finish
    return ADCR;
}

inline uint8_t get_p1_value() {
    P1_PWM &= ~P1_PWM_ENABLE; // Disable PWM output on pin
    PAC &= ~(1 << P1_PIN); //disable GPIO output
    PAPH &= ~(1 << P1_PIN); //disable pull up
    if (!(ADCC & ADCC_ADC_ENABLE)) {
        ADCC = ADCC_ADC_ENABLE; // Enable ADC
        sleep_nops = 0xC800;
        for( ; sleep_nops>0; sleep_nops-- ); // wait 400 us
    }
    ADCC = ADCC_ADC_ENABLE | P1_ADC | ADCC_ADC_CONV_START; // Start ADC
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) ); //busy wait for ADC conversion to finish
    return ADCR;
}

inline void set_p0_value(uint8_t val) {
    ADCC = 0;
    PAC |= (1 << P0_PIN); // Enable P0 Pin as output
    P0_PWM |= P0_PWM_ENABLE; // Enable PWN output on Pin
    P0_PWM_DUTY_H = val;
    P0_PWM_DUTY_L = 0;
}

inline void set_p1_value(uint8_t val) {
    ADCC = 0;
    PAC |= (1 << P1_PIN); // Enable P0 Pin as output
    P1_PWM |= P1_PWM_ENABLE; // Enable PWN output on Pin
    P1_PWM_DUTY_H = val;
    P1_PWM_DUTY_L = 0;
}

#endif // PPINS_H
