#ifndef BIGNUMB_H
#define BIGNUMB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_UNIT int8_t 
#define DEFAULT_NUMB_SIZE 8
#define DIGIT_PER_UNIT (sizeof(NUMBER_UNIT)) // maximum 9 
#define MAX_VAL	9

typedef struct{
	NUMBER_UNIT *number;
	size_t tracker;
	size_t size;
	bool sign; // if true = negative number, if false = positive number
}Int;

void print_int(Int *a);
void new_int_from_dec(Int* d, char* string_number);
void new_int_from_hex(Int* d, char* string_number);
void add_int(Int* res, Int* a,Int* b);
void sub_int(Int* res, Int* a,Int* b);
void mux_int(Int* res,Int* a,Int* b);
void pow_int(Int* res, Int* a, Int* power);
bool int_eq(Int* a, Int* b);
bool int_ueq(Int* a, Int* b);
bool int_gtr(Int* a, Int* b);
bool int_lst(Int* a, Int* b);
void inc_int(Int* a);
void dec_int(Int* a);
void generate_int(Int* dest, int digits);

char* render_int(Int* a);

NUMBER_UNIT int_pop_last(Int *a);
void int_append_head(Int* a, NUMBER_UNIT n);
void flip_int(Int* dest, Int* a);
void clone_int(Int* dest, Int* a);
void int_norm(Int* a, size_t len);

#ifdef BIGNUMB_IMP

void new_int_from_dec(Int* d, char* string_number){
	d->sign = false;
	d->tracker = 0;
	if(string_number[0] == '-'){
		d->sign = true;
		string_number++;
	}
	size_t depth = (size_t)(float)(strlen(string_number)/DIGIT_PER_UNIT);
	char* s = string_number;
	if(depth == 0) depth = 1;
	d->tracker = depth;
	d->size = depth;
	d->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*depth);
	char buffer[DIGIT_PER_UNIT+1] = {0};
	for(size_t i=0;i<depth; i++){
		memcpy(buffer, s, sizeof(char)*DIGIT_PER_UNIT);
		buffer[DIGIT_PER_UNIT] = '\0';
		d->number[i] = atoi(buffer);
		s+=DIGIT_PER_UNIT;
	}
}

void new_int_from_hex(Int* d, char* string_number){
	printf("NOT IMPLEMENTED\n");
	exit(1);
}

void print_int(Int *a){
	if(a->tracker == 0){
		printf("empty number\n");
		return;
	}
	if(a->sign) printf("-");
	for(size_t i=0;i<a->tracker; i++){
		printf("%.*u", (int)DIGIT_PER_UNIT, a->number[i]);
	}
	printf("\n");
}


void add_int(Int* res, Int* a,Int* b){
	res->tracker = 0;
	res->sign = false;
	Int ca = {0};
	Int cb = {0};
	clone_int(&ca, a);
	clone_int(&cb, b);
	bool inv_required = false;

	NUMBER_UNIT carry = 0;
	if(ca.tracker > cb.tracker){
		int_norm(&cb, ca.tracker+1);
	}else if(ca.tracker < cb.tracker){
		int_norm(&ca, cb.tracker+1);
	}
	size_t d = ca.tracker;

	if(ca.sign){
		Int n = {0};
		flip_int(&n, &ca);
		memcpy(&ca, &n, sizeof(Int));
		//printf("Flipped: CA is:  %s\n", render_int(&ca));
		inv_required = true;
	}
	if(cb.sign){
		Int m = {0};
		flip_int(&m, &cb);
		memcpy(&cb, &m, sizeof(Int));
		//printf("Flipped: CB is: %s\n", render_int(&ca));
		inv_required = true;
	}
	for(size_t i=0;i<d || carry != 0; i++){
		NUMBER_UNIT x = int_pop_last(&ca);
		NUMBER_UNIT y = int_pop_last(&cb);
		int dst = x+y+carry;
		if(dst > MAX_VAL){
			carry = (int)(dst/(MAX_VAL+1));
			//printf("Carry of '%d'\n", carry);
			dst = dst%(MAX_VAL+1);
			//printf("Dst of '%d'\n", dst);
			if(i+1 == d && inv_required && carry){
				// carry act as a flag, if is 1 then the final math with a 
				// ten's complement number is positive and the carry must be 
				// set to zero.
				// If the carry is 0 the math with a ten's complement number 
				// is negative. 
				// If both number are negative ( so inverted with ten's complement )
				// then the invertion is required by default no matter the carry outcome.
				carry = 0; 
				inv_required = false;
			}
		}else{
			carry = 0;
		}
		int_append_head(res, dst);
	}
	if(inv_required || (a->sign && b->sign)){
		Int j = {0}; 
		clone_int(&j, res);
		flip_int(res, &j);
	}
	for(size_t i=0;i<res->tracker; i++){
		if(res->number[i] > 0){
			res->tracker -= i;
			res->number += i;
			res->size -= i;
			break;
		}
	}
}

void inc_int(Int* a){
	Int factor = {0};
	Int b = {0};
	new_int_from_dec(&factor, "1");
	clone_int(&b, a);
	add_int(a, &b, &factor);
}

void dec_int(Int* a){
	Int factor = {0};
	Int b = {0};
	new_int_from_dec(&factor, "-1");
	clone_int(&b, a);
	add_int(a, &b, &factor);
}


