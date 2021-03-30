[<< back](index)

# The assembler language


## Program Structure

Each line of an MCxxxx program must have the following structure:

    LABEL CONDITION INSTRUCTION COMMENT

All components are optional, but must appear in the specified order if present. 
Examples of syntactically valid lines:

```
# This line is a comment. 
loop: # until ACC is ten
teq acc 10 
+ jmp end
mov 50 x2 
add 1 
jmp loop
end:
mov 0 acc # reset counter
```


## Comments

Any text following a “#” symbol is ignored until the end of the line. Comments improve developer productivity by allowing the behavior of code to be described in-line with the program itself.


## Labels

Labels must appear first on a line, and are followed by a colon (“__:__”). Labels are used as jump targets by the __jmp__ instruction. Labels must begin with a letter and may contain alphabetic, numeric, and underscore characters.


## Conditional Execution

All instructions in the MCxxxx programming language are capable of conditional execution. 

Prefixing an in- struction with a “__+__” or “__-__” symbol will cause that instruction to be enabled or disabled by test instructions. 

When an instruction is disabled by a test instruction, it will be skipped and will not consume power. Instructions with no prefix are never disabled and always execute normally. All conditional instructions start in a disabled state. A test instruction must be executed to enable conditional instructions.


## Registers

Registers are used as sources and destinations for data manipulated by MC5000 instructions.

__acc__ is the primary general-purpose register used for internal computation on MCxxxx family microcontrollers. All arithmetic operations implicitly use and modify the value of __acc__.

__dat__ is a second register available on the MC5000. It can be used in most contexts where __acc__ is permitted.

The internal registers of the MC5000 microprocessor (__acc__ and __dat__) are initialized to the value 0.
The pin registers (__p0__, __p1__, __x0__, __x1__) are used when reading from or writing to the pins of MC5000 microcontrollers. Reading and writing through the pins allows a single MC5000 microcontroller to communicate and coordinate with other connected, compatible devices. Each pin on an MC5000 microcontroller functions as either a simple I/O or XBus interface.


## Instruction Operands

Each type of instruction requires a fixed number of operands. If an instruction has any associated operands, they must appear following the instruction name, separated by spaces. For the benefit of development productivity, the MC5000 programming system does not require the use of redundant punctuation to separate instruction operands.

Instruction operands are described with the following notation:


| Notation | Meaning                     |
|:--------:|:---------------------------:|
| R        | Register                    |
| I        | Integer [1]                 |
| R/I      | Register or integer [1]     |
| P        | Pin register (p0, p1, etc.) |
| L        | Label [2]                   |

[1] Integer values must be in the range -999 to 999.

[2] Labels used as operands must be defined elsewhere in the program.


## Basic Instructions

__mov R/I R__ : Copy the value of the first operand into the second operand.



__jmp L__ : Jump to the instruction following the specified label.



__slp R/I__ : Sleep for the number of time units specified by the operand.



__slx P__ : Sleep until data is available to be read on the XBus pin specified by the operand.


## Arithmetic Instructions

Registers can store integer values between -999 and 999, inclusive. If an arithmetic operation would produce a result outside this range, the closest allowed value is stored instead. For example, if acc contains the value 800 and the instruction add 400 is executed, then the value 999 will be stored in acc.

__add R/I__ : Add the value of the first operand to the value of the acc register and store the result in the acc register.



__sub R/I__ : Subtract the value of the first operand from the value of the acc register and store the result in the acc register.



__mul R/I__  : Multiply the value of the first operand by the value of the acc register and store the result in the acc register.



__not__ : If the value in acc is 0, store a value of 100 in acc. Otherwise, store a value of 0 in acc. 



__dgt R/I__ : Isolate the specified digit of the value in the acc register and store the result in the acc register. 



__dst R/I R/I__ : Set the digit of acc specified by the first operand to the value of the second operand. Examples of the dgt and dst instructions:


| acc  | Instruction | acc’ |
|:----:|:-----------:|:----:|
| 596  | dgt 0       | 6    |
| 596  | dgt 1       | 9    |
| 596  | dgt 2       | 5    |
| 596  | dst 0 7     | 597  |
| 596  | dst 1 7     | 576  |
| 596  | dst 2 7     | 796  |


## Test Instructions

__teq R/I R/I__

Test if the value of the first operand (A) is equal to the value of the second operand (B).

| Condition            | Effect on ‘+’ Instructions | Effect on ‘-’ Instructions’ |
|:--------------------:|:--------------------------:|:---------------------------:|
| A is equal to B      | Enabled                    | *Disabled*                  |
| A is not equal to B  | *Disabled*                 | Enabled                     |



__tgt R/I R/I__ : Test if the value of the first operand (A) is greater than the value of the second operand (B).

| Condition               | Effect on ‘+’ Instructions | Effect on ‘-’ Instructions’ |
|:-----------------------:|:--------------------------:|:---------------------------:|
| A is greater than B     | Enabled                    | *Disabled*                  |
| A is not greater than B | *Disabled*                 | Enabled                     |



__tlt R/I R/I__ : Test if the value of the first operand (A) is less than the value of the second operand (B).

| Condition            | Effect on ‘+’ Instructions | Effect on ‘-’ Instructions’ |
|:--------------------:|:--------------------------:|:---------------------------:|
| A is less than B     | Enabled                    | *Disabled*                  |
| A is not less than B | *Disabled*                 | Enabled                     |



__tcp R/I R/I__ : Compare the value of the first operand (A) to the value of the second operand (B).

| Condition            | Effect on ‘+’ Instructions | Effect on ‘-’ Instructions’ |
|:--------------------:|:--------------------------:|:---------------------------:|
| A is greater than B  | Enabled                    | *Disabled*                  |
| A is equal to B      | *Disabled*                 | *Disabled*                  |
| A is less than B     | *Disabled*                 | Enabled                     |

