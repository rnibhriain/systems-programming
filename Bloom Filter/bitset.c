#include "bitset.h"
#include <stdlib.h>
#include <stdio.h>

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
	struct bitset * data;

	data = malloc(sizeof(struct bitset));

	int word_size = sizeof(uint64_t)*8;

	// calculate number of words based on input size
	int words = (size / word_size) + (size % word_size > 0);

	data->size_in_words = words;

	data->universe_size = size;

	// allocates memory for the actual bits
	data->bits = malloc(sizeof(uint64_t)*words);

	// sets all values to 0
	for(int i = 0; i < words; i++) {
		data->bits[i] = 0;
	}

	return data;

}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
	return this->universe_size;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this){
	int result = 0;

	// go through each word in the bitset
	for(int i = 0; i < this->size_in_words; i++) {
		uint64_t current_word = this->bits[i];

		// for each 64 bit word check each bit
		while (current_word != 0) {

			// clears every bit except the first
			uint64_t bit_to_check = current_word & 1;
			if (bit_to_check != 0) {
				result++;
			}
			current_word = current_word >> 1;
		}
	}

	return result;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item){
	int word_location = item / (sizeof(uint64_t)*8);
	int bit_location = item % (sizeof(uint64_t)*8);
	if (word_location < 0 || word_location > this->size_in_words) {
		// returns invalid if the location is not within range
		return -1;
	}
	uint64_t current_word = this->bits[word_location];
	current_word = current_word >> (uint64_t) bit_location;

	// get the bit needed to check
	current_word = current_word & 1;
	if (current_word == 1) {
		// found value
		return 1;
	} else {
		// value not recorded in bitset
		return 0;
	}
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
void bitset_add(struct bitset * this, int item) {
	int word_location = item / (sizeof(uint64_t)*8);
	int bit_location = item % (sizeof(uint64_t)*8);

	if (word_location < 0 || word_location > this->size_in_words) {
		// checks if the index can be inserted
		printf("not a valid location");
	} else {
		// uses logical OR to set the bit recording the value
		uint64_t current_word = this->bits[word_location];
		current_word = current_word | ((uint64_t)1 << bit_location);
		// storing the changed word
		this->bits[word_location] = current_word;
	}
}

// remove an item with number 'item' from the set
void bitset_remove(struct bitset * this, int item) {
	int word_location = item / (sizeof(uint64_t)*8);
	int bit_location = item % (sizeof(uint64_t)*8);

	if (word_location < 0 || word_location >= this->size_in_words) {
		// checks if the index is valid
		printf("not a valid location");
	} else {
		uint64_t current_word = this->bits[word_location];
		// Uses logical OR to make sure the bit is set
		current_word = current_word | ((uint64_t)1 << bit_location);
		// Uses logical XOR to flip the bit
		current_word = current_word ^ ((uint64_t)1 << bit_location);
		// store the changed value
		this->bits[word_location] = current_word;
	}
}

// place the union of src1 and src2 into dest;
// all of src1, src2, and dest must have the same size universe
void bitset_union(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
	// runs through each word and uses logical OR to get the union
	for (int i = 0; i < dest->size_in_words; i++ ) {
		dest->bits[i] = src1->bits[i] | src2->bits[i];
	}
}

// place the intersection of src1 and src2 into dest
// all of src1, src2, and dest must have the same size universe
void bitset_intersect(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
	// runs through each word and uses logical AND to get the intersection
	for (int i = 0; i < dest->size_in_words; i++) {
		dest->bits[i] = src1->bits[i] & src2->bits[i];
	}

}
