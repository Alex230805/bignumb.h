#ifndef BIGNUMB_H
#define BIGNUMB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define NUMBER_UNIT			uint64_t
#define DEFAULT_NUMB_SIZE	2
#define MAX_VAL				((uint64_t)(0xFFFFFFFFFFFFFFFF)) // max value for 64bit    
#define BYTE_SIZE			sizeof(NUMBER_UNIT)
#define HEX_DIGITS			(BYTE_SIZE*2)

// Main integer representation in BASE 2 used to store and perform 
// calculation on big numbers. 
// Every operation and every number will be converted in BASE 2 first 
// before initializing any kind of processing. 
// If displayed or rendered the big number will be converted from BASE 2 
// to BASE 10, so from struct Int to struct Int_View. 

typedef struct{
	NUMBER_UNIT* ptr;
	NUMBER_UNIT *number;
	size_t tracker;
	size_t size;
	bool sign; // if true = negative number, if false = positive number
}Int;

// struct to store the BASE 10 representation of an Integer. Used 
// to display or render a stored integer. This representation is 
// not involved in calculation of any kind. Each number stored 
// by the user is in BASE 2, if diplayed or rendered it will 
// be first converted in BASE 10 with Int_View struct as store 
// destination.

typedef struct{
	uint8_t *number;
	size_t tracker;
	size_t size;
	bool sign;
}Int_View;

void int_view_to_int(Int* d, Int_View* s);

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
void generate_int(Int* dest, int bytes);

char* render_int(Int* a);

NUMBER_UNIT int_pop_last(Int *a);
void int_append_head(Int* a, NUMBER_UNIT n);
void int_view_append_head(Int_View* a, uint8_t n);
void int_append(Int* a, NUMBER_UNIT n);
void flip_int(Int* dest, Int* a);
void invert_array(NUMBER_UNIT* array, size_t size);
void clone_int(Int* dest, Int* a);
void free_int(Int* a);
void int_norm(Int* a, size_t len);

#ifdef BIGNUMB_IMP

void print_int(Int *a){
	if(a->tracker == 0){
		printf("empty number\n");
		return;
	}
	if(a->sign) printf("-");
	for(size_t i=0;i<a->tracker; i++){
		printf("%.*llx", (int)HEX_DIGITS, a->number[i]);
	}
	printf("\n");
}

void new_int_from_dec(Int* d, char* string_number){
	printf("NOT IMPLEMENTED\n");
	exit(1);
}

void new_int_from_hex(Int* d, char* string_number){
	d->tracker = 0;
	if(string_number[0] == '-') { d->sign = true; string_number++; }
	size_t string_size = strlen(string_number);
	size_t padding = 0;
	if(string_size%HEX_DIGITS){
		while((string_size+padding)%HEX_DIGITS) padding+=1;
	}
	char* buffer = (char*)malloc(sizeof(char)*(string_size+padding)+1);
	char* og_ptr = buffer;
	memset(buffer, '0', sizeof(char)*(string_size+padding));
	buffer+=padding;
	strcpy(buffer, string_number);
	buffer-=padding;
	d->size = (string_size+padding)/HEX_DIGITS;
	d->tracker = d->size;
	d->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*d->size);
	d->ptr = d->number;
	char c[HEX_DIGITS+1] = {0};
	for(size_t i=0;i<d->size; i++){
		uint32_t n = 0;
		memcpy(c, buffer, sizeof(char)*HEX_DIGITS);
		c[HEX_DIGITS] = '\0';
		sscanf(c, "%x", &n);
		buffer += HEX_DIGITS;
		d->number[i] = n;
	}
	free(og_ptr);
	og_ptr = NULL;
}

