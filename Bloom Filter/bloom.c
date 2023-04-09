#include <stdlib.h>
#include "bloom.h"


const int BLOOM_HASH1 = 17;
const int BLOOM_HASH2 = 29;

// compute a hash of a string using a seed value, where the result
// falls between zero and range-1
int hash_string(char * string, int seed, int range)
{
  int i;
  int hash = 0;

  // simple loop for mixing the input string
  for ( i = 0; string[i] != '\0'; i++ ) {
    hash = hash * seed + string[i];
  }
  // check for unlikely case that hash is negative
  if ( hash < 0 ) {
    hash = -hash;
  }
  // bring the hash within the range 0..range-1
  hash = hash % range;

  return hash;
}

// create a new, empty Bloom filter of 'size' items
struct bloom * bloom_new(int size) {
	struct bloom * result;
	result = malloc(sizeof(struct bloom));

	result->size = size;
	// creates a bit vector set to store the bloom filter
	struct bitset * bitset = bitset_new(size);
	result->bitset = bitset;

	return result;
}

// check to see if a string is in the set
int bloom_lookup(struct bloom * this, char * item) {
	int hash1, hash2;
	hash1 = hash_string(item, BLOOM_HASH1, this->size);

	// check for first hash
	int result = bitset_lookup(this->bitset, hash1);

	// if the first item's hash is found then check the second hash
	if (result == 1) {
		hash2 = hash_string(item, BLOOM_HASH2, this->size);
		result = bitset_lookup(this->bitset, hash2);

		// ... and the second hash is in the bitset
		if (result == 1) {
			return 1;
		} else if (result == -1) {
			// outside of range
			return -1;
		} else {
			// the second hash is not in bitset
			return 0;
		}
	} else if (result == -1) {
		// outside of range
		return -1;
	} else {
		// hash is not in bitset
		return 0;
	}
}

// add a string to the set
// has no effect if the item is already in the set
void bloom_add(struct bloom * this, char * item) {
	int hash1, hash2;
	// gets two hashes for the string and inputs both in the bitset
	hash1 = hash_string(item, BLOOM_HASH1, this->size);
	bitset_add(this->bitset, hash1);
	hash2 = hash_string(item, BLOOM_HASH2, this->size);
	bitset_add(this->bitset, hash2);
}

// place the union of src1 and src2 into dest
void bloom_union(struct bloom * dest, struct bloom * src1,
    struct bloom * src2) {
	// union the bitsets from src1 and src2 logical OR
	bitset_union(dest->bitset, src1->bitset, src2->bitset);
}

// place the intersection of src1 and src2 into dest
void bloom_intersect(struct bloom * dest, struct bloom * src1,
    struct bloom * src2) {
	// get the intersect of src1 and src2 logical AND
	bitset_intersect(dest->bitset, src1->bitset, src2->bitset);
}
