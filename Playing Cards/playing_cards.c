//   playing_cards.c
//   David Gregg
//   December 2020

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "playing_cards.h"



// pack the playing card structures into bytes of memory
unsigned char * pack_playing_cards(struct playing_card * cards,
				   int number_of_cards) {

	// the current card being packed
	struct playing_card current;

	// allocate memory for the storage bytes
	unsigned char * the_cards =  malloc(sizeof(unsigned char)* number_of_cards);


	// control booleans
	int value_set = 0;
	int suit_set = 0;

	// count the bytes going up
	int byte_count = 0;

	// run through every card until they are all packed
	for (int i = 0; i < number_of_cards;) {

		// choose the current card
		current = cards[i];

		// set bits used to 0
		int bit_count = 0;

		// set the bits for a byte to be 0
		unsigned char the_char = (unsigned char) 0;

		// while all the bits have not been used
		while (bit_count < 8) {

			// if the suit isn't set then set the suit according to the bits that have been used
			if (suit_set == 0 && bit_count < 8) {

				// shift to get the correct value
				the_char = the_char | (((unsigned char) current.suit) << (8 - bit_count - 2));

				//change all the booleans accordingly and add to the bits used count
				value_set = 0;
				bit_count += 2;
				suit_set = 1;
			}

			// if none of the value has been set
			if (value_set == 0 && bit_count < 8) {

				// only half the value is in the byte
				if (bit_count == 6) {

					// assign the two most significant bits into the byte
					the_char = the_char | (((unsigned char) current.value >> 2));

					// tells us that the other half of the value needs to be added in
					value_set = 2;

					// add to the bits used
					bit_count += 2;
					suit_set = 1;
				}
				else {

					// assign the value to the byte
					the_char = the_char | (((unsigned char) current.value) << (8 - bit_count -4));

					// change the booleans and be sure to switch to the next card
					value_set = 1;
					suit_set = 0;
					i++;
					bit_count += 4;
					current = cards[i];
				}


			}

			// else if the value has been half set
			else if (value_set == 2 && bit_count == 0) {

				// set the two least significant bits to the next byte
				the_char = the_char | ((((unsigned char) current.value) << 6));

				// change all the booleans and add to the bit count

				value_set = 1;
				suit_set = 0;
				i++;

				// switch to the next card
				current = cards[i];
				bit_count += 2;
			}
		}

		// assign the bytes into the byte array and inrement
		the_cards[byte_count] = the_char;
		byte_count++;
	}

	// return the array
	return the_cards;
}




// unpack bytes of memory containing card data into playing card structures
struct playing_card * unpack_playing_cards(unsigned char * packed_cards,
					   int number_of_cards) {

	// allocate memory for the array of cards
	struct playing_card * cards = malloc(sizeof(struct playing_card) * number_of_cards);

	//set a count to keep track of the bytes
	int current_byte = 0;

	// set the boolean values
	int suit_set = 0;
	int value_set = 0;

	// run through until all the cards have been set
	for (int i = 0; i < number_of_cards;) {

		//  bits used in the byte set as 0
		int bit_count = 0;

		// cycle through the bits until the byte ends
		while (bit_count < 8) {

			// if the suit in the card needs to be set
			if (suit_set == 0) {

				// set the suit as the appropriate bits from the current byte
				cards[i].suit = (int) (3 & ((int) (packed_cards[current_byte] >> (8 - bit_count - 2))));

				// set the booleans and add to the bit count
				value_set = 0;
				bit_count += 2;
				suit_set = 1;
			}

			// if the value has not been set at all
			if (value_set == 0 && bit_count < 8) {


				// if there is only half the value in the
				if (bit_count == 6) {

					// set the two most significant bits of the value
					cards[i].value = (int ) (3 & (packed_cards[current_byte]));

					// set the value boolean as half set
					value_set = 2;

					// add to the count
					bit_count +=2;
					suit_set = 1;
				}


				else {

					// set the value
					cards[i].value = (int) (15 & ((int) (packed_cards[current_byte] >> (8 -  bit_count - 4))));

					// set the booleans and add to the bit count
					value_set = 1;
					suit_set = 0;
					bit_count += 4;

					// go the next playing card
					i++;
				}
			}

			// if the value is half set
			else if (value_set == 2 && bit_count < 8) {

				// shift the most significant bits to make room
				cards[i].value = (int) ((int)(cards[i].value << 2) & 12);

				// or the least significant bits into the value
				cards[i].value = (int) ((int) ((cards[i].value) | (packed_cards[current_byte] >> 6)));


				// add to the bit count and change the booleans again
				bit_count += 2;
				suit_set = 0;

				// go to the next playing card
				i++;
				value_set = 1;
			}

		}

		// increment to the next byte
		current_byte++;


	}

	// return the array of cards
	return cards;
}


