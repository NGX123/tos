/*Chip8 has a screen that is a screen that is 64 pixels wide and 32 pixels tall, and it could
display only one color - white. 

Turning pixels off and on can be done using XOR
0 ^ 0 = 0
0 ^ 1 = 1
1 ^ 0 = 1
1 ^ 1 = 1

If draw operation will turn off a pixel that was on, it would store 1 in register VF - this 
can be used to perform sprite hit detection. 
When we use chip8 we manipulate not pixels but groups of pixels(sprites) that can be 1-15 bytes long, 
these bit patterns correspond to pixels on or off.
         hex
-------+----
byte 1 |  7C
byte 2 |  40
byte 3 |  40
byte 4 |  7C
byte 5 |  40
byte 6 |  40
byte 7 |  7C

Before we use the draw command we need to load the memory location of the sprite into the
Index regiser


DRAW INSTRUCTION
Dxyn - D is instruction code, 
    x specifies the register tha stores X coordinate(0-63), 
    y specifies the register that stores y coordinate(0-31)
    n specifies how many bytes the sprite is(0-15)


*/