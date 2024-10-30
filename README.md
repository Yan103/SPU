# **SPU**


<img align="right" width="160" height="160" src="img/cpu.jpg">

<div style="margin-right: 170px;">

**SPU** - this is an attempt to implement a proprietary processor in the C programming language.

The program receives an assembler file as input (carefully study the list of available commands!), converts it into machine code and executes it. In the assembler, you can leave comments using the ";" symbol

</div>

## Available processor commands
* PUSH - puts a number on the stack, register, or RAM
* POP - takes a number from the stack and puts it in a register or in RAM
* ADD - adds two numbers
* SUB - finds the difference between two numbers
* DIV - finds the quotient of two numbers
* MUL - finds the product of two numbers
* IN - reads a number from the keyboard
* SQRT - finds the square root of a number
* OUT - displays the result on the screen
* JNE - ump to the label if the numbers are not equal
* JE - jump to the label if the numbers are equal
* JZ - jump to the label if the number is 0
* JLZ - jump to the label if the number is less than 0
* JGZ - jump to the label if the number is greater than 0
* JMP - jump to the label
* CALL - calling a function
* RET - recursive return from a function
* HLT - the sign of the end of the program

## Installation and compilation
To run the program on your computer, download the repository and run the Makefile
1. Cloning a repository
```bash
https://github.com/Yan103/SPU
```
2. Compilation of source files
```bash
make
```
3. Start
```bash
make run
```
4. Optional: remove files (deleting object and executable files)
```bash
make clean
```
For a more detailed study of the principle of operation of the program, you can also read the *documentation* that is available in.

## Principles of work
The processor supports working with registers, RAM, is able to cause recursion, and also has conditional operators. To test the operation of the processor, you can use ready-made program templates - finding a factorial for a natural number or solving a quadratic equation.

## Contact information
If you are interested in this project or have any questions/suggestions about the project, you can contact the author:

**Mail**: fazleev.yans@phystech.edu

**Telegram**: @yan_ya_n3

![Alt Text](https://media.giphy.com/media/vFKqnCdLPNOKc/giphy.gif)
