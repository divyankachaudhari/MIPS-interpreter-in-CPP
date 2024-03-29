# One thing I'm not sure of is how to directly divide floats without
# initialising new variables, that is why I have made extra variables

# ----------------------------------------------------------------------------
.data

	Intro: 		.ascii 		"Enter the values here\n"
						.asciiz 	"The no. of points: "

	Input: 		.asciiz 	"Enter the coordinates of points after pressing enter each time and less than 2147483000: "
	Output: 	.asciiz		"The total area is: "
	areaf: 		.double 	0.0
	area: 		.word 		0
	total:		.word	 		0
	twoPointZero: 	.double 	2.0


# ----------------------------------------------------------------------------
	# Regsiters used for:
	# f0: double areaf
	# f2: x2-x1 --> x2-x1/2 (x part of area)
	# f4: [(y1)^2+(y2)^2]/(|y1| + |y2|) or |y1| + |y2| (y part of area)
	# f8: twoPointZero
	#----------------
	# t8: First x coordinate
	# t0: Second x coordinate
	# t9: First y coordinate --> its absolute value
	# t1: Second y coordinate
	# t6: absolute value of t1
	#----------------
	# s0: Number of points
	# s1: Index/Counter for loop
	# s2: area (NO NEED OF THIS; ask and delete)
	# s3: Initiliased to 1 for help
	# s4: intMax
# ----------------------------------------------------------------------------

.text
.globl main
.ent main
main:
	# Print the first string Intro
	la 			$a0, 		Intro
	li			$v0, 		4
	syscall

	# Taking user's input of the number of coordinates
	li 			$v0, 		5
	syscall

	# Storing the input in s0 and copying to total
	sw 			$v0, 		total
	move 		$s0, 		$v0

	# s1 will act as our counter for loop, initialising with 1
	li 			$s1, 		1

	# Storing some values in registers
	l.d 		$f8, 		twoPointZero 		# Storing 2.0 to use later in float calculations
	lw 			$s2, 		area						# Total area in word
	l.d 		$f0, 		areaf 					# Total area in double
	li			$s3,		1								# Temporary variable assigned value 1 to check for single

	# Checking if the number of pts is 1 or 0
	# If yes, go to single; else continue
	ble 		$s0,		$s3, 		single

	# Printing the prompt string to get coordinate values
	la 			$a0, 		Input
	li			$v0, 		4
	syscall

	# Storing the first point x coordinate
	li 			$v0, 		5
	syscall
	move 		$t8, 		$v0

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0

>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
	# Storing the first point y coordinate
	li 			$v0, 		5
	syscall
	move 		$t9, 		$v0

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0


>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
# ----------------------------------------------------------------------------
# Loop for calculation
	calculation:

		# Storing subsequent x coordinates (except first) using loop
		li 			$v0, 		5
		syscall
		move 		$t0, 		$v0

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0

>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
		# Storing subsequent y coordinates (except first) using loop
		li 			$v0, 		5
		syscall
		move 		$t1, 		$v0

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0


>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
		# Storing x2-x1/2 in f2
		sub 			$t2, 		$t0, 		$t8			# Storing the value of x2-x1 in an integer register
		mtc1.d 		$t2, 		$f2							# Storing the value of the x2-x1 in a floating point register
		cvt.d.w 	$f2,		$f2
		div.d 		$f2, 		$f2, 		$f8 		# Storing the value of x2-x2/2 in a floating point register

		# Checking if the signs of continuous y coordinates entered are opposite
		abs 		$s6, 		$t1
		abs 		$s7, 		$t9
		div 		$s6,		$t1, 		$s6				# Getting sign of second y
		div 		$s7, 		$t9, 		$s7				# Getting sign of first y
		mul 		$s5, 		$s6,	 	$s7				# Multiplying signs both y coordinates

		# Changing to positive or negative branch on basis of multiplied sign
		bltz		$s5, 		negative					# Opposite signs
		bgez 		$s5, 		positive					# Same signs

# NEGATIVE-------
		negative:
			# Making both y coordinates absolute
			abs				$t6, 		$t1
			abs 			$t9, 		$t9

			# Obtaining |y1| + |y2| and storing in f4.
			mtc1.d 		$t6, 		$f16
			mtc1.d		$t9,		$f14
			cvt.d.w 	$f14, 		$f14
			cvt.d.w 	$f16, 		$f16
			add.d		$f4, 		$f16, 		$f14


			# Obtaining (y1)^2, (y2)^2 and (y1)^2+(y2)^2. Storing in f6.

			mul.d			$f16,		$f16,		$f16			# y1^2
			mul.d			$f14, 		$f14,		$f14			# y2^2
			add.d			$f6, 		$f16,	 	$f14			# y1^2+y2^2

			# Obtaining [(y1)^2+(y2)^2]/(|y1| + |y2|). Storing in f4.
			div.d 		$f4, 		$f6, 		$f4

			j 			extras

# POSITIVE-------
		positive:
			# Making both y coordinates absolute
			abs 			$t6, 		$t1
			abs				$t9,	 	$t9

			# Obtaining |y1| + |y2| and storing in f4.
			mtc1.d 		$t6, 		$f16
			mtc1.d		$t9,		$f14
			cvt.d.w 	$f14, 		$f14
			cvt.d.w 	$f16, 		$f16
			add.d		$f4, 		$f16, 		$f14

# EXTRAS--------
		extras:
			# In f2, we had (x2-x1)/2. Multipying that with either
			# [(y1)^2+(y2)^2]/(|y1| + |y2|) or |y1| + |y2|
			# depending on our branch. Storing in f2.
			mul.d 		$f2, 		$f2, 		$f4

			# Increasing our loop counter in s1.
			add 			$s1, 		$s1,		1

			# Adding our area obtained with 2 points (in f2) to previous total area (in f0).
			add.d 		$f0, 		$f0, 		$f2

			# We will now shift our second set of coordinates (t0, t1) into first set registers (t8, t9)
			# Then, we will input new set of coordinates (in t0, t1) by loop to calculate area and add.
			move			$t8, 		$t0
			move			$t9, 		$t1

			# Looping back
			blt 		$s1, 		$s0, 		calculation

	.end calculation
# ----------------------------------------------------------------------------

# Ouput 0 if the number of points is 1 or 0.
	single:
	.end single

<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
=======

>>>>>>> de6c9b77e37d090550fd4561c2aaa81e430363e0
# ----------------------------------------------------------------------------

	# Storing value in f0 to areaf to print.
	s.d 		$f0, 		areaf

	# Printing the final double value of area.
	la 			$a0, 		Output
	li			$v0, 		4
	syscall
	l.d 		$f12,		areaf
	li 			$v0, 		3
	syscall
	jr	$ra

# ----------------------------------------------------------------------------

	li $v0, 10 			# Terminate
	syscall 			# System call
.end main
