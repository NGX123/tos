/* 
In chip8 specification there is a place for character so you would not need to draw each one
on your own when you need to output it

Firtst 80 bytes in the interpreter are reserved for the character from 0-9 a-f each character 
is 5 bytes long
Location    | Character
$00 to $04  |    0
$05 to $09  |    1
$0A to $0E  |    2
$0F to $13  |    3
$14 to $18  |    4
$19 to $1D  |    5
$1E to $22  |    6
$23 to $27  |    7
$28 to $2C  |    8
$2D to $31  |    9
$32 to $36  |    A
$37 to $3B  |    B
$3C to $40  |    C
$41 to $45  |    D
$46 to $4A  |    E
$4B to $4F  |    F


Using Font Set - Hard way
* Place the sprite in memory(done)
* Load intex register with it's start location
* Load register with x position 
* Load register with y position
* Use draw command

Font Set - Easy way 
There is an Fs29 command that uses the source register as an index into font set

Place the sprite in memory (this is already done for us)
Load a register with the value we want to print, in this case, we'll store 9 in R1.
Use the Fs29 command to specify the source register, in this case, it will be R1.
Load a register with the X position, in this case, we'll store 10 in R2.
Load a register with the Y position, in this case, we'll staore 5 in R3.
Issue the draw command with the R2 for the X position, R3 for the Y position, and 5 bytes to print.
*/