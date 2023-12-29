#include "lab06.h"


uint32_t eval_tree(struct parse_node_st *node){

	//if node is an intval we are at a leaf in our tree and return the value 
	//to the earlier recursive call
	
	if(node->type == EX_INTVAL){
		return node->intval.value;

	//if node is an oper2 recursively call function to get int value on left and right 
	//then return the values to whatever operand the node is
		
	} else if(node->type == EX_OPER2){
		int left = eval_tree(node->oper2.left);
		int right = eval_tree(node->oper2.right);
		if(node->oper2.oper == OP_PLUS){
			return left + right;
		} else if(node->oper2.oper == OP_MINUS){
			return left -right;
		} else if(node->oper2.oper == OP_MULT){
			return left * right;
		}else if(node->oper2.oper == OP_DIV){
			return left / right;
		} else if(node->oper2.oper == OP_LSHIFT){
			return left << right;
		} else if(node->oper2.oper == OP_RSHIFT){
			return left >> right;
		}  else if(node->oper2.oper == OP_AND){
			return left & right;
		} else if(node->oper2.oper == OP_OR){
			return left | right;
		}else if(node->oper2.oper == OP_XOR){
			return left ^ right;
		}

	//if node is oper1 recursively call eval_tree to get the int value associated with it
	//then return the int to the operand

	} else if(node->type == EX_OPER1){
		int num = eval_tree(node->oper1.operand);
		if (node->oper1.oper == OP_NOT){
			return ~num;
		} else if(node->oper1.oper == OP_MINUS){
			return -num;
		}
		
	}
}

char *reverse_string(char *strVer, int base){
	int revSize;
	char *rev;
	size_t j = 2;

	//when base == 10 the size is just set 

	if(base == 10){
		revSize = 11;
		rev = (char *)malloc(revSize);

	//when base ==16 add  the '0x' at the beginning and then as then leading zeros
	//dependeant on the size of the numeric answer to make a 32 bit integer

	
	} else if (base ==16){
		revSize = 11;
		rev = (char *)malloc(revSize);
		//calculate leading zeros
		rev[0] = '0';
		rev[1] = 'x';
		if(strlen(strVer) <= 8){
			while(j <= 9-strlen(strVer)){
				rev[j++] = '0';
		 	}	
		}
		
	//when base ==2 add  the '0b' at the beginning and then as then leading zeros
	//dependeant on the size of the numeric answer to make a 32 bit integer
		
	} else if (base == 2){
		revSize = 35;
		rev = (char *)malloc(revSize);
		//calculate leading zeros
		rev[0] = '0';
		rev[1] = 'b';
		while(j <= 33-strlen(strVer)){
			rev[j++] = '0';
	 	}
		
	}

	//now that everything has been added (prefix and leading zeros) reverse string
	//by setting front index or reverse to back index of orgional

	for(int i = strlen(strVer) -1; i>=0; i--){
		rev[j++] = strVer[i];

	}

	//and termination and print
	
	rev[j] = '\0';
	printf(rev);
	return rev;
}

void *int_to_string(uint32_t result, int base){
	char strVer[100];
	int len = 0;
	char strRem;

	//divide result by base, adding remainder to string version
	//once result is 0, it means that the int has been successfuly 
	//converted into the given base
	
	
	if(base ==2){
		while(true){
			if(result == 0){
				break;
			}
			int rem = result % base;
			strRem = rem +48;
			strVer[len] = strRem;
			len++;
		
			result = result / base;
		}
	}

	//in base 16 if the number is greater than 9, conver to the hex char

	if(base ==16){
		while(true){
			
			if(result == 0){
				break;
			}
			int rem = result % base;
			if(rem >= 10){
				strRem = rem +55;
			} else{
				strRem = rem +48;
			}
			strVer[len] = strRem;
			len++;
			
			result = result / base;
		}
		strVer[len] = '\0';	
	}
	if(base == 10){
		printf("%d", result);
		
	} else{
		reverse_string(strVer, base);
		
	}

	
}
