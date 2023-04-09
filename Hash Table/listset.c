#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// include the header files with the declarations of listset
#include "listset.h"

// create a new, empty linked list set
struct listset * listset_new() {

	// initalise a linked list
	struct listset * result;
	result = malloc(sizeof(struct listset));

	// ensure there is memory left
	assert(result != NULL);

	// set the head equal to NULL
	result-> head = NULL;

	// return the result
	return result;

}

/* check to see if an item is in the set
   returns 1 if in the set, 0 if not */
int listset_lookup(struct listset * this, char * item) {
	struct listnode *p;

	// go through each element and return as 1 if the element string is the same as the item
	for (p = this->head; p != NULL; p = p->next) {

		// compares the item with the string in the element
		if (strcmp(item, p->str) == 0) {
			return 1;
		}
	}

	// returns as false if there is no similar element
	return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set.
// New items that are not already in the set should
// be added to the start of the list
void listset_add(struct listset * this, char * item) {

	// if the item isn't in the list then continue
	if (listset_lookup(this, item) == 0) {

		// initialise the node in memory
		struct listnode * node;
		node = malloc(sizeof(struct listnode));

		// ensure there is enough memory
		assert(node != NULL);

		// set the string to item and the next to the head
		node->str = item;
		node->next = this-> head;

		// set head equal to the new node
		this->head = node;
	}
}

// remove an item with number 'item' from the set
void listset_remove(struct listset * this, char * item) {

	// if the item is in the list then continue, else do nothing
	if (listset_lookup(this, item) ==1) {

		// initialise two nodes as a prev and a current node
		struct listnode*prev;
		struct listnode*current;
		prev = NULL;
		current = this->head;

		// while the item has not been found
		while (strcmp(item, current->str) != 0) {

			// set the prev to the current node and the current to the next node
			prev = current;
			current = current->next;
		}

		// if the previous is still NULL then the head contains the item so we switch the head to the next
		if (prev == NULL) {
			this->head = current->next;
		} else {
			// else we make the previous nodes next equal to the current nodes next
			prev->next = current->next;
		}
	}
}

// place the union of src1 and src2 into dest
void listset_union(struct listset * dest, struct listset * src1,
		   struct listset * src2) {

	// set a pointer
	struct listnode*p;

	// place everything from the first source in the destination
	for (p =src1->head; p != NULL; p = p->next) {
		listset_add(dest, p->str);
	}

	// places everything from the second source in the destination
	// won't add anything that is already in there
	for (p = src2->head; p != NULL; p = p->next) {
		listset_add(dest, p->str);
	}

}

// place the intersection of src1 and src2 into dest
void listset_intersect(struct listset * dest, struct listset * src1,
		       struct listset * src2) {

	// set a pointer
	struct listnode*p;

	// for every item in src1 if the item is in src2, place it in the destination
	for (p = src1->head; p!= NULL; p = p->next) {

		// checks to see if the src1 item is in src2
		if (listset_lookup(src2, p->str) == 1) {

			// adds the item to the destination
			listset_add(dest, p->str);
		}
	}

}

// return the number of items in the listset
int listset_cardinality(struct listset * this) {

	// set a pointer
	struct listnode * p;

	// start a count at 0
	int count = 0;

	// while there is still another element in the list, add to the count
	for (p = this->head; p != NULL; p = p->next) {
		count++;
	}

	// return the count
	return count;
}

// print the elements of the list set
void listset_print(struct listset * this) {
  struct listnode * p;

  for ( p = this->head; p != NULL; p = p->next ) {
    printf("%s, ", p->str);
  }
  printf("\n");
}
