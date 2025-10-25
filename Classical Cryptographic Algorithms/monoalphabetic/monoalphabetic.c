#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// dynamic array of chars
typedef struct {
	char *data; // points to the array
	size_t size; // number of elements used 
	size_t capacity; // total allocated space
} CharVec;

// allocate the array
void CharVec_init(CharVec *v, size_t initial_capacity){
	v->data = (char*) malloc(initial_capacity * sizeof(char));
	v->size = 0;
	v->capacity = initial_capacity;
}

// free the array memory
void CharVec_free(CharVec *v){
	free(v->data);
	v->data = NULL;
	v->size = 0;
	v-> capacity = 0;
}

// growing the array
void CharVec_push(CharVec *v, char elem){
	if(v->size >= v-> capacity){
		size_t new_capacity = v->capacity * 2 + 1;
		v->data = (char*) realloc(v->data, new_capacity * sizeof(char));
		v-> capacity = new_capacity;
	}
	v->data[v->size++] = elem;
}

CharVec Plain, Cipher;
