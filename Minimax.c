
#include "Chess.h"
/*the main minimax function
 * col - the color of the player initially called the minimax (decides who's the maxi)
 * depth - the initial depth of the the "tree"
 * alpha - an alpha score for the given node
 * beta - a beta score for the given node
 * printingBestMoves - a flag to indicate whether to print the best moves
 */
move* minimaxRoot (int col, int depth, int alpha, int beta, int printingBestMoves){

	move *mov;
	mov = (move*)malloc(sizeof(move));
	if (mov == NULL){
		perror("Error: standard function malloc has failed");
		quit();
	}
	moveNode *printingList = NULL, *tail = NULL;
	if(depth == 0){
		mov->value = scoreA(col);
		return mov;
	}else if (depth == -5){
		mov->value = scoreB(col);
		return mov;
	}else {
		mov->value = INT_MIN;
	}
	move* son = NULL;
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			//found an own piece
			if( (board[i][j] > 0  && col == 0) || (board[i][j] < 0 && col == 1) ){

				for(int ii = 0; ii < BOARD_SIZE; ii++){
					for(int jj = 0; jj < BOARD_SIZE; jj++){
						//found other player piece or empty slot
						if( (board[ii][jj] <= 0  && col == 0) || (board[ii][jj] >= 0 && col == 1)
								|| (i != ii && j != jj)){
							//create the move to be sent for validation
							loc fromCheck = createLoc(i,j);
							loc toCheck = createLoc(ii,jj);
							move *moveToCheck = createmove(fromCheck, toCheck);
							//check validation
							if(isValid(*moveToCheck, col, abs(board[i][j])-1, 0)){
								son = minimax(moveToCheck, alpha, beta, col, depth - 1, 0, "queen");
								//value update
								if (son->value > mov->value) {
									mov->value = son->value;
									copyMove(son, mov);
									if(printingBestMoves){
										if(printingList != NULL){
											delMoveNodes(printingList);
										}
										tail = printingList = (moveNode*)malloc(sizeof(moveNode));
										if (printingList == NULL){
											perror("Error: standard function malloc has failed");
											quit();
										}
										tail->data = (move*)malloc(sizeof(move));
										if(tail->data == NULL){
											perror("Error: standard function malloc has failed");
											quit();
										}
										copyMove(son, tail->data);
										tail->nextMove = NULL;
									}
								} else if (printingBestMoves && (son->value == mov->value) ){
									tail->nextMove = (moveNode*)malloc(sizeof(moveNode));
									if(tail->nextMove == NULL){
										perror("Error: standard function malloc has failed");
										quit();
									}
									tail = tail->nextMove;
									tail->data = (move*)malloc(sizeof(move));
									if(tail->data == NULL){
										perror("Error: standard function malloc has failed");
										quit();
									}
									copyMove(son, tail->data);
									tail->nextMove = NULL;
								}

								//alpha update
								if (son->value > alpha){
									alpha = son->value;
								}

								//pruning condition
								if (beta < mov->value){
									if(printingBestMoves){
										print_moveList(printingList);
										delMoveNodes(printingList);
									}
									delMove(moveToCheck);
									return mov;
								}
							}
							delMove(moveToCheck);
						}
					}
				}
			}
		}
	}
	if(printingBestMoves){
		print_moveList(printingList);
		if (AIMove == NULL){
			AIMove = (move*) malloc (sizeof(move));
			if(AIMove == NULL){
				perror("Error: standard function malloc has failed");
				quit();
			}
		}
		copyMove(printingList->data, AIMove);
		delMoveNodes(printingList);
	}
//	printf("node score: %d\n", mov->value);
	return mov;
}


/*the main minimax function
 * node - the move to be simulated
 * alpha - an alpha score for the given node
 * beta - a beta score for the given node
 * col - the color of the player initially called the minimax (decides who's the maxi)
 * depth - the depth of the node in the "tree"
 * maxi - 0 = mini node / 1 = maxi node
 */
