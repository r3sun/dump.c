# dump
 
## NAME

`dump` -- convert between different numbers

## SYNOPISIS

`dump -[bihov] -[nf] input -[l] length`

## DESCRIPTION

`dumps` file or argument in different numeral system as defined by the first argument. The function letters defines:

**`b`** binary

**`i`** integers

**`h`** hexadecimals

**`o`** octals

**`n`** the argument immediatly after is treated as the input.

**`f`** the argument immediatly after is treated as the path of a file that contains the input.

**`l`** the argument immediatly after is the length of number per line (files only).

**`v`** Normally `dump` does its work silently. The **`v`** (verbose) option cause it to prints details about the output.

In the letter(s) defining the base first letter is treated as the input base and if the letter immediatly after that defines a base then it is treated as the output base. If output base is not defined in case of numbers it prints the number in all the other bases, in case of file binary is choosen.

**`n`** and **`f`** should not be used simultaneously. If used only the action of **`n`** will be done. In the same way none of the letters representing the base can not be repeted.

## EXAMPLES

`dump -ib -n 512`

here,

**`n`** tells that 512 is the input number.

**`i`** tells that the number is in integer base.

**`b`** tells that the number must be converted to binary.

`dump -ibv -f dump.c`

here,

**`f`** tells that dump.c is a file containing the input.

**`i`** tells that each character of the file is in integer base.

**`b`** tells that each character of the file must be converted to binary.

**`v`** tells that more details must be printed about the output.

To get the dump of a file containing of an ASCII or Unicode character just type in **`i`** as the input base. As ASCII or Unicode characters are all valid integers so their binary, hexadecimal and octal representation is the same as that of the integers.

## FILES

[`./dump.c`](https://github.com/r3sun/dump.c/blob/main/dump.c)

## DIAGNOSTICS

Complaints about bad key characters and input number. Complaints if unable to open file.

## BUGS

There's no way to read the characters from a file, in base other than integers.
