 #include "stack.h"

struct double_stack * double_stack_new(int max_size) {
	 struct double_stack*result;
 
 // allocate space for the header
	 result = malloc(sizeof(struct double_stack));
	 result->max_size = max_size;
	 result->top = 0;
 
 // allocate space for the data store d in the stack
 	result->items = malloc(sizeof(double)*max_size);

// iniantalise all items in the stack to 0

	for (int count = 0; count < max_size; count++) {
		result->items[count] = 0;
	}

 // return a pointer to the newly-allocated stack
 return result;

}

// push a value onto the stack
void double_stack_push(struct double_stack * this, double value) {
	 if (this->top < this->max_size) {
  		this->items[this->top] = value;
		this->top++;
		// printf("\n you have successfully input a value \n");
   
 	} else {
 		  printf("\n There are too many things in the stack right now. (%d; %d), \n", this->top, this->max_size);
 	}
}

// pop a value from the stack
double double_stack_pop(struct double_stack * this) {
	double value;
	if (this->top >= 0) {
		this->items[this->top] = 0;
		this->top--;
		value = this->items[this->top];
		//printf("\n you have successfully popped a value \n");
	} else {
		printf("\n there is nothing left to pop \n");
	}
	//printf("\n %lf", value);
	return value;

}

// print the values in the stack
void double_stack_print (struct double_stack * this) {
	for (int count = 0; count < this->max_size; count++) {
		printf("%lf", this->items[count]);

	}
}
