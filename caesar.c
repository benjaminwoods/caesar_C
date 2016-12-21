// Caesar binary shift
// Incremental shift on blocks of size [num_byt] bytes
//
// Created by bjqw500 (first stable version, 21.12.16; last edited 21.12.16)
//
// Maximum block size is currently 4 bytes
// Maximum file size of 2 Gb (ftell)

#include <stdio.h> 
#include <stdlib.h>

typedef unsigned long block;
char* file_contents;

#define IN_NAME "in.txt"
#define OUT_NAME "out.txt"

long input_file_size;

// Function list
void capitalise(char* input,long size); 										// Takes ASCII input and produces capitalised output
void caesar_block(char* input,long size,block shift,long num_byt,int sign); 	// Caesar routine for non-ASCII input
void caesar_ASCII(char* input,long size,block shift,int sign);					// Caesar routine for ASCII input
long file_toarray(char* input_file_name, long num_byt);							// Converts file to char array; returns file size in bytes
void array_tofile(char* output_file_name, long size);							// Converts char array to file

// Takes ASCII input and produces capitalised output
void capitalise(char* input,long size)
  {
	long n;
	
	for (n=0;n<size;n++)
	  {
		if ( ((input[n] >= 0) &&(input[n] < 65)) || ((input[n] > 90) && (input[n] < 97)) || ((input[n] > 122) && (input[n] < 128)) )
		  {
			input[n] = 32;
		  }
		else if ( (input[n] > 96) && (input[n] < 123) )
		  {
			input[n] = input[n] - 32;
		  }
	  }
  }

// Caesar routine for non-ASCII input
//
// shift = amount of shift
// num_byt = number of bytes
// sign = +1 for fwd shift
void caesar_block(char* input,long size,block shift,long num_byt,int sign)
  {
	long n;
	int p;
	block len = 1;
	block i;
	
	// Generate length
	len <<= 8*num_byt; //shift by 1 byte left for each byte (length is total number of values held by num_byt)
	
	for (n=0;n<size/num_byt;n++)
	  {
		i = 0;
		for (p=0;p<num_byt;p++)	
		  {
			i += input[(n*num_byt)+p]<<(8*(num_byt-p-1)); //concatenate num_byt bytes into one temporary block
		  }
		i = (i + len + sign*shift)%len; // Caesar shift
		for (p=0;p<num_byt;p++)	
		  {
			input[(n*num_byt)+p] = ( i>>(8*(num_byt-p-1) ) ) & (255); //mask off correct portion of block back into char
		  }
	  }
  }

// Caesar routine for ASCII input
//
// shift = amount of shift
// sign = +1 for fwd shift
void caesar_ASCII(char* input,long size,block shift,int sign)
{
	long n;
 
	capitalise(input,size);
	
	for (n=0;n<size;n++)
	  {
		if (input[n] != 32)	input[n] = 65 + ((input[n] - 65) + 26 + sign*shift)%26;
	  }
}

// Converts file to char array
long file_toarray(char* input_file_name, long num_byt)
  {
	long size,n;
	FILE *input_file = fopen(input_file_name, "rb");
	
	// Get file size
	fseek(input_file, 0, SEEK_END);
	size = ftell(input_file);
	rewind(input_file);
	
	// Dynamically allocate memory
	file_contents = (char*)malloc((size) * sizeof(char));
		
	// Read file
	fread(file_contents, sizeof(char), size, input_file);
	
	// Close file stream
	fclose(input_file);
	
	return size;
  }

// Converts char array to file
void array_tofile(char* output_file_name, long size)
  {
	FILE *output_file = fopen(output_file_name, "wb");
	
	// Write file
	fwrite(file_contents, sizeof(char), size, output_file);
	
	fclose(output_file);
  }

int main(int argc, char *argv[])
  {
	block shift; //Shift is a single value; varying shift is implemented in Vigenere
	int sign;
	long num_byt;
	long size;
	
	// arg initialisation
	if (argc == 1)
	  {
		return 0;
	  }
	else if (argc == 2)
	  {
		shift = atoi(argv[1]); // atoi is a bit lazy; for large shifts one would need to write a char->block routine
		sign = 1;
		num_byt = 0;
	  }
	else if (argc == 3)
	  {
		shift = atoi(argv[1]);
		sign = atoi(argv[2]);
		num_byt = 0;
	  }
	else
	  {
		shift = atoi(argv[1]);
		sign = atoi(argv[2]);
		num_byt = atoi(argv[3]);
	  }
	
	size = file_toarray(IN_NAME,num_byt);
	
	// ASCII flag
	if (num_byt == 0)
	  { 	
		caesar_ASCII(file_contents,size,shift,sign);
	  }
	else
	  {
		caesar_block(file_contents,size,shift,num_byt,sign);
	  }
	
	array_tofile(OUT_NAME,size);
	
	return 0;
  }