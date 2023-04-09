// simple C program that contains a hash table for strings
// David Gregg, November 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"


// compute a hash of a string using a seed value, where the result
// falls between zero and range-1
int hash_string(char * string, int range)
{
  int i;
  int hash = 0;
  const int HASH_SEED = 19;

  // simple loop for mixing the input string
  for ( i = 0; string[i] != '\0'; i++ ) {
    hash = hash * HASH_SEED + string[i];
  }
  // check for unlikely case that hash is negative
  if ( hash < 0 ) {
    hash = -hash;
  }
  // bring the hash within the range 0..range-1
  hash = hash % range;

  //printf("str: \'%s\', hash: %d\n", string, hash);
  return hash;
}

// create a new empty hashtable
struct hashtable * hashtable_new(int size)
{
	// initialise the memory
	struct hashtable * result;
	result = malloc(sizeof(struct hashtable));

	// check there is enough memory
	assert(result != NULL);

	// set the size and allocate memory for the array of listsets
	result->size = size;
	result->table = malloc(sizeof(struct listset)*size);

	// set an empty listset
	struct listset*p;
	p = listset_new();

	// initialise the array
	for (int i = 0; i < size; i++) {
		// set all the listsets to the empty listset
		result->table[i] = *p;
	}

	return result;
}

// add an item to th hashtable
void hashtable_add(struct hashtable * this, char * item)
{
	// get the hash of the item
	int hash = hash_string(item, this->size);

	// add the item to the listset it hashes to
	listset_add(&(this->table[hash]), item);

}

// return 1 if item is in hashtable, 0 otherwise
int hashtable_lookup(struct hashtable * this, char * item)
{
	// set a listnode pointer
	struct listnode*p;

	// get the hash of the item
	int hash = hash_string(item, this->size);

	// run through every item in the listset that the item hashes to
	for (p = this->table[hash].head; p != NULL; p = p->next) {

		// if the item is equal to a string in one of the nodes then 
		// return true
		if (strcmp(p->str, item) == 0) {
			return 1;
		}
	}
	// when all items in the listset have been checked then return false
	return 0;
}

// remove an item from the hash table; if the item is in the table
// multiple times, just remove the first one that we encounter
void hashtable_remove(struct hashtable * this, char * item)
{
	// initalise a pointer to a node
	struct listnode*p;

	// continue boolean
	int cont = 1;

	// for loop to run through ever listset until the item has been found
	for (int i = 0; i < this->size && cont == 1; i++) {

		// for loop to run through item in the listset until the item has been
		// found
		for (p = this->table[i].head; p != NULL && cont == 1; p = p->next) {

			// if the item in the set is the same as the parameter string
			// remove the item and set continue to false
			if (strcmp(p->str, item) == 0) {
				listset_remove(&(this->table[i]), item);
				cont = 0;
			}
		}
	}
}

// print the elements of the hashtable set
void hashtable_print(struct hashtable * this) {
  for (int i = 0; i < this->size; i++ ) {
    listset_print(this->table+i);
  }
}

