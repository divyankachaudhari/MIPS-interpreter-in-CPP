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

	la 			$a0, 		Input 	#printing the prompt string
	li			$v0, 		4
	syscall

	li 			$s1, 		1		#counter increment
	l.d 		$f4, 		helper 	#storing the value that will help in division
	lw 			$s2, 		area
	l.d 		$f0, 		areaf 	#total area in double

	li 			$v0, 		5 		#storing the first point x coordinate
	syscall
	move 		$s3, 		$v0 

#To do: Add a case for the number of points 1, throw 0

	li 			$v0, 		5		#storing the first point y coordinate
	syscall
	move 		$s4, 		$v0


#To do: Account for negative numbers  (formula for one negative one positive (x2-x1)(y1^2+y2^2)/2(|y1|+|y2|))

	calculation:

		li 			$v0, 		5						#storing the x coordinate of the next point in the loop
		syscall		
		move 		$t0, 		$v0					

		li 			$v0, 		5						#storing the y coordinate of the next point in the loop
		syscall
		move 		$t1, 		$v0					

#To do: My code is currently always subtracting from $s3 which means the initial point which is wrong

		subu 		$t2, 		$t0, 		$s3			#storing the value of x2-x1 in an integer register
		mtc1.d 		$t2, 		$f2						#storing the value of the x2-x1 in a floating point register
		div.d 		$f2, 		$f2, 		$f4 		#storing the value of x2-x2/2 in a float

		add			$t3, 		$t1, 		$s4 		#storing y1+y2
		mtc1.d 		$t3, 		$f4						#storing y1+y2 in floating point register
		mul.d 		$f2, 		$f2, 		$f4 		#storing the final value

		add 		$s1, 		$s1,		1			#increasing the counter
		add.d 		$f0, 		$f0, 		$f2 		#adding this value to the total sum
		blt 		$s1, 		$s0, 		calculation #looping back into the code

	.end calculation
		s.d 		$f0, 		areaf					#storing value into variable for printing

	l.d 		$f12,		areaf			#printing the final double value
	li 			$v0, 		3		
	syscall

#To do: printing the wrong value which I'm not sure why (would have to look at code or individual registers, for which I'm not in the right state of mind)






