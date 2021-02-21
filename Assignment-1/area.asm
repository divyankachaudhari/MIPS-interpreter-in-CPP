.data
	
	Intro: 		.ascii 		"Enter the values here\n"
				.asciiz 	"The no. of points: "

	Input: 		.asciiz 	"Enter the coordinates of points as space separated integer values: "
	areaf: 		.double 	0.0
	area: 		.integer 	0
	total:		.integer 	0

.text
.globl main
.ent main
main:
	
	la 			$a0, 		Intro 	#printing the initial string
	li			$v0, 		4
	syscall

	li 			$v0, 		5 		#storing the total number of points	in s0
	syscall
	sw 			$v0, 		total
	move 		$s0, 		$v0

	li 			$s1, 		1		#counter increment
	lw 			$s2, 		area
	l.d 		$f0, 		areaf 	#total area in double

	li 			$v0, 		5 		#storing the first point x coordinate
	syscall
	move 		$s3, 		$v0 

#To do: Add a case for the number of points 1, throw 0

	li 			$v0, 		5		#storing the first point y coordinate
	syscall
	move 		$s4, 		$v0


#To do: Account for negative numbers

	calculation:

		li 			$v0, 		5						#storing the x coordinate of the next point in the loop
		syscall		
		move 		$t0, 		$v0					

		li 			$v0, 		5						#storing the y coordinate of the next point in the loop
		syscall
		move 		$t1, 		$v0					

		subu 		$t2, 		$t0, 		$s3			#storing the value of x2-x1 in an integer register
		mtc1.d 		$t2, 		$f2						#storing the value of the x2-x1 in a floating point register
		mul.d 		$f2, 		$f2, 		0.5 		#getting the value of (x2-x1)/2 in that floating point register

		sum			$t3, 		$t1, 		$s4 		#storing y1+y2
		mtc1.d 		$t3, 		$f4						#storing y1+y2 in floating point register
		mul.d 		$f2, 		$f2, 		$f4 		#storing the final value

		li 			$s1, 		$s1,		1			#increasing the counter
		add.d 		$f0, 		$f0, 		$f2 		#adding this value to the total sum
		blt 		$s1, 		$s0, 		calculation #looping back into the code

	.end calculation

	li 			$v0, 		3		#printing the final double value
	l.d 		$f12,		$f0 
	syscall






