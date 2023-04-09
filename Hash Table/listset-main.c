#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "listset.h"

// main function to test the listset ADT
int main(int argc, char ** argv) {
  struct listset * myset;
  struct listset * myset1;
  struct listset * myset2;

  if ( argc < 2 ) {
    fprintf(stderr, "Usage: ./listset <command> [<string>]\n");
    fprintf(stderr, "       where <command> is + (add following string)");
    fprintf(stderr, "                          - (remove following string)");
    fprintf(stderr, "                          = (print set)");
    exit(1);
  }

  // create the sets and follow the commands
  myset = listset_new();
  myset1 = listset_new();
  myset2 = listset_new();
  int i = 1;
  while ( i < argc ) {
	// adds an item to the first set
    if ( argv[i][0] == '+' ) {
      assert( i+1 < argc );
      listset_add(myset, argv[i+1]);
      i = i + 2;
    }
	// removes an item from the first set
    else if (argv[i][0] == '-' ) {
      assert( i+1 < argc );
      listset_remove(myset, argv[i+1]);
      i = i + 2;
    }
	// prints the first set
    else if (argv[i][0] == '=' ) {
      listset_print(myset);
      i++;
    }
	// adds an item to the second set (myset1)
    else if (argv[i][0] == ':') {
	assert(i+1 <argc);
	listset_add(myset1, argv[i+1]);
	i = i + 2;
    }
	// unions the first two sets into the other set and the prints it to check it
    else if (argv[i][0] == 'U') {
	listset_union(myset2, myset1, myset);
	listset_print(myset2);
	i++;
    }
	// finds the intersection of two sets and places it in the last set and prints it to check it
   else if (argv[i][0] == 'I') {
	listset_intersect(myset2, myset1, myset);
	listset_print(myset2);
	i++;
    }
	// finds the cardinality of the set and prints it
   else if (argv[i][0] == 'C') {
	printf("Cardinality of Set is: %d\n", listset_cardinality(myset));
	i++;
    }
	// prints the second set to check it's elements
   else if (argv[i][0] == 'P') {
	listset_print(myset1);
	i ++;
    }
	// tells you your entered command is incorrect
    else {
      fprintf(stderr, "Error: Unknown command %s\n", argv[i]);
      exit(1);
    }
//    assert( 0 ); // we should never get here
  }
    
  return 0;
}
