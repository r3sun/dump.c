# dump.c
 
## NAME

dump -- convert between integers, binary, hexadecimals and octals

## SYNOPISIS

dump -key [input]

## DESCRIPTION

> dump takes an number as input and print the number in the specified numeral system. The *key* is a string contains at most one function letter defining the immediate next argument plus letters that defines the base of input and output number. Other arguments to the command are either input or a file containing the input.

>The function letter is one of the following letters:

> > **a** the next argument is treated as the input

> > **f** the next argument is treated as the path of a file that contains the input

> The following letters represent different base:

> > **b** binary

> > **i** integers

> > **h** hexadecimals

> > **o** octals

> **a** and **f** can not be used simultaneously. In the same way none of the letters representing the base can not be repeted.

## EXAMPLES

> dump aib 512

> > here,

> > **a** tells that 512 is the input

> > **i** tells that it is in integer base

> > **b** tells that it must be converted to binary

> dump fib dump.c.

> > here,

> > f tells that dump.c is a file containing the input

> > i tells that each character of the file is in integer base

> > b tells that each character of the file must be converted to binary

> to get the binary, hexadecimal and octal representation of an ASCII character just type ai in the *key* string. As ASCII characters are all valid integers in the range 0 to 256 so their binary, hexadecimal and octal representation is the same as that of the integers in that range.

## BUGS

> Inputing invalid number will print error following the input character. Characters to distinguish different base are invalid, dump will ignore them.