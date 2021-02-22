.data
	
	Intro: 		.ascii 		"Enter the values here\n"
				.asciiz 	"The no. of points: "

	Input: 		.asciiz 	"Enter the coordinates of points after pressing enter each time : "
	areaf: 		.double 	0.0
	area: 		.word 		0
	total:		.word	 	0
	helper: 	.double 	2.0	

#One thing I'm not sure of is how to directly divide floats without initialising new variables, that 
#is why I have made extra variables

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
	l.d 		$f8, 		helper 	#storing the value that will help in division
	lw 			$s2, 		area
	l.d 		$f0, 		areaf 	#total area in double
	li			$s3,		1		#temporary variable to check for single or less than 1 points

	ble 		$s0,		$s3, 		single	#cheking if the no. of points is 1


	la 			$a0, 		Input 	#printing the prompt string
	li			$v0, 		4
	syscall

	li 			$v0, 		5 		#storing the first point x coordinate
	syscall
	move 		$t8, 		$v0 

	li 			$v0, 		5		#storing the first point y coordinate
	syscall
	move 		$t9, 		$v0

	calculation:


		li 			$v0, 		5						#storing the x coordinate of the next point in the loop
		syscall		
		move 		$t0, 		$v0					

		li 			$v0, 		5						#storing the y coordinate of the next point in the loop
		syscall
		move 		$t1, 		$v0					

		sub 		$t2, 		$t0, 		$t8			#storing the value of x2-x1 in an integer register
		mtc1.d 		$t2, 		$f2						#storing the value of the x2-x1 in a floating point register
		cvt.d.w 	$f2,		$f2
		div.d 		$f2, 		$f2, 		$f8 		#storing the value of x2-x2/2 in a float

		mul 		$t6, 		$t1,	 	$t9			#checking if the values of the coordinates are opposite
		bltz		$t6, 		negative				#changing the branch to account for values with opp signs
		bgez 		$t6, 		positive

		negative:
			abs			$t6, 		$t1
			abs 		$t9, 		$t9
			add 		$t4, 		$t6, 		$t9			#storing the value of y1+y2 after taking absolute
			mtc1.d 		$t4, 		$f4
			cvt.d.w 	$f4, 		$f4

			mul			$s4,		$t6,		$t6			#y1^2
			mul			$t9, 		$t9,		$t9			#y2^2
			add 		$t4, 		$s4,	 	$t9			#y1^2+y2^2
			mtc1.d 		$t4, 		$f6
			cvt.d.w 	$f6, 		$f6
			div.d 		$f4, 		$f6, 		$f4

			j 			extras

		positive:
		
			abs 		$t6, 		$t1
			abs			$t9,	 	$t9
			add			$t4, 		$t6, 		$t9			#storing y1+y2
			mtc1.d 		$t4, 		$f4						#storing y1+y2 in floating point register
			cvt.d.w 	$f4, 		$f4		

		extras:	
			mul.d 		$f2, 		$f2, 		$f4 		#storing the final value
			add 		$s1, 		$s1,		1			#increasing the counter
			add.d 		$f0, 		$f0, 		$f2 		#adding this value to the total sum

			move		$t8, 		$t0						#storing the value of the point in this loop in t8, t9
			move		$t9, 		$t1
			
			blt 		$s1, 		$s0, 		calculation #looping back into the code

	.end calculation

	single:
	.end single

		
	s.d 		$f0, 		areaf			#storing value into variable for printing
	l.d 		$f12,		areaf			#printing the final double value
	li 			$v0, 		3		
	syscall
	jr	$ra