move* minimax(move* node, int alpha, int beta, int col, int depth, int maxi, char* prom){

	loc from = node->from;
	loc to = node->target;
	int originalFromType = abs(board[node->from.column][node->from.row]) -1;
	int fromType = originalFromType; //in case of promotion
	int toType = abs(board[node->target.column][node->target.row]) -1;

	//simulate move
	int movCol = (maxi == 0) ? col : 1-col;
	rm(to.column, to.row, 0);
	rm(from.column, from.row, 0);
	//in case of pawn promotion
	if (originalFromType == 0 && to.row == 7*(1-movCol)) {
		fromType = piece2num(prom);
	}
	set(to.column, to.row, movCol, fromType, 0);

	int isEnd = CMT(movCol);
	if( isEnd == 3 ){
		node->value = 4500*(pow(-1, maxi));
		//undo the move made by "node" before return up
		rm(to.column, to.row, 0);
		set(to.column, to.row, 1-movCol, toType, 0);
		set(from.column, from.row, movCol, originalFromType, 0);
		return node;
	}
	else if( isEnd== 2 ){
		node->value = 4390*(pow(-1, maxi));
		//undo the move made by "node" before return up
		rm(to.column, to.row, 0);
		set(to.column, to.row, 1-movCol, toType, 0);
		set(from.column, from.row, movCol, originalFromType, 0);
		return node;
	}


	//leaf
	if(depth == 0 || depth ==-5){
		if (depth ==0) node->value = scoreA(col);
		else node->value = scoreB(col);
		//undo the move made by "node" before return up
		rm(to.column, to.row, 0);
		set(to.column, to.row, 1-movCol, toType, 0);
		set(from.column, from.row, movCol, originalFromType, 0);
		return node;
	}

	//minimax algorithm
	move* son = NULL;
	//if maxi
	if (maxi) {
		node->value = INT_MIN;
		for(int i = 0; i < BOARD_SIZE; i++){
			for(int j = 0; j < BOARD_SIZE; j++){
				//found own piece
				if( (board[i][j] > 0  && movCol == 1) || (board[i][j] < 0 && movCol == 0) ){

					for(int ii = 0; ii < BOARD_SIZE; ii++){
						for(int jj = 0; jj < BOARD_SIZE; jj++){
							//found other player piece
							if( (board[ii][jj] <= 0  && movCol == 1) || (board[ii][jj] >= 0 && movCol == 0) ){
								//create the move to sent for validation
								loc fromCheck = createLoc(i,j);
								loc toCheck = createLoc(ii,jj);
								move *moveToCheck = createmove(fromCheck, toCheck);
								//check validation
								if(isValid(*moveToCheck, 1-movCol, abs(board[i][j])-1, 0)){
									son = minimax(moveToCheck, alpha, beta, col, depth - 1, 0, "queen");
									//value update
									node->value = (node->value > son->value) ? node->value : son->value;
									//alpha update
									alpha = (son->value > alpha) ?  son->value : alpha;
									//pruning condition
									if (beta < node->value){
										delMove(moveToCheck);
										//undo the move made by "node" before return up
										rm(to.column, to.row, 0);
										set(to.column, to.row, 1-movCol, toType, 0);
										set(from.column, from.row, movCol, originalFromType, 0);
										return node;
									}
								}
								delMove(moveToCheck);
							}
						}
					}
				}
			}
		}
	}
	//(if not maxi node)
	else {
		node->value = INT_MAX;
		for(int i = 0; i < BOARD_SIZE; i++){
			for(int j = 0; j < BOARD_SIZE; j++){
				//found own piece
				if( (board[i][j] > 0  && movCol == 1) || (board[i][j] < 0 && movCol == 0) ){

					for(int ii = 0; ii < BOARD_SIZE; ii++){
						for(int jj = 0; jj < BOARD_SIZE; jj++){
							//found other player piece
							if( (board[ii][jj] <= 0  && movCol == 1) || (board[ii][jj] >= 0 && movCol == 0) ){
								//create the move to sent for validation
								loc fromCheck = createLoc(i,j);
								loc toCheck = createLoc(ii,jj);
								move *moveToCheck = createmove(fromCheck, toCheck);
								//check validation
								if(isValid(*moveToCheck, 1-movCol, abs(board[i][j])-1, 0)){
									son = minimax(moveToCheck, alpha, beta, col, depth - 1, 1, "queen");
									node->value = (node->value < son->value) ? node->value : son->value;
									beta = (beta < son->value) ? beta : son->value;
									//pruning condition
									if (alpha > node->value){
										delMove(moveToCheck);
										//undo the move made by "node" before return up
										rm(to.column, to.row, 0);
										set(to.column, to.row, 1-movCol, toType, 0);
										set(from.column, from.row, movCol, originalFromType, 0);
										return node;
									}
								}
								delMove(moveToCheck);
							}
						}
					}
				}
			}
		}
	}

	//undo the move made by "node" before return up
	rm(to.column, to.row, 0);
	set(to.column, to.row, 1-movCol, toType, 0);
	set(from.column, from.row, movCol, originalFromType, 0);
	return node;
}


/*end minimax functions*/
