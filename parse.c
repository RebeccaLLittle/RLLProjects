// parse.c - parsing and parse tree construction

#include "lab06.h"


void parse_table_init(struct parse_table_st *parse_table) {
    parse_table->root = NULL;
}

// Allocate a parse node from the table of parse nodes
struct parse_node_st * parse_node_new() {
    return calloc(1, sizeof(struct parse_node_st));
}

void parse_error(char *err) {
    printf("parse_error: %s\n", err);
    exit(-1);
}

// These are names of operators for printing
char *parse_oper_strings[] = {"PLUS", "MINUS", "MULT", "DIV"};

// Print the dots which represent tree depth in the output
void parse_tree_print_indent(int level) {
    level *= 2;
    for (int i = 0; i < level; i++) {
        printf(".");
    }
}


//For type OPER1 determins the type of oper and returns it to be printed 

char *get_oper_string(struct parse_node_st *node){
	 if(node->oper1.oper == OP_PLUS){
		return "PLUS";		
	} else if(node->oper1.oper == OP_MINUS){
		return "MINUS";
	} else if(node->oper1.oper == OP_MULT){
		return "MULT";
	} else if(node->oper1.oper == OP_DIV){
		return "DIV";
	}
		
}

// Traverse the tree recursively to print out the structs

void parse_tree_print_expr(struct parse_node_st *node, int level) {

    parse_tree_print_indent(level);
    printf("EXPR ");
    
    if (node->type == EX_INTVAL) {
        printf("INTVAL %d\n", node->intval.value);
        
    } else if (node->type == EX_OPER2) {
        printf("OPER2 %s\n", parse_oper_strings[node->oper2.oper]);
        parse_tree_print_expr(node->oper2.left, level + 1);
        parse_tree_print_expr(node->oper2.right, level + 1);
   
    } else if(node->type == EX_OPER1){
    	printf("OPER1 %s\n", get_oper_string(node));
    	parse_tree_print_expr(node->oper1.operand, level +1);
    }
}

void parse_tree_print(struct parse_node_st *np) {
    parse_tree_print_expr(np, 0);
}

// Parse the "program" part of the EBNF
// A program is composed of an expression followed by EOT
struct parse_node_st * parse_program(struct scan_table_st *scan_table) {
    struct parse_node_st *root;

    root = parse_expression(scan_table);

    if (!scan_table_accept(scan_table, TK_EOT)) {
        parse_error("Expecting EOT");        
    }

    return root;                                       
}



// Build the tree for expressions
// Expressions are defined in the EBNF as an operator followed
// by zero or more operator operand pairs

//if(tp id = +-/*) scan table accept
//helper function to get op_plus and  passes in id and if id= equals + 

enum parse_oper_enum parse_node_helper(struct scan_token_st *token){
	if(token->id == TK_PLUS){
		return OP_PLUS;
	}if(token->id == TK_MINUS){
		return OP_MINUS;
	}if(token->id == TK_MULT){
		return OP_MULT;
	}if(token->id == TK_DIV){
		return OP_DIV;
	}if(token->id == TK_LSHIFT){
		return OP_LSHIFT;
	}if(token->id == TK_RSHIFT){
		return OP_RSHIFT;
	}if(token->id == TK_AND){
		return OP_AND;
	}if(token->id == TK_OR){
		return OP_OR;
	}if(token->id == TK_NOT){
		return OP_NOT;
	}if(token->id == TK_XOR){
		return OP_XOR;
	}		
}


struct parse_node_st * parse_expression(struct scan_table_st *scan_table) {

    struct scan_token_st *token;
    struct parse_node_st *node1, *node2;

    node1 = parse_operand(scan_table);

    
	while (true){
		token = scan_table_get(scan_table, 0);

		if(token->id == TK_PLUS || token->id == TK_MINUS || token->id == TK_MULT ||token->id == TK_DIV ||token->id == TK_RSHIFT ||token->id == TK_LSHIFT ||token->id == TK_AND ||token->id == TK_OR || token->id == TK_XOR ||token->id == TK_NOT){
			scan_table_accept(scan_table, TK_ANY);
	       	node2 = parse_node_new();
	       	node2->type = EX_OPER2;
	       	node2->oper2.oper = parse_node_helper(token);
	       	node2->oper2.left = node1;
	       	node2->oper2.right = parse_operand(scan_table);
	       	node1 = node2;
		} else{
			break;
		}
	}

    return node1;
}



// Parse operands, which are defined in the EBNF to be 
// integer literals or unary minus or expressions



struct parse_node_st *parse_operand(struct scan_table_st *scan_table) {
    struct scan_token_st *token;
    struct parse_node_st *node;


    if (scan_table_accept(scan_table, TK_INTLIT)) {
        token = scan_table_get(scan_table, -1);
        node = parse_node_new();
        node->type = EX_INTVAL;
        node->intval.value = string_to_int(token->name, 10);
        
    } else if(scan_table_accept(scan_table, TK_BINLIT)){
    	token = scan_table_get(scan_table, -1);
    	node = parse_node_new();
    	node->type = EX_INTVAL;
    	node->intval.value = string_to_int(token->name, 2);
    	
    }else if(scan_table_accept(scan_table, TK_HEXLIT)){
    	token = scan_table_get(scan_table, -1);
    	node = parse_node_new();
    	node->type = EX_INTVAL;
    	node->intval.value = string_to_int(token->name, 16);


    //EBNF says a ( must be followed be a expression, so "recursively"
    //call parse_expression, and afterwards if we do not have ) it
    //means the input is not valid
    
    } else if(scan_table_accept(scan_table, TK_LPAREN)){
    	node = parse_expression(scan_table);
    
    	//if not accept then bogus
    	if(!scan_table_accept(scan_table, TK_RPAREN)){
    		parse_error("Bad operand");	
    	}

	//According to the EBNF a OPER1 minus and OPER not must be followed by a operand
	//so we "recursively" call parse_operand
	
    } else if(scan_table_accept(scan_table, TK_MINUS)){
    	node = parse_node_new();
    	node->type = EX_OPER1;
    	node->oper1.oper = OP_MINUS;
    	node->oper1.operand = parse_operand(scan_table);
    	
    }  else if(scan_table_accept(scan_table, TK_NOT)){
    	node = parse_node_new();
    	node->type = EX_OPER1;
    	node->oper1.oper = OP_NOT;
    	node->oper1.operand = parse_operand(scan_table);
    	
    } else {
    	parse_error("Bad operand");
    }

    return node;
    
}