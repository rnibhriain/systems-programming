#include "infix.h"
#include <math.h>

// evaluate expression stored as an array of string tokens
double evaluate_infix_expression(char ** args, int nargs) {
  // Write your code here
	char ** postfix = malloc(sizeof(char*)*nargs);
	int postfix_length = 0;


	struct double_stack*stack = double_stack_new(nargs);


	for (int count = 0; count < nargs; count++) {
		if (strcmp(args[count], "(") == 0) {
			double_stack_push(stack, count);
		}
		else if (strcmp(args[count], "+") == 0 || strcmp(args[count], "-") == 0) {
			if (stack->top > 0) {
				int op = double_stack_pop(stack);
				double_stack_push(stack, op);
				while ((strcmp(args[op], "+") == 0 || strcmp(args[op], "-") == 0 || strcmp(args[op], "X") == 0 || strcmp(args[op], "/") == 0 || strcmp(args[op], "^") == 0) && stack->top > 0) {
					double_stack_pop(stack);
					postfix[postfix_length] = args[op];
					postfix_length++;
					op = double_stack_pop(stack);
					double_stack_push(stack, op);
				}
			}
			double_stack_push(stack, count);
		} else if (strcmp(args[count], "X") == 0 || strcmp(args[count], "/") == 0) {
			if (stack->top > 0) {
				int op = double_stack_pop(stack);
				double_stack_push(stack, op);
				while ((strcmp(args[op], "X") == 0 || strcmp(args[op], "/") == 0 || strcmp(args[op], "^") == 0) && stack->top > 0) {
					double_stack_pop(stack);
					postfix[postfix_length] = args[op];
					postfix_length++;
					op = double_stack_pop(stack);
					double_stack_push(stack, op);
				}
			}
			double_stack_push(stack, count);
		}
		else if (strcmp(args[count], "^") == 0) {
			if (stack->top >0) {
				int op = double_stack_pop(stack);
				double_stack_push(stack, op);
				while ((strcmp(args[op], "^") == 0) && stack->top > 0) {
					double_stack_pop(stack);
					postfix[postfix_length] = args[op];
					postfix_length++;
					op = double_stack_pop(stack);
					double_stack_push(stack, op);
				}
			}
			double_stack_push(stack, count);
		} else if (strcmp(args[count], ")") == 0) {
			int op = double_stack_pop(stack);
			while ((!(strcmp(args[op], "(") == 0)) && stack->top > 0) {
				postfix[postfix_length] = args[op];
				postfix_length++;
				op = double_stack_pop(stack);
			}
		} else {
			postfix[postfix_length] = args[count];
			postfix_length++;
		}
	}
	while (stack->top > 0) {
		int op = double_stack_pop(stack);
		postfix[postfix_length] = args[op];
		postfix_length++;
	}
	//for (int i = 0; i < postfix_length; i++) {
	//	printf(postfix[i]);
	//	printf(" ");
	//}
	double result = evaluate_postfix_expression(postfix, postfix_length);
	//printf("%lf", result);
	return result;
}
