#| Input: Postfix expression with constant integer operands in the range 0-­9 and
#| operators +, -,and *.
#| Output: Print the result of the expression.
#-------------------------------------------------------------------------------
#| Read about:
#| 1. Inputting and printing string from the book.
#| 2. Stack from book.
#| 3. Converting ASCII char to int.
#-------------------------------------------------------------------------------
#| Method:
#| 1. Read string inputted.
#| 2. Start looping through each char:
#|    (a) If the char is operator -> pop 2 numbers from stack and operate -> push the result into stack
#|    (b) If the char is NOT operaor (is a number) -> convert to int -> push into stack
#| 3. Loop until the end of string (here length = 256 (I decided this, can change)).
#| 4. In the end, only result will be left in the stack. Pop and output it.
#-------------------------------------------------------------------------------
#| Register usage:
#| $a0: Start address of the string
#| $a1: Address pointer for string (to read char)
#| $t1: Counter for loop (i)
#| $t0: Temporary storage of each char read from string
#| $t2: Size of the string (fixed here, 256)
#| $t3: Stores the total number of integers in the input string
#| $t4: Stores the total number of operators in the input string
#| $t5: Helps in checking if the postfix expression is correct
#| $s1: Temporary storage of result after each operation to push into stack
#| $s2: '+' (For comparison in STEP 2 in method above)
#| $s3: '-'
#| $s4: '*'
#| $s5: Storage of popped number1 during operation
#| $s6: Storage of popped number2 during operation
#| $s7: Storing of final result after popping to output.
#-------------------------------------------------------------------------------
#| Doubts:
#|  1. Int is 4 bytes and char is 1 byte. So, while updating address of string I add 1 byte to it.
#|   But, to update stack of integers, I add 4 bytes to it. Don't know if any errors could be caused.
#|  2. Can't compile due to errors idk what? Am I stupid? Maybe. Hmmmmmmm. is my method wrong? ;~;

# -----------------------------------------------------------------------------
#| Data declarations
.data
InputPrompt:  .asciiz "Please enter the expression: "
OutputPrompt: .asciiz "The result is: "
userAns:      .space   1028
length:       .word    1028
result:       .word    0
checker:      .word    0
Invalid:      .asciiz  "Invalid Postfix expression"

# -----------------------------------------------------------------------------
#| Code

.text
.globl main
.ent main
#-----------
main:

  #| Printing the initial prompt to take input
  li $v0, 4
  la $a0, InputPrompt
  syscall

  #| Inputting the user's string
  li $v0, 8                 # call code, read string
  la $a0, userAns           # addr, where to put chars
  li $a1, 1028               # max chars for string
  syscall

  #| Remember, the first address of our string is stored at $a0. Copying to $a1 for loop.
  la $a1, userAns

  #| To check for errors in the postfix expression
  #| Checks the number of integers
  li $t3, 0

  #| Checks the number of characters initialised to 1 so that direct compare
  li $t4, 1

  li $t5, 10
  #| Size of one char is one byte.
  #| Pushing the elements of string into a stack in 2 steps:
  #|  1. Converting the ASCII char to int.
  #|  2. Pushing the int to our stack. Loop.

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
  #nop
  beq $t0, $s3, subtract # '-'
  #nop
  beq $t0, $s4, multiply # '*'
  #nop

  #| Continuing...
  #| Converting the ASCII char loaded in $t0 to int.
  #| From: https://stackoverflow.com/questions/15940331/convert-string-of-ascii-digits-to-int-in-mips-assembler
  #| Please check if this is the only step required, from answer 1 in given link
  add $t0, $t0, -48

  #|Incrementing the no. of integers counter
  add $t3, $t3, 1

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
  j ending

#--------
addition:

  #| Pop the last 2 numbers from the stack
  lw $s5, ($sp)
  lw $s6, 4($sp)
  addu $sp, $sp, 8

  #| Incrementing the number of operators
  add $t4, $t4, 1

  #| Operate on those 2 numbers and store temporarily in $s1
  add $s1, $s5, $s6

  #| Push the result back into stack
  subu $sp, $sp, 4
  sw   $s1, ($sp)


  #| Updating loop parameters
  add $t1, $t1, 1
  add $a1, $a1, 1

  lb $t0, ($a1)

  beq $t0, $s2, addition # '+'
  #nop
  beq $t0, $s3, subtract # '-'
  #nop
  beq $t0, $s4, multiply # '*'
  #nop
  bne $t0, $t5, pushLoop
  j ending

#---------
subtract:
  #| Pop the last 2 numbers from the stack
  lw    $s5, ($sp)
  lw    $s6, 4($sp)
  addu  $sp, $sp, 8

  #| Incrementing the number of operators
  add $t4, $t4, 1

  #| Operate on those 2 numbers and store temporarily in $s1
  sub $s1, $s6, $s5

  #| Push the result back into stack
  subu $sp, $sp, 4
  sw   $s1, ($sp)

  #| Updating loop parameters
  add $t1, $t1, 1
  add $a1, $a1, 1

  lb $t0, ($a1)

  beq $t0, $s2, addition # '+'
  #nop
  beq $t0, $s3, subtract # '-'
  #nop
  beq $t0, $s4, multiply # '*'
  #nop
  bne $t0, $t5, pushLoop

  j ending

#---------
multiply:
  #| Pop the last 2 numbers from the stack
  lw $s5, ($sp)
  lw $s6, 4($sp)
  addu $sp, $sp, 8

  #| Incrementing the number of operators
  add $t4, $t4, 1

  #| Operate on those 2 numbers and store temporarily in $s1
  mul $s1, $s5, $s6


  #| Push the result back into stack
  subu $sp, $sp, 4
  sw   $s1, ($sp)


  #| Updating loop parameters
  add $t1, $t1, 1
  add $a1, $a1, 1

  lb $t0, ($a1)

  beq $t0, $s2, addition # '+'
  #nop
  beq $t0, $s3, subtract # '-'
  #nop
  beq $t0, $s4, multiply # '*'
  #nop
  bne $t0, $t5, pushLoop
  j ending
#---------
ending:
  
  bne $t3, $t4, incorrect

  lw $s1, ($sp)
  addu $sp, $sp, 4

  #| Storing value in result to print
  sw $s1, result

  #| Printing OutputPrompt and result
  la 			$a0, 		OutputPrompt
  li			$v0, 		4
  syscall
  lw      $a0, result
  li      $v0, 1
  syscall
  jr	$ra

  j final

.end ending

incorrect:
  la      $a0,    Invalid
  li      $v0,    4
  syscall

final:
.end final

#------------------------------------------------------------------------------
#| End of the Code

  li $v0, 10
  syscall
.end main
