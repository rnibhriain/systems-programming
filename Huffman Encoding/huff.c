// code for a huffman coder

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "huff.h"

// define all extra functions used

struct huffchar * new_compound (struct huffchar * left, struct huffchar * right, int seqno);
int find_least_freq (struct huffchar ** list, int size);
void write_huffman_codes (struct huffchar * root, struct huffcoder * this);
void write_codes_recursive (struct huffchar * current, char * path, int depth, struct huffcoder * this);
char * CloneString (char * theString, int length);
char find_char(struct huffcoder * this, char * code, int location);
char find_char_recursive(struct huffchar * this, char * code, int location);

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{

	// allocate memory
	struct huffcoder * result;
	result = malloc(sizeof(struct huffcoder));

	// initialise all the variables in the huffcoder to 0
	for (int i = 0; i < NUM_CHARS; i++) {
		result->freqs[i] = 0;
		result->code_lengths[i] = 0;
		result->codes[i] = NULL;
	}

	// set the pointer to NULL
	result->tree = (struct huffchar*)NULL;

	// return the struct
	return result;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{

	unsigned char c;

	// set a pointer to read the file
	FILE * fp;
	fp = fopen(filename, "r");

	//  run through each character in the file until the end, adding to the frequency of said character
	c = fgetc(fp);
	while (!feof(fp)) {
		this->freqs[c]++;
		c = fgetc(fp);
	}

	// run through all the frequencies and if the frequency is 0 then set it to 1 to avoid errors
	for (int i = 0; i < NUM_CHARS; i++) {
		if (this->freqs[i] == 0) {
			this->freqs[i] = 1;
		}
	}

	// close the file
	fclose(fp);
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{

	// allocate memory for each character
	struct huffchar ** list;
	list = malloc (sizeof(struct huffchar*)*NUM_CHARS);

	// set up list of simple chars
	for (int i = 0; i < NUM_CHARS; i++) {
		list[i] = malloc(sizeof(struct huffchar));
		list[i]->freq = this->freqs[i];
		list[i]->is_compound = 0;
		list[i]->seqno = i; // tie breaking
		list[i]->u.c = i;
	}

	int list_size = NUM_CHARS;
	int seqno = NUM_CHARS;

	while (list_size > 1) {

		// find the least frequent
		int smallest = find_least_freq(list, list_size);
		struct huffchar * min1 = list[smallest];

		// remove that leaf node from the list
		list[smallest] = list[list_size-1];

		// find the next least frequent
		smallest = find_least_freq(list, list_size-1);
		struct huffchar * min2 = list[smallest];

		// remove that leaf node from the list
		list[smallest] = list[list_size-2];

		// create compound character
		struct huffchar * compound;
		compound = new_compound(min1, min2, seqno);

		// decrement the list size
		list_size--;

		// add to the current sequence number
		seqno++;

		// add the compound node to the list
		list[list_size-1] = compound;
	}

	// set the huffcoder pointer to the root of the tree
	this->tree = list[0];
}

// create a new compound character with a left and right node and a sequence number
struct huffchar * new_compound (struct huffchar * left, struct huffchar * right, int seqno) {

	// allocate memory for the compound node
	struct huffchar * compound;
	compound = malloc(sizeof(struct huffchar));

	// add the two frequencies together for the new frequency
	compound->freq = left->freq + right->freq;

	// set it as a compound number
	compound->is_compound = 1;

	// allocate the new sequence number and the two nodes
	compound->seqno = seqno;
	compound->u.compound.left = left;
	compound->u.compound.right = right;

	// return the compound node
	return compound;
}

// finds the node with the smallest frequency
int find_least_freq (struct huffchar ** list, int size) {

	// start the min and the seqno at the beginning
	int min = list[0]->freq;
	int seqno = list[0]->seqno;

	// initialise the minimum index
	int min_idx = 0;

	// run through all the nodes starting at 1
	for (int i = 1; i < size; i++) {

		// if the frequency is less than the current frequency
		// or the frequency is the same and the seqno is lower than the current seqno
		if (list[i]->freq < min || (list[i]->freq == min && list[i]->seqno < seqno)) {

			//new min
			min = list[i]->freq;
			seqno = list[i]->seqno;
			min_idx = i;
		}
	}

	// return the index of the lowest frequency
	return min_idx;
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{
	write_huffman_codes(this->tree, this);
}

// will write out all the codes in the huffcoder structure
void write_huffman_codes (struct huffchar * root, struct huffcoder * this) {

	// allocate memory for an array to build the path
	char * path = malloc(sizeof(char)*NUM_CHARS);

	// set all chars in the array to '-' originally
	for (int i = 0; i < NUM_CHARS;  i++) {
		path[i] = '-';
	}

	// call the recursive function starting at the root
	write_codes_recursive(root, path, 0, this);
}

void write_codes_recursive(struct huffchar * current, char * path, int depth, struct huffcoder * this) {

	// if the node is compound then continuing recursively writing the path
	if (current->is_compound == 1) {

		// do left path
		path[depth] = '0';
		write_codes_recursive(current->u.compound.left, path, depth+1, this);

		// do right path
		path[depth] = '1';
		write_codes_recursive(current->u.compound.right, path, depth+1, this);

	} else {

		unsigned char c = current->u.c;

		// set the code length and copy the path into the code for the current character
		this->code_lengths[c] = depth;
		this->codes[c] = CloneString(path, depth);
	}
}


// copies a string into another memory location and returns a pointer to the new copy
char * CloneString (char * theString, int length) {

	// allocate memory for the string
	char * copy =  malloc(sizeof(char)*(length+1));

	// copy each character into the copy until the counter hits the length
	for (int i = 0; i < length; i++) {
		copy[i] = theString[i];
	}

	// end of the copy
	copy[length] = '\0';

	// return a pointer to the copied string
	return copy;
}

// print the Huffman codes for each character in order;
// you should not modify this function
void huffcoder_print_codes(struct huffcoder * this)
{
  for ( int i = 0; i < NUM_CHARS; i++ ) {
    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], this->codes[i]);;
  }
}

// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
	unsigned char c;
	FILE * fp, * fp2;

	// open the input as a read file and the output as a write file
	fp = fopen(input_filename, "r");
	fp2 = fopen(output_filename, "w");

	// get the first character and set a count to 0
	c = fgetc(fp);
	int i = 0;

	// while there are still characters left
	while (!feof(fp)) {

		// places each '0' or '1' from the code for that char into the output
		for (int j = 0; j < this->code_lengths[c]; j++, i++) {

			// places the current '0' or '1' in the outpur
			fputc(this->codes[c][j], fp2);
		}

		// get next char
		c = fgetc(fp);
	}

	// place the code for EOT in the output
	for (int j = 0; j < this->code_lengths['\x04']; j++, i++) {

		fputc(this->codes['\x04'][j], fp2);
	}

	// close the files
	fclose(fp);
	fclose(fp2);
}

// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
	FILE * fp, * fp2;
	unsigned char c;

	// open the input as a read file and the output as a write file
	fp = fopen(input_filename, "r");
	fp2 = fopen(output_filename, "w");

	// get the size of the input
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);

	// initialise an array of the size of the file
	char theCodes [size];

	// set the pointer at the start of the file
	rewind(fp);

	// place all the chars from the file in the char array
	fread(theCodes, size, 1, fp);

	// set the location counter to 0 and find the first character
	int location = 0;
	c = find_char(this, theCodes, location);

	// while the end of the file has not been reached
	while (c != '\x04') {

		// place the found character in the output
		fputc(c, fp2);

		// increment the location using the code length for that character
		location = location + this->code_lengths[c];

		// get the next character
		c = find_char(this, theCodes, location);
	}

	// close the files
	fclose(fp);
	fclose(fp2);
}

// finds the next char in the input file
char find_char (struct huffcoder * this, char * code, int location) {

		// call the recursive function
	return find_char_recursive(this->tree, code, location);
}

// finds the next char in the input file from the current node
char find_char_recursive ( struct huffchar * current, char * code, int location) {

	// if the node is not a compound node then return the character
	if (current->is_compound != 1) {
		return current->u.c;
	}

	// if the code tells you to go left then follow the tree left
	 else if (code[location] == '0') {
		return find_char_recursive(current->u.compound.left, code, location+1);
	}

	// else follow the tree right
	else {
		return find_char_recursive(current->u.compound.right, code, location+1);
	}
}