void mux_int(Int* res, Int* a,Int* b){
	Int i = {0};
	Int m = {0};
	Int ca = {0};
	Int cb = {0};

	res->tracker = 0;
	clone_int(&ca, a);
	clone_int(&cb, b);
	new_int_from_dec(&i, "0");
	a->sign = false;
	b->sign = false;
	if(int_ueq(a, &i) || int_ueq(b, &i)){
		int_append_head(res, 0);
		return;
	}
	inc_int(&i);
	clone_int(&m, a);
	while(int_lst(&i, b)){
		add_int(res, a, &m);
		clone_int(a, res);
		inc_int(&i);
	}
	clone_int(res, a);
	clone_int(a, &ca);
	clone_int(b, &cb);
	if(a->sign == b->sign) {
		res->sign = false;
	}else{
		res->sign = true;
	}
}

void pow_int(Int* res, Int* a, Int* power){
	Int ca = {0};
	Int cpower = {0};
	Int i = {0};

	int_append_head(&i, 0);
	if(int_ueq(power, &i) || int_ueq(power, &i)){
		int_append_head(res, 1);
		return;
	}
	inc_int(&i);
	if(int_ueq(power, &i)){
		clone_int(res, a);
		return;
	}
	clone_int(&ca, a);
	clone_int(&cpower, power);
	while(int_lst(&i, power)){
		mux_int(res, a, &ca);
		clone_int(a, res);
		inc_int(&i);
	}

	clone_int(a, &ca);
	clone_int(power, &cpower);
}

NUMBER_UNIT int_pop_last(Int *a){
	if((int)a->tracker-1 < 0){
		return 0;
	}
	NUMBER_UNIT p = a->number[a->tracker-1];
	a->tracker -= 1;
	return p;
}

void int_append_head(Int* a, NUMBER_UNIT n){
	if(a->number == NULL){
		a->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*DEFAULT_NUMB_SIZE);
		a->tracker = 0;
		a->size = DEFAULT_NUMB_SIZE;
	}
	if(a->tracker+1 >= a->size){
		NUMBER_UNIT* old = a->number;
		a->size *= 2;
		a->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*a->size);
		for(size_t i=0;i<a->tracker;i++){
			a->number[i] = old[i];
		}
	}
	for(size_t i=a->tracker; i>0; i-=1){
		 a->number[i] = a->number[i-1];
	}
	a->number[0] = n;
	a->tracker += 1;
}

void flip_int(Int* dest, Int* a){
	Int ca = {0};
	clone_int(&ca, a);
	dest->tracker = 0;
	for(size_t i=0;i<a->tracker; i++){
		a->number[i] = MAX_VAL-a->number[i];
	}
	Int b = {0};
	new_int_from_dec(&b, "1");
	a->sign = false;
	add_int(dest, a, &b);
	dest->sign = false;
	clone_int(a, &ca);
	a->sign = !a->sign;
}

void clone_int(Int* dest, Int* a){
	dest->tracker = a->tracker;
	dest->size = a->size;
	dest->sign = a->sign;
	dest->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*dest->size);
	for(size_t i=0;i<dest->tracker; i++){
		dest->number[i] = a->number[i];
	}
}


bool int_eq(Int* a, Int* b){
	if(a->sign != b->sign) return false;
	return int_ueq(a, b);
}

bool int_ueq(Int* a, Int* b){
	Int ca = {0};
	Int cb = {0};
	clone_int(&ca, a);
	clone_int(&cb, b);
	if(ca.tracker > cb.tracker){
		int_norm(&cb, ca.tracker);
	}else if(ca.tracker < cb.tracker){
		int_norm(&ca, cb.tracker);
	}
	bool res = true;
	for(size_t i=0;i<ca.tracker;i+=1){
		if(ca.number[i] != cb.number[i]){
			res = false;
			break;
		}
	}
	return res;
}

bool int_gtr(Int* a, Int* b){
	if(int_eq(a, b)) return false;
	Int ca = {0};
	Int cb = {0};
	bool res = false;
	if(!a->sign && b->sign){
		return true;
	}else if(a->sign && !b->sign){
		return false;
	}else{
		clone_int(&ca, a);
		clone_int(&cb, b);
		size_t len = 0;
		if(ca.tracker > cb.tracker){
			len = ca.tracker;
			int_norm(&cb, len);
		}else{
			len = cb.tracker;
			int_norm(&ca, len);
		}
		for(size_t i=0;i<len;i+=1){
			if(ca.number[i] > cb.number[i]){
				res = true;
				break;
			}
			if(ca.number[i] < cb.number[i]){
				res = false;
				break;
			}
		}
	}
	return res;
}

bool int_lst(Int* a, Int* b){
	if(int_eq(a, b)) return false;
	return !int_gtr(a, b);
}

void int_norm(Int* a, size_t len){
	for(size_t i=0;i<len-a->tracker+1; i+=1){
		int_append_head(a, 0);
	}	
}

void generate_int(Int* dest, int digits){
	dest->tracker = 0;
	srand(time(NULL));
	for(size_t i=0;i<digits/DIGIT_PER_UNIT; i++){
		int data = rand()%MAX_VAL;
		int_append_head(dest, data);
	}
}

char* render_int(Int* a){
	size_t size = sizeof(char)*a->tracker*2;
	if(a->sign) size++;
	char* buffer = (char*)malloc(size);
	if(a->sign) strcat(buffer, "-");
	char t[3] = {0};
	for(size_t i=0;i<a->tracker; i++){
		sprintf(t, "%.*u", (int)DIGIT_PER_UNIT, a->number[i]);
		strcat(buffer, t);
	}
	return buffer;
}

#endif // BIGNUMB_IMP
#endif // BIGNUMB_H
