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
	a.tracker = 0;
	a.sign = false;
	b.tracker = 0;
	b.sign = false;
	res.tracker = 0;
	res.sign = false;
	int_append_head(&a, 1);
	int_append_head(&b, 20);
	print_int(&a);
	printf("*\n");
	print_int(&b);
	printf("=\n");
	mux_int(&res, &a, &b);
	print_int(&res);

	printf("Testing more extreme multiplication\n");
	a.tracker = 0;
	generate_int(&a, 300);
	b.tracker = 0;
	generate_int(&a, 500);
	res.tracker = 0;
	mux_int(&res, &a, &b);
	print_int(&res);
	return 0;
}