void add_int(Int* res, Int* a,Int* b){
	res->tracker = 0;
	res->sign = false;
	Int ca = {0};
	Int cb = {0};
	clone_int(&ca, a);
	clone_int(&cb, b);
	bool inv_required = false;

	if(ca.tracker > cb.tracker){
		int_norm(&cb, ca.tracker+1);
	}else if(ca.tracker < cb.tracker){
		int_norm(&ca, cb.tracker+1);
	}
	size_t d = ca.tracker;

	if(ca.sign){
		Int n = {0};
		flip_int(&n, &ca);
		free_int(&ca);
		memcpy(&ca, &n, sizeof(Int));
		//printf("Flipped: CA is:  %s\n", render_int(&ca));
		inv_required = true;
	}
	if(cb.sign){
		Int m = {0};
		flip_int(&m, &cb);
		free_int(&cb);
		memcpy(&cb, &m, sizeof(Int));
		//printf("Flipped: CB is: %s\n", render_int(&ca));
		inv_required = true;
	}
	NUMBER_UNIT carry = 0;
	for(size_t i=0;i<d || carry != 0; i++){
		NUMBER_UNIT x = int_pop_last(&ca);
		NUMBER_UNIT y = int_pop_last(&cb);
		__uint128_t dst = x+y+carry;
		if(dst > MAX_VAL){
			carry = (int)(dst/MAX_VAL);
			dst = dst%MAX_VAL;
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
		free_int(&j);
	}
	free_int(&ca);
	free_int(&cb);
}

void inc_int(Int* a){
	Int factor = {0};
	Int b = {0};
	int_append_head(&factor, 1);
	clone_int(&b, a);
	add_int(a, &b, &factor);
	free_int(&factor);
	free_int(&b);
}

void dec_int(Int* a){
	Int factor = {0};
	Int b = {0};
	int_append_head(&factor, 1);
	factor.sign = true;
	clone_int(&b, a);
	add_int(a, &b, &factor);
	free_int(&factor);
	free_int(&b);
}


void mux_int(Int* res, Int* a,Int* b){
	Int ca = {0};
	Int cb = {0};
	clone_int(&ca, a);
	clone_int(&cb, b);
	invert_array(ca.number, ca.tracker);
	invert_array(cb.number, cb.tracker);
	if(ca.tracker > cb.tracker){
		int_norm(&cb, ca.tracker);
	}else if(cb.tracker > ca.tracker){
		int_norm(&ca, cb.tracker);
	}

	size_t res_size = a->tracker+b->tracker;
	if(res->ptr){
		free(res->ptr);
		res->ptr = NULL;
		res->number = NULL;
		res->tracker = 0;
		res->size = 0;
	}
	res->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*res_size);
	res->ptr = res->number;
	res->size = res_size;
	res->tracker = res_size;

	for(size_t i=0;i<b->tracker;i++){
		NUMBER_UNIT carry = 0;
		for(size_t j=0;j<a->tracker; j++){
			__uint128_t p = res->number[i+j];
			p += carry + (a->number[j] * b->number[i]);
			carry = a->number[i+j] / MAX_VAL;
			res->number[i+j] = p % MAX_VAL;
			res->number[i+a->tracker] = carry;
		}
	}
	res->tracker -= 1;
	free_int(&ca);
	free_int(&cb);
	if(a->sign != b->sign){
		res->sign = true;
	}else{
		res->sign = false;
	}
}

void pow_int(Int* res, Int* a, Int* power){
	Int ca = {0};
	Int cpower = {0};
	Int i = {0};
	
	clone_int(&ca, a);
	clone_int(&cpower, power);
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
	while(int_lst(&i, power)){
		//printf("i = %s\n", render_int(&i));
		mux_int(res, a, &ca);
		clone_int(a, res);
		inc_int(&i);
	}
	clone_int(a, &ca);
	clone_int(power, &cpower);
	free_int(&ca);
	free_int(&cpower);
}

NUMBER_UNIT int_pop_last(Int *a){
	if((int)a->tracker-1 < 0){
		return 0;
	}
	NUMBER_UNIT p = a->number[a->tracker-1];
	a->tracker -= 1;
	return p;
}

void int_append(Int* a, NUMBER_UNIT n){
	if(a->number == NULL){
		a->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*DEFAULT_NUMB_SIZE);
		a->ptr = a->number;
		a->tracker = 0;
		a->size = DEFAULT_NUMB_SIZE;
	}
	if(a->tracker+1 >= a->size){
		a->size *= 2;
		a->number = (NUMBER_UNIT*)realloc(a->number, sizeof(NUMBER_UNIT)*a->size);
		a->ptr = a->number;
		if(!a->number){
			fprintf(stderr, "Reallocation failed for %p: %s\n", a, strerror(errno));
			exit(errno);
		}
	}
	a->number[a->tracker] = n;
	a->tracker += 1;
}

