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

	li 		$v0, 		5 		#storing the integer	
	syscall
	sw 		$v0, 		total

	li 		$s0, 		1		#counter increment
	lw 		$s1, 		temp

	calculation:

		li 			$v0, 		5
		syscall







