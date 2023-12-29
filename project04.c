#include "lab06.h"


int main(int argc, char **argv){

	int base = 0;

	//check for a base listed in command line arguments, by grabbing the 
	//arguments listed after the -b if there is one 

	for(int i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-b")){
			base = atoi(argv[i+1]);
			break;
		} else{
			base = 10;
		}	
	}


	struct scan_table_st scan_table; // table of tokens
    struct parse_table_st parse_table; // table of parse nodes
    struct parse_node_st *parse_tree; // tree (pointers only) of parse nodes
   

    char input[SCAN_INPUT_LEN];
    int len;

 
    strncpy(input, argv[2], SCAN_INPUT_LEN);
    len = strnlen(input, SCAN_INPUT_LEN);

    scan_table_init(&scan_table); //initailize scan table
    scan_table_scan(&scan_table, input, len); //make scan_tokens
    parse_table_init(&parse_table); 
    parse_tree = parse_program(&scan_table); //make parse_tree of scan_tokens

	uint32_t result = eval_tree(parse_tree); //evaluate tree
	int_to_string(result, base); //convert result into a string with given base

        
    return 0;
	
}
