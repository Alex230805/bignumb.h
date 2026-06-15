#define BIGNUMB_IMP
#include "bignumb.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
	Int a = {0};
	Int b = {0};
	new_int_from_dec(&a, "-11");
	new_int_from_dec(&b, "-11");

	printf("Testing logic comparision\n");
	if(int_gtr(&a, &b)){
		printf("A is greater than B\n");
	}else{
		printf("B is greater than A\n");
	}
	printf("Testing addition unit\n");
	print_int(&a);
	printf("+\n");
	print_int(&b);
	printf("=\n");
	Int res = {0};
	add_int(&res, &a, &b);
	print_int(&res);

	printf("Testing random number generation\n");
	Int random = {0};
	generate_int(&random,300);
	print_int(&random);

	printf("Testing multiplication\n");
	new_int_from_dec(&a, "2");
	new_int_from_dec(&b, "2");
	print_int(&a);
	printf("*\n");
	print_int(&b);
	printf("=\n");
	mux_int(&res, &a, &b);
	print_int(&res);

	printf("Testing more extreme multiplication\n");
	generate_int(&a, 300);
	generate_int(&a, 500);
	mux_int(&res, &a, &b);
	print_int(&res);

	printf("Testing render\n");
	char* r = render_int(&res);
	printf("%s\n", r);

	printf("Testing subtraction\n");

	new_int_from_dec(&a, "1");
	new_int_from_dec(&b, "-2");
	print_int(&a);
	printf("-\n");
	print_int(&b);
	printf("=\n");
	sub_int(&res, &a, &b);
	print_int(&res);

	return 0;
}
