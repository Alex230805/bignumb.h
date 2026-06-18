#define BIGNUMB_IMP
#include "bignumb.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
	Int a = {0};
	Int b = {0};
	Int res = {0};

	new_int_from_dec(&a, "100");
	new_int_from_dec(&b, "-1");
	add_int(&res, &a, &b);
	printf("%s + %s -> %s\n", render_int(&a), render_int(&b), render_int(&res));

	printf("\nTESTING RANDOM NUMBER GENERATION\n");
	Int random = {0};
	generate_int(&random,300);
	print_int(&random);

	printf("\nTESTING MULTIPLICATION\n\n");
	new_int_from_dec(&a, "64");
	new_int_from_dec(&b, "64");
	printf("%s * %s = \n", render_int(&a), render_int(&b));
	mux_int(&res, &a, &b);
	print_int(&res);

	printf("\nTESTING MORE EXTREME MULTIPLICATION\n\n");
	generate_int(&a, 300);
	generate_int(&b, 5);
	printf("%s * %s = \n", render_int(&a), render_int(&b));
	mux_int(&res, &a, &b);
	print_int(&res);

	printf("\nTESTING RENDER\n\n");
	char* r = render_int(&res);
	printf("%s\n", r);

	printf("\nTESTING POWER\n\n");
	new_int_from_dec(&a, "2");
	new_int_from_dec(&b, "64");
	printf("%s  ^ %s = \n", render_int(&a), render_int(&b));
	pow_int(&res, &a, &b);
	print_int(&res);

	printf("\nTESTING EXTREME POWER\n\n");
	generate_int(&a, 2);
	generate_int(&b, 3);
	printf("%s ^ %s =\n", render_int(&a), render_int(&b));
	pow_int(&res, &a, &b);
	printf("%s\n", render_int(&res));
	return 0;
}
