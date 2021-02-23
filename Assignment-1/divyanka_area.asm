# Name and general description of program
# ----------------------------------------
# Data declarations go in this section.


.data


# program specific data declarations
# ----------------------------------------


# Program code goes in this section.
.text
.globl    main
.ent      main
main:
# -----
# your program code goes here.

# We will use the following registers for storing our data
# t0 - array address
# t1 - count of elements(co-ordinates)
# s2 - area
# s3 - x coordinate from array
# s4 - y coordinate from array


# -----
# Done, terminate program.

  li $v0, 10
  syscall                         # all done!
.end main