void int_append_head(Int* a, NUMBER_UNIT n){
	if(a->number == NULL){
		a->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*DEFAULT_NUMB_SIZE);
		a->ptr = a->number;
		if(!a->number){
			fprintf(stderr, "Allocation failed for %p: %s\n", a, strerror(errno));
			exit(errno);
		}
		a->tracker = 0;
		a->size = DEFAULT_NUMB_SIZE;
	}
	if(a->tracker+1 >= a->size){
		a->size *= 2;
		a->number = (NUMBER_UNIT*)realloc(a->number, BYTE_SIZE*a->size);
		a->ptr = a->number;
		if(!a->number){
			fprintf(stderr, "Reallocation failed for %p: %s\n", a, strerror(errno));
			exit(errno);
		}
	}
	for(size_t i=a->tracker; i>0; i-=1){
		 a->number[i] = a->number[i-1];
	}
	a->number[0] = n;
	a->tracker += 1;
}

void int_view_append_head(Int_View* a, uint8_t n){
	if(a->number == NULL){
		a->number = (uint8_t*)malloc(sizeof(uint8_t)*DEFAULT_NUMB_SIZE);
		a->tracker = 0;
		a->size = DEFAULT_NUMB_SIZE;
	}
	if(a->tracker+1 >= a->size){
		a->size *= 2;
		a->number = (uint8_t*)realloc(a->number, sizeof(uint8_t)*a->size);
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
	for(size_t i=0;i<ca.tracker; i++){
		ca.number[i] = MAX_VAL-ca.number[i];
	}
	Int b = {0};
	new_int_from_hex(&b, "1");
	ca.sign = false;
	add_int(dest, &ca, &b);
	dest->sign = false;
	free_int(&ca);
}

void invert_array(NUMBER_UNIT* array, size_t size){
	NUMBER_UNIT* c = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*size);
	memcpy(c, array, sizeof(NUMBER_UNIT)*size);
	size_t t = size-1;
	for(size_t i=0;i<size && t >= 0; i++){
		array[i] = c[t];
		t-=1;
	}
	free(c);
	c = NULL;
}

void free_int(Int* a){
	if(a->ptr){
		free(a->ptr);
		a->ptr = NULL;
		a->number = NULL;
		a->size = 0;
		a->tracker = 0;
	}
}

void clone_int(Int* dest, Int* a){
	if(dest->number){
		if(dest->size < a->size){
			NUMBER_UNIT* old = dest->number;
			dest->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*a->size);
			dest->ptr = dest->number;
			printf("size is bigger\n");
			if(!dest->number){
				fprintf(stderr, "Clone allocation failed for %p: %s\n", a, strerror(errno));
				exit(errno);
			}
			free(old);
			old = NULL;
		}
	}else{
		dest->number = (NUMBER_UNIT*)malloc(sizeof(NUMBER_UNIT)*a->size);
		dest->ptr = dest->number;
		if(!dest->number){
			fprintf(stderr, "New clone allocation failed for %p: %s\n", a, strerror(errno));
			exit(errno);
		}
	}
	dest->tracker = a->tracker;
	dest->size = a->size;
	dest->sign = a->sign;
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
	free_int(&ca);
	free_int(&cb);
	return res;
}

bool int_gtr(Int* a, Int* b){
	if(int_eq(a, b)) return false;
	bool res = false;
	Int ca = {0};
	Int cb = {0};
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
	free_int(&ca);
	free_int(&cb);
	return res;
}

bool int_lst(Int* a, Int* b){
	if(int_eq(a, b)) return false;
	return !int_gtr(a, b);
}

void int_norm(Int* a, size_t len){
	for(size_t i=0;i<len-a->tracker; i+=1){
		int_append_head(a, 0);
	}
}

void generate_int(Int* dest, int bytes){
	dest->tracker = 0;
	srand(time(NULL));
	for(size_t i=0;i<bytes; i++){
		NUMBER_UNIT data = rand()%MAX_VAL;
		int_append_head(dest, data);
	}
}

char* render_int(Int* a){
	size_t size = sizeof(char)*a->tracker*HEX_DIGITS+1;
	if(a->sign) size++;
	char* buffer = (char*)malloc(size);
	if(a->sign) strcpy(buffer, "-");
	char t[HEX_DIGITS+1] = {0};
	for(size_t i=0;i<a->tracker; i++){
		sprintf(t, "%.*llx", (int)HEX_DIGITS, a->number[i]);
		strcat(buffer, t);
	}
	return buffer;
}

#endif // BIGNUMB_IMP
#endif // BIGNUMB_H
