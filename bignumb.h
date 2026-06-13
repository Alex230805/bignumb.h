#ifndef BIGNUMB_H
#define BIGNUMB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NUMBER_UNIT int8_t 
#define DEFAULT_NUMB_SIZE 8
#define DIGIT_PER_UNIT (sizeof(NUMBER_UNIT)*2) // maximum for one NUMBER_UNIT = 99

typedef struct{
	NUMBER_UNIT *number;
	size_t tracker;
	size_t size;
	bool sign; // if true = negative number, if false = positive number
}Int;

void new_int_from_dec(Int* d, char* string_number);
void new_int_from_hex(Int* d, char* string_number);
void add_int(Int* res, Int* a,Int* b);
void sub_int(Int* a,Int* b);
void mux_int(Int* a,Int* b);
void print_int(Int *a);

NUMBER_UNIT int_pop_last(Int *a);
void int_append_head(Int* a, NUMBER_UNIT n);

#ifdef BIGNUMB_IMP

void new_int_from_dec(Int* d, char* string_number){
	if(string_number[0] == '-'){
		d->sign = true;
		string_number++;
	}
	size_t depth = (size_t)(float)(strlen(string_number)/DIGIT_PER_UNIT);
	char* s = string_number;
	//printf("depth for %s = %d, %d\n", string_number, depth, depth%2);
	if(strlen(string_number)%2 != 0){
		depth += 1;
		char* new_number = (char*)malloc(sizeof(char)+strlen(string_number)+2);
		strcat(new_number, "0");
		strcat(new_number, string_number);
		s = new_number;
		//printf("New numb: %s\n", s);
	}
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
	if(a->sign) printf("-");
	for(size_t i=0;i<a->tracker; i++){
		printf("%.2u", a->number[i]);
	}
	printf("\n");
}


void add_int(Int* res, Int* a,Int* b){
	size_t cached_a_tracker = a->tracker;
	size_t cached_b_tracker = b->tracker;

	NUMBER_UNIT carry = 0;
	size_t d = 0;
	if(a->tracker > b->tracker){
		d = a->tracker;
	}else{
		d = b->tracker;
	}
	for(size_t i=0;i<d || carry != 0; i++){
		NUMBER_UNIT x = int_pop_last(a);
		NUMBER_UNIT y = int_pop_last(b);
		//printf("adding x=%.2d + y=%.2d\n", x, y);
		int dst = 0;
		dst = x+y+carry;
		//printf("result = %d, carry = %d\n", dst, carry);
		if(dst > 99){
			carry = (int)(dst/100);
			dst = dst%100;
		}else{
			carry = 0;
		}
		int_append_head(res, dst);
	}
	a->tracker = cached_a_tracker;
	b->tracker = cached_b_tracker;
}


void sub_int(Int* a,Int* b){

}

void mux_int(Int* a,Int* b){

}

NUMBER_UNIT int_pop_last(Int *a){
	if((int)a->tracker -1 < 0){
		return 0;
	}
	int p = a->number[a->tracker-1];
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

#endif // BIGNUMB_IMP
#endif // BIGNUMB_H
