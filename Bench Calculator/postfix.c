#include "postfix.h"
#include <math.h>
#include <stdlib.h>

// evaluate expression stored as an array of string tokens
double evaluate_postfix_expression(char ** args, int nargs) {
  // Write your code here
	double result, number1, number2;
	struct double_stack*stack = double_stack_new(nargs);

	for (int count = 0; count < nargs; count++) {
		if (strcmp(args[count], "+") == 0) {
			number2 = double_stack_pop(stack);
			number1 = double_stack_pop(stack);
			result = number1 + number2;
			double_stack_push(stack, result);
		} else if (strcmp(args[count], "-") == 0) {
			number2 = double_stack_pop(stack);
			number1 = double_stack_pop(stack);
			result = number1 - number2;
			double_stack_push(stack, result);
		} else if (strcmp(args[count], "X") == 0) {
			number2 = double_stack_pop(stack);
			number1 = double_stack_pop(stack);
			result = number1 * number2;
			double_stack_push(stack, result);

		} else if (strcmp(args[count], "/") == 0) {
			number2 = double_stack_pop(stack);
			number1 = double_stack_pop(stack);
			result = number1 / number2;
			double_stack_push(stack, result);

		} else if (strcmp(args[count], "^") == 0) {
			number2 = double_stack_pop(stack);
			number1 = double_stack_pop(stack);
			result = pow(number1, number2);
			double_stack_push(stack, result);

		} else {
			double_stack_push(stack, strtod(args[count], NULL));
		}
	}
	return result;
}
