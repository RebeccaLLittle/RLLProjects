#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/param.h>

/*
	make board bigger than necassary so that it works for all sizes. 
*/

#define BOARD_SZ 5
#define EMPTY '_'

int SIZE;

typedef char board_t[BOARD_SZ][BOARD_SZ];


/*
	Struct made to hold row and column position so that it can be returned

*/


struct position{
	int row;
	int column;
};


/*
	2 init_board function's one that runs the default 3 and another that runs for all other sizes
	
*/

void init_board(board_t board){

	for(int i =0; i < 3; i++){
		for(int j =0; j < 3; j++){
			board[i][j] = '_';
		}
	}
	
	
}

void init_board_size(board_t board, char *argv){
	int len =0;
	for(int i =0; i < SIZE; i++){
		for(int j =0; j < SIZE; j++){
			board[i][j] = argv[len++];
		}
	}
}


/*
	2 prints for the differnt sizes, done for simplicity
	
*/

void print_board(board_t board){
	printf(" %c | %c | %c \n" , board[0][0], board[0][1], board[0][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c \n",  board[1][0], board[1][1], board[1][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c \n",  board[2][0], board[2][1], board[2][2]);
	
}

void print_board_size(board_t board){
	printf(" %c | %c | %c | %c \n" , board[0][0], board[0][1], board[0][2], board[0][3]);
	printf("---+---+---+---\n");
	printf(" %c | %c | %c | %c \n",  board[1][0], board[1][1], board[1][2], board[1][3]);
	printf("---+---+---+---\n");
	printf(" %c | %c | %c | %c \n",  board[2][0], board[2][1], board[2][2], board[2][3]);
	printf("---+---+---+---\n");
	printf(" %c | %c | %c | %c \n",  board[3][0], board[3][1], board[3][2], board[3][3]);
}

/*
	Checks for a winner, seeing all 3 in a row combinations
*/

int check_board(board_t board){

	for(int i = 0; i<3; i++){
		if(board[i][0]== 'X' && board[i][1]== 'X' && board[i][2]== 'X'){
			return 1;
		}
		if(board[i][0]== 'O' && board[i][1]== 'O' && board[i][2]== 'O'){
			return -1;
		}
		
	}

	for(int j = 0; j<3; j++){
		if(board[0][j]== 'X' && board[1][j]== 'X' && board[2][j]== 'X'){
			return 1;
		}
		if(board[0][j]== 'O' && board[1][j]== 'O' && board[2][j]== 'O'){
			return -1;
		}
	}


	if(board[0][0]== 'X' && board[1][1]== 'X' && board[2][2]== 'X'){
		return 1;
	}
	if(board[0][0]== 'O' && board[1][1]== 'O' && board[2][2]== 'O'){
		return -1;
	}

	if(board[0][2]== 'X' && board[1][1]== 'X' && board[2][0]== 'X'){
		return 1;
	}
	if(board[0][2]== 'O' && board[1][1]== 'O' && board[2][0]== 'O'){
		return -1;
	}


	return 0;
		
	
}

/*
	Updates the board with the position user enterd

*/


void update_board(board_t board, char *input, char *player){

	int row = input[1] - '0';
	int column = input[4] - '0';
	board[row][column] = *player;

}

/*
	Checks if the board is full, meaning it is a draw and returns that result

*/

bool draw(board_t board){

	bool isDraw = true;
	for(int i =0; i < SIZE; i++){
		for(int j =0; j < SIZE; j++){
			if(board[i][j] != '_')
				isDraw = false;
		}
	}
	return isDraw;
}

/*	
	Minimax is sued to check all possible board combinations given an input to find the 
	best possible position. Finds this by seeing if the move results in a win for the user
	using the least amount of moves. The function retursn the bestscore.

*/

	
int minimax(board_t board, int depth, bool mini){



	
	int result = check_board(board);
	int score;

	if(result == 1){
		return 10  - depth;
	}
	
	if(result == -1){
		return -10 +depth;
	}
	else if(draw(board)){
		return 0;
	}
	if(mini){
		score = 100;
		for(int i = 0; i <SIZE; i++){
			for(int j =0; j <SIZE; j++){
				if(board[i][j] == '_'){
					board[i][j] = 'O';
					score = MIN(score, minimax(board, depth+1, false));
					board[i][j]  = '_';
				}
		
			}
		}
		return score;
			
	}
	if(!mini){
		score = -100;
		for(int i = 0; i <SIZE; i++){
			for(int j =0; j <SIZE; j++){
				if(board[i][j] == '_'){
					board[i][j] = 'X';
					score = MAX(score, minimax(board, depth+1, false));
					board[i][j]  = '_';
				}
		
			}
		}
		return score;
		
	}
}

/*
	FindBestMove iterates through all the possible moves for O and calls minimax to find what score that postion 
	makes. The highest scoring position gets returned

*/

struct position findBestMove(board_t board){

	int max = 100;
	int temp;
	struct position move;
	bool empty = true;

	for(int i =0; i < SIZE; i++){
		for(int j =0; j < SIZE; j++){
			if(board[i][j] != '_')
				empty = false;
		}
	}
	if(empty){
		move.row = 0;
		move.column =0;
		return move;
	}

	
	
	for(int i =0; i <SIZE; i++){
		for(int j = 0; j <SIZE; j++){
			if(board[i][j] == '_'){
				board[i][j] = 'O';
				temp = minimax(board, true, 0);
				if(temp < max){
					max = temp;
					move.row = i;
					move.column = j;
				}
				board[i][j] = '_';
				
			}
		}
	}

	return move;
	
}

/*
	PlayGame function switches turns between x and o, allowing player to chose a position for x and then the
	ai plays the o position

*/



int playGame(){

	char input[10];
	board_t b;
	init_board(b);
	bool isX = true;
	int winner = 0;
	struct position pos;

	//passing x and o to each function
	
	while(true){

		if(isX){
			print_board(b);
			printf("X's turn:");
			fscanf(stdin, "%10s", input);
	 		update_board(b, input, "X");
			print_board(b);
			isX = false;
			winner = check_board(b);
			if(winner == 1){
				return winner;
			}
				
		}
		if(!isX){
			printf("O's turn\n");
			pos = findBestMove(b);
			b[pos.row][pos.column] = 'O';
			print_board(b);
			isX = true;
			winner = check_board(b);
			if(winner == -1){
				return winner;
			}
			
		}
		if(draw(b)){
			return 0;
		}
	}
	
}


int main(int argc, char **argv){


	char bEntry[32];
	struct position move;
	

	if(argc >9){
		board_t board;
		
		
		if(strcmp(argv[1], "-s") ==0){

			SIZE = atoi(argv[2]);
			for(int i =3; i < argc; i++){
				strcat(bEntry, argv[i]);
			}
		
			init_board_size(board, bEntry);
			move = findBestMove(board);
			print_board_size(board);
			printf("O: %d %d", move.row, move.column);
			
			
		} else{
			SIZE = 3;
			for(int i =1; i < argc; i++){
				strcat(bEntry, argv[i]);
			}
			init_board_size(board, bEntry);
			move = findBestMove(board);
			print_board(board);
			printf("O: %d %d", move.row, move.column);
		}
	

		
	} else {
		SIZE =3;
		int win =playGame();
		
		if(win ==1){
			printf("X wins");
		}
		else if(win ==-1){
			printf("O wins");
		}
		else{
			printf("draw");
		}
	}
	return 0;
	
}
