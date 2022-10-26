#Alex Yu
#Assembly Structures Lab
#Chris Muncey and Zach McMichael
#3/7/2021
#parallel universes https://www.youtube.com/watch?v=qB1ucmJSgRE

#NOS Table: values
#Name | Offset | Size
#operation | 0 | 1
#left | 2 | 2
#right | 4 | 2
#result | 8 | 4

#Matrix3 stack size: 40
#Vector3 stack size: 16

.text
gofunc:
	#Memory Layout:
	#Value values[] | $a0
	#int num_values | $a1
	#int i | $a2
	#int counter | $a3
	#values[i].operation | $t0
	#values[i].left | $t1
	#values[i].right | $t2
	#values[i].result | $t3
	
	#preliminary code for gofunc
	add $a0, $sp, $zero #sets array to beginning
	move $a2, $zero #int i = 0
	move $a3, $zero #int counter = 0
	
	#code to loop
	funcloop:
		beq $a2, $a1, funcloopbreak #ends loop when i = num_values
		sll $a2, $a2, 4 #multiplies i by 16 (size of Values struct) for proper traversal of values[] array
		add $a0, $sp, $a2 #sets values[] to location i
		lb $t0, 0($a0) #loads values[i].operation
		lh $t1, 2($a0) #loads values[i].left
		lh $t2, 4($a0) #loads values[i].right
		
		#simulates switch statement using branches and labels
		beq $t0, '+', sum
		beq $t0, '-', dif
		beq $t0, '*', pro
		beq $t0, '/', quo
		beq $t0, '%', mod
		beq $t0, '&', amp
		beq $t0, '|', ver
		beq $t0, '^', car
		beq $t0, '~', til
		
		#if values[i].operation is invalid, sets values[i].result to zero
		move $t3, $zero
		j funcloopend
	sum: 
		add $t3, $t1, $t2 #result = left + right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	dif:
		sub $t3, $t1, $t2 #result = left - right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	pro:
		mul $t3, $t1, $t2 #result = left * right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	quo:
		div $t3, $t1, $t2 #result = left / right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	mod:
		rem $t3, $t1, $t2 #result = left % right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	amp:
		and $t3, $t1, $t2 #result = left & right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	ver:
		or $t3, $t1, $t2 #result = left | right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	car:
		xor $t3, $t1, $t2 #result = left ^ right
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	til:
		not $t3, $t1 #result = ~left
		addi $a3, $a3, 1 #++counter
		j funcloopend #break
	funcloopend:
		srl $a2, $a2, 4 #shifts i back to original number
		addi $a2, $a2, 1 #++i
		sw $t3, 8($a0) #stores values[i].result into stack at location i
		j funcloop
	funcloopbreak:
		li $v0, 1 
		move $a0, $a3 #prints value of counter
		syscall
		jr $ra
		
matmul:
	#Memory Layout:
	#Vector3 result | $a0
	#Matrix3 mat | $a1
	#Vector3 vec | $a2
	#int i | $t0
	#int j | $t1
	#int v | $t2
	#result.vec[i] | $t3
	#mat.mat[j] | $t4
	#vec.vec[v] | $t5
	
	#preliminary code for matmul
	add $a0, $sp, $zero #sets array to beginning
	move $t0, $zero #int i = 0
	move $t1, $zero #int j = 0
	move $t2, $zero #int v = 0
	
	#first loop
	matrixloop:
		beq $t0, 3, matrixloopbreak #ends loop when i = 3
		move $t6, $zero #resets $t6 register for further use
		move $t2, $zero #v = 0
		mul $t1, $t0, 3 #j = i * 3
		addi $t6, $t1, 3 #sets $t6 to end value, (i*3) + 3
		sll $t0, $t0, 2 #multiplies i by 4 (size of int) for proper array traversal 
		add $a0, $sp, $t0 #sets result array to correct location
		addi $a2, $sp, 56 #sets vec array to starting location in the stack
		
		#nested loop
		matrixloop2:
			beq $t1, $t6, matrixloopend #ends loop when j = (i*3) + 3
			sll $t1, $t1, 2 #multiplies j by 4 (size of int) for proper array traversal 
			sll $t2, $t2, 2 #multiplies v by 4 (size of int) for proper array traversal 
			addi $t1, $t1, 16 #adds 16 (the starting position of mat[] in the stack) to j 
			addi $t2, $t2, 56 #adds 56 (the starting position of vec[] in the stack) to v 
			add $a1, $sp, $t1 #sets mat[] to location j in the stack
			add $a2, $sp, $t2 #sets vec[] to location v in the stack
			lw $t4, 0($a1) #loads mat[j] into mat.mat[j]
			lw $t5, 0($a2) #loads vec[v] into vec.vec[v]
			mul $t7, $t4, $t5 #mat.mat[j] * vec.vec[v]
			add $t3, $t3, $t7 #result.vec[i] = ^^
			sw $t3, 0($a0) #stores result.vec[i] into location i in the stack
			sub $t1, $t1, 16 #subtracts 16 from j as first step in returning j to original value
			sub $t2, $t2, 56 #subtracts 56 from v as first step in returning v to original value
			srl $t1, $t1, 2 #shifts j to the right twice to return it to original value
			srl $t2, $t2, 2 #shifts v to the right twice to return it to original value
			addi $t1, $t1, 1 #++j
			addi $t2, $t2, 1 #++v
			j matrixloop2
		matrixloopend:
			move $t3, $zero #resets $t3 for further use
			srl $t0, $t0, 2 #shifts i right twice to return it to original value
			addi $t0, $t0, 1 #++i
			j matrixloop
	matrixloopbreak:
		jr $ra