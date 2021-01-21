#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#define TICK_INTERPRETER_CLOCK clock_tick++
#define GET16 (program[current_pos++] << 8) + program[current_pos++]
#define GET8 program[current_pos++]

uint8_t *program;
uint8_t program_size = 0;
uint8_t current_pos = 0;
uint16_t u16_1;
uint16_t u16_2;
uint8_t u8;
volatile uint16_t clock_tick;

typedef enum {
        none=0,
        true=1,
        false=2
} true_or_false;

true_or_false condition;

#define NUM_COMMANDS 19
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
        0x11 << 2,   // loop
        0x12 << 2,   // end
};

void set_program(uint8_t *new_program, uint8_t new_program_size) {
        program = new_program;
        program_size = new_program_size;
        current_pos = 0;
}

uint8_t run_program_line() {
        // Handle end of program buffer
        if (current_pos >= program_size-1) current_pos = 0;

        // get current command including condition
        uint8_t command = program[current_pos];
        // Get condition for command
        if (program[current_pos] && 0x01) condition = true;
        else if (program[current_pos] && 0x02) condition = false;
        else condition = none;

        // Remove condition bits
        command &= 0x03;

        // Get command number from static list
        uint8_t command_num = 0;
        for (uint8_t i = 1; i < NUM_COMMANDS; i++) {
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
                u16_1 = GET16;
                u8 = GET8;
                break;
            case 3: // jmp L
                u8 = GET8;
                break;
            case 4: // slp R/I
                u16_1 = GET16;
                break;
            case 5: // slx P
                u8 = GET8;
                break;
            case 6: // teq R/I R/I
                u16_1 = GET16;
                u16_2 = GET16;
                break;
            case 7: // tgt R/I R/I
                u16_1 = GET16;
                u16_2 = GET16;
                break;
            case 8: // tlt R/I R/I
                u16_1 = GET16;
                u16_2 = GET16;
                break;
            case 9: // tcp R/I R/I
                u16_1 = GET16;
                u16_2 = GET16;
                break;
            case 10: // add R/I
                u16_1 = GET16;
                break;
            case 11: // sub R/I
                u16_1 = GET16;
                break;
            case 12: // mul R/I
                u16_1 = GET16;
                break;
            case 13: // not

                break;
            case 14: // dgt R/I
                u16_1 = GET16;
                break;
            case 15: // dst R/I R/I
                u16_1 = GET16;
                u16_2 = GET16;
                break;
            case 16: // label L
                current_pos++;
                return run_program_line(); // Label, jump to next command
                break;
            case 17: // loop
                return run_program_line(); // Loop, jump to next command
                break;
            case 18: // end
                return run_program_line(); // End, jump to start of loop
                break;
        }
        return 0;
}

#endif //__INTERPRETER_H__
