#define BIGNUMB_IMP
#include "bignumb.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
	Int a = {0};
	Int b = {0};
	new_int_from_dec(&a, "69000");
	new_int_from_dec(&b, "4200");
	print_int(&a);
	printf("+\n");
	print_int(&b);
	printf("=\n");
	Int res = {0};
	add_int(&res, &a, &b);
	print_int(&res);

	return 0;
}
