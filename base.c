#include "lab06.h"
#include <math.h>

int string_to_int(char *string, int base) {
    //TODO: implement this

    int total = 0;
    int dec;
    int place = 1;

	for(int i = strlen(string)-1; i >=0; i--){
		
		char c = string[i];
		if(c >= 'A' && c <= 'F'){
			dec = c - 'A' +10;
		}else if(c >= 'a' && c <= 'f'){
			dec = c - 'a' +10;
		} else if(c < 'a'){
			dec = c - 48;
		}
		total += dec * place;
		place *= base;
	
	}

    
    return total;
}
