#Alex Yu
#Translating C++ Lab
#Chris Muncey and Zach McMichael
#2/26/2021
#wahoo https://www.youtube.com/watch?v=-mtcfkWDbOU

.data #labels containing the string constants to be used
	instruct: .asciiz "Enter operation +, -, *, %, or / (type q to quit): \n"
	left: .asciiz "Enter left operand: \n"
	right: .asciiz "Enter right operand: \n"
	equals: .asciiz "="
.text
	addi $sp, $sp, -8 #spills 8 bytes of the stack; because "op" is intialized as an array of chars, we need to allocate enough memory in order to save every value. Since a char is 1 byte, we only need to spill 5 bytes, but the stack must be aligned in multiples of 8, hence spilling 8 bytes.
	j main #jumps to main after preliminary steps like spilling the stack
	
sum: #label to add
	add $t2, $t0, $t1
	j result
dif: #label to subtract
	sub $t2, $t0, $t1
	j result
pro: #label to multiply
	mul $t2, $t0, $t1
	j result
quo: #label to divide
	div $t2, $t0, $t1
	j result
mod: #label for modulus
	rem $t2, $t0, $t1
	j result
	
#all arithmetic labels end with a jump to the result label, to simulate 'break'

quit: #label to end the program
	addi $sp, $sp, 8 #restores stack; simulates delete[]
	li $v0, 17
	li $a0, 0 #simulates exit(0) by using syscall 17
	syscall
main:
	li $v0, 4
	la $a0, instruct #prints instructions for user; simulates printf by using syscall 4 to print a string value that is stored in $a0. By using 'la', we can store the wanted string constant (inintialized in .data) into $a0 in order to print it
	syscall
	
	li $v0, 12
	syscall #stores symbol into $s0
	move $s0, $v0
	sb $s0, 0($sp) #stores register $s0 into stack
	
	beq $s0, 'q', quit #if $s0 is q, moves to label "quit"; beq simulates an if statement
	
	li $v0, 11
	li $a0, '\n' #prints new line to clean up output
	syscall
	
	li $v0, 4
	la $a0, left #asks user for left operand; uses syscall 4 to simulate printf
	syscall
	
	li $v0, 5
	syscall #stores left operand into $t0; simulates cin by using syscall 5 to read an int value, which is stored into $v0. The 'move' instruction copies $v0 into $t0.
	move $t0, $v0
	
	li $v0, 4
	la $a0, right #asks user for right operand; uses syscall 4 to simulate printf
	syscall
	
	li $v0, 5
	syscall #stores right operand into $t1; uses syscall 5 to simulate cin
	move $t1, $v0
	
	#simulates 'switch'; since switch is essentially a series of if(a == b) statements, a series of beq statements can be used instead, since beq can simulate an if statement 
	beq $s0, '+', sum
	beq $s0, '-', dif
	beq $s0, '*', pro
	beq $s0, '/', quo
	beq $s0, '%', mod
	
result: #label to print the full equation; simulates a printf statement that has variables in its parameters. Because variables aren't hardcoded to a certain value, 'li' and 'move' must be used in order to print them
	li $v0, 1
	move $a0, $t0 #prints left operand
	syscall
	
	li $v0, 11
	li $a0, ' ' #prints a space
	syscall
	
	li $v0, 11
	move $a0, $s0 #prints operation symbol
	syscall
	
	li $v0, 11
	li $a0, ' ' #prints a space
	syscall
	
	li $v0, 1
	move $a0, $t1 #prints right operand
	syscall
	
	li $v0, 11
	li $a0, ' ' #prints a space
	syscall
	
	li $v0, 4
	la $a0, equals #prints equal sign
	syscall
	
	li $v0, 11
	li $a0, ' ' #prints a space
	syscall
	
	li $v0, 1
	move $a0, $t2 #prints result of equation
	syscall
	
	li $v0, 11
	li $a0, '\n' #prints newline for next execution of code
	syscall
	
	lb $s0, 4($sp) #unspills stack
	
	j main #returns to beginning of main; simulates while loop
	
	
	
	
	
