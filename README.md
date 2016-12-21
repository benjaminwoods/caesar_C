# caesar_C
A small C code that performs a variety of Caesar shifts on data.

***
COMPILING AND RUNNING:
***

	It's pretty straightforward to compile, as an example:
	
	gcc caesar.c -o caesar
	
	The binary takes 1-3 arguments:
	
	caesar SHIFT SIGN NUM_BYT
	- SHIFT is how much you would like to shift your input by
	- SIGN is which way (+1 increases all values by 1, -1 decreases)	[default of 1]
	- NUM_BYT is how many bytes form your character set			[default is letter input]
	
	The input file is "in.txt", and the output file is "out.txt", but the files can be binaries; they
	do not have to be ASCII.

***
WHAT THE CODE ACTUALLY DOES:
***

1) Binary shift

	The code takes the input and splits it into chunks of size NUM_BYT bytes:
	
	i.e. for NUM_BYT = 2:
		input		[01110011 00010001 11001101 10101010 11100110 01001100 11111010 01110111]
						|-    block 1    -|-    block 2    -|-    block 3    -|-    block 4    -|
	
	The code then takes each block and increments it by a value of SIGN*SHIFT. The block is wrapped using
	modulo so that the character set (defined by the length of a block) is looped.
	
	i.e. for SHIFT = 4:
													v                 v                 v                 v
		output	[01110011 00010101 11001101 10101110 11100110 01010000 11111010 01111011]
						|-    block 1    -|-    block 2    -|-    block 3    -|-    block 4    -|
	
	The output is then written to file. 

2) ASCII shift

	For the special case of NUM_BYT = 0, the code takes the input, capitalises it, and replaces all
	characters which are not capital letters with spaces.
	
	i.e. 
		input		[The quick brown dog jumped over the lazy dogs. What an odd sentence!]
		caps		[THE QUICK BROWN DOG JUMPED OVER THE LAZY DOGS  WHAT AN ODD SENTENCE ]
	
	Then, it performs the Caesar shift on each letter, moving each letter to the letter in the alphabet
	a value of SIGN*SHIFT away. The block is wrapped using a 26 character set (the alphabet).
	
	i.e.
		output	[YMJ VZNHP GWTBS ITL OZRUJI TAJW YMJ QFED ITLX  BMFY FS TII XJSYJSHJ ]
