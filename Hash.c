#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sha256.h"


#define MAX_PASSWORD 32
#define HASH_SIZE ((PASS_SIZE * 2) + 1)
#define PASS_SIZE 32
#define DIG_BIN_LEN 32
#define DIG_STR_LEN ((DIG_BIN_LEN * 2) + 1)


void sha256(char *dest, char *src) {
    // zero out the sha256 context
    struct sha256_ctx ctx;
    memset(&ctx, 0, sizeof(ctx));

    // zero out the binary version of the hash digest
    unsigned char dig_bin[DIG_BIN_LEN];
    memset(dig_bin, 0, DIG_BIN_LEN);

    // zero out the string version of the hash digest
    memset(dest, 0, DIG_STR_LEN);

    // compute the binary hash digest
    __sha256_init_ctx(&ctx);
    __sha256_process_bytes(src, strlen(src), &ctx);
    __sha256_finish_ctx(&ctx, dig_bin);

    // convert it into a string of hexadecimal digits
    for (int i = 0; i < DIG_BIN_LEN; i++) {
        snprintf(dest, 3, "%02x", dig_bin[i]);
        dest += 2;
    }
}





struct hashDict {

  char password[PASS_SIZE];
  char hash[DIG_STR_LEN];	
  struct hashDict *next;
  
};

/*
	insertPair takes parameters struct head and struct pair, head is the beginning of
	the linked listand pair is the current struct that needs be inserted. First the program 
	checks if head is null, if so assigning it to pair. Next it checks if pair hash is lower
	than head hash, if so placing the pair behind the head and making the pair the new head 
	by returning pair. Next if checks where to place the hash if it is somewhere in the middle
	or end of the linked list. Since current will go to null when the struct needs to go
	to the end of the last, prev stores the value before that. Then it inserts and returns
	head
	

*/

struct hashDict* insertPair(struct hashDict *head, struct hashDict *pair){

	if(!head){
		pair->next = NULL;
		return pair;
		
	} else if (strcmp(pair->hash, head->hash) < 0){
		pair->next = head;
		return pair;
	}
	struct hashDict *current = head;
    struct hashDict *prev = current;

	
	while(current && strcmp(prev->hash, pair->hash) <0){
	    prev = current;
		current = current->next;
		
	}
	if(strcmp(prev->hash, pair->hash) ==0){
		free(pair);
		return head;
	}

	pair->next = prev->next;
	prev->next = pair;

	return head;


}

/*
	Next 3 functions work the same: clear text, l33t and plus one. All create a struct
	where the password is in its designated form and then the sturct is filled with
	the hash and password. It then gets returned 

*/

struct hashDict* dictWriter(char *curPass) {

  	char hashVer[64];
	struct hashDict *pair = malloc(sizeof(struct hashDict));


    sha256(hashVer, curPass);
    strncpy(pair->password, curPass, PASS_SIZE);
    strncpy(pair->hash, hashVer, HASH_SIZE);
    return pair;
  
}

struct hashDict* l33tWriter(char *curPass) {

  	char hashVer[64];
  	struct hashDict *pair = malloc(sizeof(struct hashDict));
	
    for (size_t i = 0; i < strlen(curPass); i++) {
      switch (curPass[i]) {
      case 'o':
        curPass[i] = '0';
        break;
      case 'e':
        curPass[i] = '3';
        break;
      case 'i':
        curPass[i] = '!';
        break;
      case 'a':
        curPass[i] = '@';
        break;
      case 'h':
        curPass[i] = '#';
        break;
      case 's':
        curPass[i] = '$';
        break;
      case 't':
        curPass[i] = '+';
        break;
      }
    }

    sha256(hashVer, curPass);
    strncpy(pair->password, curPass, PASS_SIZE);
    strncpy(pair->hash, hashVer, HASH_SIZE);
    return pair;

  
}

struct hashDict* plusOneWriter(char *curPass) {

  	char hashVer[64];

   	struct hashDict *pair = malloc(sizeof(struct hashDict));
    strcat(curPass, "1");
    sha256(hashVer, curPass);
    strncpy(pair->password, curPass, PASS_SIZE);
    strncpy(pair->hash, hashVer, HASH_SIZE);
	return pair;
  
}

/*
	Verbose runs when the the -v is entered on the command line. It iterates through
	the linked list each time it is called and prints the hash pass pair.

*/



void verbosef(struct hashDict *curPair, struct hashDict *head, bool verbose){

	struct hashDict *current = head;
	if(verbose){
		printf("inserting: %s\n", curPair->password);
		while(current){
			printf("%s %.5s...\n", current->password, current->hash);	
			current = current->next;
		}
		
			
	}
	printf("  \n");
	
}

/*
	Grab password takes each password from the input file, changing the newline charatcer to
	a string terminating one. It then sends that password to the 3 functions. 
	Once there, a new struct is returned which then gets send to the insert function. The
	head is updated by the insert function, so it always stays the beginning of the linked
	list. It is then returned to main

*/

struct hashDict* grabPassword(FILE *inputFile, struct hashDict *head, bool verbose){

	char curLine[100];
	char curLineL33t[100];
	char curLine1[100];

	while(fgets(curLine, sizeof(curLine), inputFile)){


		if(curLine[strlen(curLine) -1] == '\n'){
			curLine[strlen(curLine) -1] = '\0';
		}
		
		
		strcpy(curLine1, curLine);
		strcpy(curLineL33t, curLine);


		
		struct hashDict *curPair = dictWriter(curLine);
		head =insertPair(head, curPair);
		verbosef(curPair, head, verbose);
		
		struct hashDict *curPair2 = l33tWriter(curLineL33t);
		head = insertPair(head, curPair2);
		verbosef(curPair2, head, verbose);

		struct hashDict *curPair3 = plusOneWriter(curLine1);
		head = insertPair(head, curPair3);
		verbosef(curPair3, head, verbose);

	}

	return head;

		
}

/*
	Head is copied twice in write struct, once to see how many items are in the
	linked list, and another time to free all of the structs in the list. The writeStruct
	function also adds all of the structs to the output file, formatting them as such.

*/


void writeStruct(struct hashDict *head, FILE *outputFile){

	struct hashDict *temp = head;
	struct hashDict *freef = head;
	int count =0;

	while(temp){
		count++;
		temp = temp->next; 
	}

	fprintf(outputFile, "%d\n", count);

	while(head){
		fprintf(outputFile, "%s,%s\n", head->hash, head->password);
		head = head->next;
	}

	while(freef){
		struct hashDict *temp = freef;
		freef = freef->next;
		free(temp);
	}
	
}


/*
	Main checks for valid arguments, making sure there is 3 or 4. It also checks
	to make sure that the files can be opened succesfully. It called the main functions
	grabPassword and WriteStruct, then ends by closing the file.


*/

int main(int argc, char *argv[]){

	if (argc <2){
		printf("invlaid arguments");
		exit(-1);
	}

	struct hashDict *head = NULL;
	bool verbose;

	FILE *inputFile = fopen(argv[1], "r");
	FILE *outputFile = fopen(argv[2], "w");

	if(!inputFile || !outputFile){
		printf("files fails");
		exit(-1);
	}

	if(argc == 4){
		if(strcmp(argv[3], "-v") ==0){
			verbose = true;
		}
		
		
	} else{
		verbose = false;
	}

	head = grabPassword(inputFile, head, verbose);
	writeStruct(head, outputFile);

	

	fclose(outputFile);

}