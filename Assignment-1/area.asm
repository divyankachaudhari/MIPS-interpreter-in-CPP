.data
	
	Intro: 		.ascii 		"Enter the values here\n"
				.asciiz 	"The no. of points: "

	Input: 		.asciiz 	"Enter the coordinates of points as space separated integer values: "
	temp: 		.double 	0.0
	area: 		.integer 	0
	total:		.integer 	0

.text
.globl main
.ent main
main:
	
	la 		$a0, 		Intro 	#printing the initial string
	li		$v0, 		4
	syscall

	li 		$v0, 		5 		#storing the total number of points	in s0
	syscall
	sw 		$v0, 		total
	move 	$s0, 		$v0

	li 		$s1, 		1		#counter increment
	lw 		$s2, 		area
	l.d 	f0, 		temp 	#total area in double

	li 		$v0, 		5 		#storing the first point x coordinate
	syscall
	move 	$s3, 		$v0 

#To do: Add a case for the number of points 1, throw 0

	li 		$v0, 		5		#storing the first point y coordinate
	syscall
	move 	$s4, 		$v0


#To do: Account for negative numbers

	calculation:

		li 			$v0, 		5
		syscall		
		move 		$t0, 		$v0

		li 			$v0, 		5
		syscall
		move 		$t1, 		$v0

		subu 		$t2, 		$t1, 		$s3		#storing the value of x2-x1 in an integer register
		mtc1.d 		$t2, 		$f2					#storing the value of the sum in a floating point register
		






