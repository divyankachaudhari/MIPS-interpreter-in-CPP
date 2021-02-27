#| Input: Postfix expression with constant integer operands in the range 0­9 and
#| operators +, -,and *.
#| Output: Print the result of the expression.


#| Method:
#| 1. Read string inputted.
#| 2. Start looping through each char:
#|    (a) If the char is operator -> pop 2 numbers from stack and operate -> push the result into stack
#|    (b) If the char is NOT operator (number) -> convert to int -> push into stack
#| 3. Loop until the end of string (here length = 256 (I decided this, can change)).
#| 4. In the end, only result will be left in the stack. Pop and output it.


#| Register usage:
#| $a0: Start address of the string
#| $a1: Address pointer for string (to read char)
#| $t1: Counter for loop (i)
#| $t0: Temporary storage of each char read from string
#| $t2: Size of the string (fixed here, 256)
#| $s1: Temporary storage of result after each operation to push into stack
#| $s2: '+' (For comparison in STEP 2 in method above)
#| $s3: '-'
#| $s4: '*'
#| $s5: Storage of popped number1 during operation
#| $s6: Storage of popped number2 during operation
#| $s7: Storing of final result after popping to output.


# -----------------------------------------------------------------------------
#| Data declarations
InputPrompt:  .asciiz "Please enter the expression: "
OutputPrompt: .asciiz "The result is: "
userAns:      .space   256
length:       .word    256
result:       .word    0
# -----------------------------------------------------------------------------
#| Registers being used for:
#|
# -----------------------------------------------------------------------------
#| Code

.text
.globl main
.ent main
main:

  #| Printing the initial prompt to take input
  li $v0, 4
  la $a0, InputPrompt
  syscall

  #| Inputting the user's string
  li $v0, 8                 # call code, read string
  la $a0, userAns           # addr, where to put chars
  li $a1, 52                # max chars for string
  syscall

  #| Remember, the first address of our string is stored at $a0. Copying to $a1 for loop.
  la $a1, userAns

  #| Size of one char is one byte.
  #| Pushing the elements of string into a stack in 2 steps:
  #|  1. Converting the ASCII char to int.
  #|  2. Pushing the int to out stack. Loop.

  #| $t1 will act as counter for loop until it's equal to $t2.
  li $t1, 0
  lw $t2, length

  #| Storing operators in registers for comparison
  li $s2, '+'
  li $s3, '-'
  li $s4, '*'

  #| Storing result on each operation in $s1.
  li $s1, 0
#--------
pushLoop:

  #| Loading the the first char byte in $t0
  lb $t0, ($a1)

  #| Check if the char is an operator, if yes jump to that. If no, continue.
  beq $t0, $s2, addition # '+'
	nop
	beq $t0, $s3, subtract # '-'
	nop
	beq $t0, $s4, multiply # '*'
  nop

  #| Continuing...
  #| Converting the ASCII char loaded in $t0 to int.
  #| From: https://stackoverflow.com/questions/15940331/convert-string-of-ascii-digits-to-int-in-mips-assembler
  andi $t0, $t0, 0x0F

  #| Pushing it into our stack since int is 4 bytes
  subu $sp, $sp, 4
  sw   $t0, ($sp)

  #| i = i+1
  add $t1, $t1, 1

  #| Update the address by 1 byte since char is 1 byte
  add $a1, $a1, 1

  #| Continue until i = length
  blt $t1, $t2, pushLoop

  #| Else jump to end
  j end

#--------
addition:

  lw $s5, ($sp)
  lw $s6, 4($sp)
  addu $sp, $sp, 8

  add $s1, $s5, $s6

  subu $sp, $sp, 4
  sw   $s1, ($sp)

  #| Updating loop parameters
  add $a1, $a1, 1
  add $a1, $a1, 1
  blt $t1, $t2, pushLoop
  j end

#---------
subtract:
  lw    $s5, ($sp)
  lw    $s6, 4($sp)
  addu  $sp, $sp, 8

  sub $s1, $s5, $s6

  subu $sp, $sp, 4
  sw   $s1, ($sp)

  #| Updating loop parameters
  add $a1, $a1, 1
  add $a1, $a1, 1
  blt $t1, $t2, pushLoop
  j end

#---------
multiply:
  lw $s5, ($sp)
  lw $s6, 4($sp)
  addu $sp, $sp, 8

  mul $s1, $s5, $s6

  subu $sp, $sp, 4
  sw   $s1, ($sp)

  #| Updating loop parameters
  add $a1, $a1, 1
  add $a1, $a1, 1
  blt $t1, $t2, pushLoop
  j end
#---------
end:
  lw $s1, ($sp)

  #| Storing value in result to print
  sw $s1, result

  la 			$a0, 		OutputPrompt
	li			$v0, 		4
	syscall
  lw      $s7, result
  li      $v0, 3
  syscall
  jr	$ra

#------------------------------------------------------------------------------
#| End of the Code

  li $v0, 10
  syscall
.end main