#IO
#Prompts user to input 10 ascii digits into an array
#Converts the string of digits into a single int
#Also handles any number of digits between 1 and 10
#Returns 0 if non-digit chars are entered into the string

.data                   #declaration of vars follows
array:  .space 11        #reserves space for a 10 elem array
char:   .space 2
prompt: .asciiz "Please enter 10 numbers, then press ENTER:  \n"
null:   .asciiz ""
space:  .ascii " "
newline:.asciiz "\n"
.text                   #instructions follow

main:
la $a0, prompt          #load prompt message into $a0 for syscall
li $v0, 4               #load syscall to print string
syscall                 #print prompt message
j readInt               #call readInt function to get user input string

gets:                   #read multiple chars from keyboard buffer until ENTER key,
                        #add NULL char and store into buffer pointed to by *array
                        #passed to the subroutine
la $s1, array           #set base address of array to s1
loop:                   #start of read loop
jal getc                #jump to getc subroutine
lb $t0, char            #load the char from char buffer into t0, stripping null
sb $t0, 0($s1)          #store the char into the nth elem of array
lb $t1, newline         #load newline char into t1
beq $t0, $t1, done      #end of string?  jump to done
addi $s1, $s1, 1        #increments base address of array
j loop                  #jump to start of read loop

getc:                   #read char from keyboard buffer and return ascii value
li $v0, 8               #call code for read string
la $a0, char            #load address of char for read
li $a1, 2               #length of string is 1byte char and 1byte for null
syscall                 #store the char byte from input buffer into char
jr $ra                  #jump-register to calling function

readInt:                #read string of ascii digits, store into a local variable and
                        #convert into integer, return that int unless string contains
                        #non-integers
j gets                  #let s1 be top address of array, let s0 be the digitcounter
done:                   #let s2 be the sum total
addi $s1, $s1, -1       #reposition array pointer to last char before newline char
la $s0, array           #set base address of array to s0 for use as counter
addi $s0, $s0, -1       #reposition base array to read leftmost char in string
add $s2, $zero, $zero   #initialize sum to 0
li $t0, 10              #set t0 to be 10, used for decimal conversion
li $t3, 1
lb $t1, 0($s1)         #load char from array into t1
blt $t1, 48, error     #check if char is not a digit (ascii<'0')
bgt $t1, 57, error     #check if char is not a digit (ascii>'9')
addi $t1, $t1, -48     #converts t1's ascii value to dec value
add $s2, $s2, $t1      #add dec value of t1 to sumtotal
addi $s1, $s1, -1      #decrement array address
lp:                    #loop for all digits preceeding the LSB
mul $t3, $t3, $t0      #multiply power by 10
beq $s1, $s0, FIN      #exit if beginning of string is reached
lb $t1, ($s1)          #load char from array into t1
blt $t1, 48, error     #check if char is not a digit (ascii<'0')
bgt $t1, 57, error     #check if char is not a digit (ascii>'9')
addi $t1, $t1, -48     #converts t1's ascii value to dec value
mul $t1, $t1, $t3      #t1*10^(counter)
add $s2, $s2, $t1      #sumtotal=sumtotal+t1
addi $s1, $s1, -1      #decrement array address
j lp                   #jump to start of loop

error:                 #if non digit chars are entered, readInt returns 0
add $s2, $zero, $zero
j FIN

FIN:
li $v0, 1
add $a0, $s2, $zero
syscall
li $v0, 10             #ends programla $a0, prompt          #load prompt message into $a0 for syscall
li $v0, 4               #load syscall to print string
syscall                 #print prompt message
j readInt               #call readInt function to get user input string

gets:                   #read multiple chars from keyboard buffer until ENTER key,
                        #add NULL char and store into buffer pointed to by *array
                        #passed to the subroutine
la $s1, array           #set base address of array to s1
loop:                   #start of read loop
jal getc                #jump to getc subroutine
lb $t0, char            #load the char from char buffer into t0, stripping null
sb $t0, 0($s1)          #store the char into the nth elem of array
lb $t1, newline         #load newline char into t1
beq $t0, $t1, done      #end of string?  jump to done
addi $s1, $s1, 1        #increments base address of array
j loop                  #jump to start of read loop

getc:                   #read char from keyboard buffer and return ascii value
li $v0, 8               #call code for read string
la $a0, char            #load address of char for read
li $a1, 2               #length of string is 1byte char and 1byte for null
syscall                 #store the char byte from input buffer into char
jr $ra                  #jump-register to calling function

readInt:                #read string of ascii digits, store into a local variable and
                        #convert into integer, return that int unless string contains
                        #non-integers
j gets                  #let s1 be top address of array, let s0 be the digitcounter
done:                   #let s2 be the sum total
addi $s1, $s1, -1       #reposition array pointer to last char before newline char
la $s0, array           #set base address of array to s0 for use as counter
addi $s0, $s0, -1       #reposition base array to read leftmost char in string
add $s2, $zero, $zero   #initialize sum to 0
li $t0, 10              #set t0 to be 10, used for decimal conversion
li $t3, 1
lb $t1, 0($s1)         #load char from array into t1
blt $t1, 48, error     #check if char is not a digit (ascii<'0')
bgt $t1, 57, error     #check if char is not a digit (ascii>'9')
addi $t1, $t1, -48     #converts t1's ascii value to dec value
add $s2, $s2, $t1      #add dec value of t1 to sumtotal
addi $s1, $s1, -1      #decrement array address
lp:                    #loop for all digits preceeding the LSB
mul $t3, $t3, $t0      #multiply power by 10
beq $s1, $s0, FIN      #exit if beginning of string is reached
lb $t1, ($s1)          #load char from array into t1
blt $t1, 48, error     #check if char is not a digit (ascii<'0')
bgt $t1, 57, error     #check if char is not a digit (ascii>'9')
addi $t1, $t1, -48     #converts t1's ascii value to dec value
mul $t1, $t1, $t3      #t1*10^(counter)
add $s2, $s2, $t1      #sumtotal=sumtotal+t1
addi $s1, $s1, -1      #decrement array address
j lp                   #jump to start of loop

error:                 #if non digit chars are entered, readInt returns 0
add $s2, $zero, $zero
j FIN

FIN:
syscall
