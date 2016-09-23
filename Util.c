
#include "Chess.h"


/*	basic and setting state functions	*/

void quit() {
	if(AIMove != NULL){
		delMove(AIMove);
		AIMove = NULL;
	}
	exit(0);
}

/*
 * check if the starting board is legal
 */
int isLegalStart(){
	// checks if there are two kings of different color to start the game
	if (pieces[5][0] != 1 || pieces[5][1]!= 1){
		printf("Wrong board initialization\n");
		return 0;
	}
	// checks if the first player is set to state of tie
	if (isTie(whosTurn)){
		printf("First player Stuck!\n");
		return 0;
	}
	//checks if there is a pawn at the edge of the board
	for(int i = 0; i < BOARD_SIZE; i++){
		if(board[i][0] == -1){
			printf("black pawn in <%c,1> should have been promoted\n", ('a'+i));
			return 0;
		}
		if(board[i][7] == 1){
			printf("White pawn in <%c,8> should have been promoted\n", ('a'+i));
			return 0;
		}
	}
	return 1;

}


/* checks in the global "pieces" variable
 * if the amount of a specified piece type
 * can be changed
 */
int checkSetLimit(int col, int piece){
	if (piece == 0 && ((col == 0 && pieces[0][0] == 8) || (col == 1 && pieces[0][1] == 8)))
		return 0;
	else if (piece == 1 && ((col == 0 && pieces[1][0] == 2)|| (col == 1 && pieces[1][1] == 2)))
		return 0;
	else if (piece == 2 && ((col == 0 && pieces[2][0] == 2)|| (col == 1 && pieces[2][1] == 2)))
		return 0;
	else if (piece == 3 && ((col == 0 && pieces[3][0] == 2)|| (col == 1 && pieces[3][1] == 2)))
		return 0;
	else if (piece == 4 && ((col == 0 && pieces[4][0] == 1)|| (col == 1 && pieces[4][1] == 1)))
		return 0;
	else if (piece == 5 && ((col == 0 && pieces[5][0] == 1)|| (col == 1 && pieces[5][1] == 1)))
		return 0;
	return 1;
}


/*	end basic and setting state functions	*/



/*	moving mechanism	*/

/*	checking that a move won't cause an own Check	*/
int isOwnCheck(move mov, int col, int type){

	int toType = abs(board[mov.target.column][mov.target.row]) - 1;
	//simulate the move on the board
	rm(mov.from.column, mov.from.row, 0);
	rm(mov.target.column, mov.target.row, 0);
	set(mov.target.column, mov.target.row, col, type, 0);
	//check for tie with "isCheck" function
	if (isCheck(1-col) == 1){
		// return to previous board
		rm(mov.target.column, mov.target.row, 0);
		set(mov.from.column,mov.from.row, col, type, 0);
		set(mov.target.column, mov.target.row, 1-col, toType, 0);
		return 1;
	}
	// return to previous board
	rm(mov.target.column, mov.target.row, 0);
	set(mov.from.column,mov.from.row, col, type, 0);
	set(mov.target.column, mov.target.row, 1-col, toType, 0);
	return 0;
}

/* sending a given move to the type specified valid function
 * after checking the following global move conditioning:
 * 	valid given position
 * 	from position not empty on the board
 * 	from position is occupied by the right color
 * 	the target is either empty or occupied with an opposite piece
 * 	move doesn't stay in place
 * 	move doesn't inflicts self check
 */
int isValid(move mov, int col, int type, int printFlag) {
	/*	checks valid position	*/
	if (mov.from.column > 7 || mov.from.column < 0 || mov.from.row > 7
			|| mov.from.row < 0) {
		if (printFlag) print_message(WRONG_POSITION);
		return 0;
	}
	if (mov.target.column > 7 || mov.target.column < 0 || mov.target.row > 7
			|| mov.target.row < 0) {
		if (printFlag) print_message(WRONG_POSITION);
		return 0;
	}
	/*	check that slot is not empty	*/
	if (board[mov.from.column][mov.from.row] == 0) {
		if (printFlag) print_message("The specified position does not contain your piece\n");
		return 0;
	}
	/*	checks right color	*/
	if (((board[mov.from.column][mov.from.row]<0) && col == 0) ||
			((board[mov.from.column][mov.from.row]>0) && col == 1)){
		if (printFlag) print_message("The specified position does not contain your piece\n");
		return 0;
	}
	/*	target must contain the opposite piece	*/
	if ((board[mov.target.column][mov.target.row]<0 && col == 1) ||
			(board[mov.target.column][mov.target.row]>0 && col == 0)){
		if (printFlag) print_message(ILLEGAL_MOVE);
		return 0;
	}
	/*	can't stay in the same place	*/
	if ( (mov.from.column == mov.target.column) && (mov.from.row == mov.target.row) ){
		if (printFlag) print_message(ILLEGAL_MOVE);
		return 0;
	}


	/*	send to specific isValids per piece type*/
	switch (type){
		case(0) :
				if( isValidPawn(mov, col))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		case (1) :
				if( isValidRook(mov))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		case (2):
				if( isValidKnight(mov))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		case (3) :
				if( isValidBishop(mov))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		case (4) :
				if( isValidQueen(mov))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		case (5) :
				if( isValidKing(mov))
					break;
				else{
					if (printFlag) print_message(ILLEGAL_MOVE);
					return 0;
				}
		}

	//check that the move doesn't put your own king at risk
	if (isOwnCheck(mov, col, type)){
		if (printFlag) print_message(ILLEGAL_MOVE);
		return 0;
	}

	return 1;
}


/* checking that a specific piece move is legal for a given board
 * don't need to check basic validations (checked at "isValid" function)
 */
int isValidPawn(move mov, int col){
	//not walking backwards
	if (((mov.target.row - mov.from.row) <0 && col==0)
			|| ((mov.target.row - mov.from.row) >0 && col==1)){
		return 0;
	}
	//not eating illegally
	if (board[mov.target.column][mov.target.row] == 0){
		//target column must be the same as origin
		if (mov.target.column != mov.from.column){
			return 0;
		}
		//check that the move is either 1 slots away
		if ( abs(mov.target.row - mov.from.row) != 1) {
			return 0;
		}
		return 1;
	}
	//eating move conditions
	else{
		//must be diagonal a 1 slot move
		if (col == 0){
			if (mov.target.row - mov.from.row !=1 || abs(mov.target.column - mov.from.column) !=1){
				return 0;
			}
		}else
			if (mov.from.row - mov.target.row !=1 || abs(mov.from.column - mov.target.column) !=1){
				return 0;
			}
		return 1;
	}
}

int isValidRook(move mov){
	//can't move diagonally
	if ( (mov.target.row != mov.from.row) && (mov.target.column != mov.from.column) ){
		return 0;
	}
	//can't skip above other pieces
	//same row
	if (mov.target.row == mov.from.row) {
		int colDistance = mov.target.column - mov.from.column;
		int direction = (colDistance > 0) ? 1 : -1;
		for (int i=1; i<abs(colDistance) ; i++ ){
			if (board[mov.from.column+(i*direction)][mov.from.row] !=0){
				return 0;
			}
		}
	}
	//same column
	else{
		int rowDistance = mov.target.row - mov.from.row;
		int direction = rowDistance>0 ? 1 : -1;
		for (int i=1; i<abs(rowDistance) ; i++ ){
			if (board[mov.from.column][mov.from.row+(i*direction)] !=0){
				return 0;
			}
		}
	}
	return 1;
}

int isValidKnight(move mov){

	//check that target is in the right pattern for a knight
	if ( ( (abs(mov.target.row - mov.from.row)==2) &&
			(abs(mov.target.column - mov.from.column)==1) ) ||
			( (abs(mov.target.row - mov.from.row)==1) &&
					(abs(mov.target.column - mov.from.column)==2) ) )
		return 1;
	return 0;
}

int isValidBishop(move mov){
	//must move diagonally
	if ( abs(mov.target.column - mov.from.column) != abs(mov.target.row - mov.from.row) ){
					return 0;
	}
	//must not skip above other pieces
	int diagDist = abs(mov.target.column - mov.from.column);
	int colDirect = mov.target.column > mov.from.column ? 1 : -1;
	int rowDirect = mov.target.row > mov.from.row ? 1 : -1;
	for (int i=1 ; i< diagDist ; i++){
		if (board[mov.from.column + (i*colDirect)][mov.from.row+(i*rowDirect)] !=0){
			return 0;
		}
	}
	return 1;
}

int isValidQueen(move mov){
	//the queen conditions are in fact the combination of both bishop and rook
	return (isValidBishop(mov) ||
			isValidRook(mov));
}

int isValidKing(move mov){
	//check that target is in the right pattern for a knight
	if ( (abs(mov.target.row - mov.from.row) ==1) &&
			(abs(mov.target.column - mov.from.column) <=1) )
		return 1;
	if ( (abs(mov.target.row - mov.from.row) <=1) &&
			(abs(mov.target.column - mov.from.column) ==1) )
		return 1;
	return 0;
}

/* returning the location of the opponent king
 */
loc wheresOppKing (int col){
	loc output;
	for (int i =0; i<8; i++)
		for (int j =0; j<8; j++)
			if ( (board[i][j] == 6 && col==1) || (board[i][j] == -6 && col==0) )
				output = createLoc (i,j);
	return output;
}


//check if after a move, the opposite king is threatened ("col" is the player that made the move)
int isCheck(int col){
	loc oppKingSpot = wheresOppKing(col);
	int kingCol = oppKingSpot.column;
	int kingRow = oppKingSpot.row;
	//check if one of my knights threat the king
	if ( (kingRow+2) <=7 && (kingCol+1<=7) )
		if (board[kingCol+1][kingRow+2] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow+2) <=7 && (kingCol-1>=0) )
		if (board[kingCol-1][kingRow+2] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow-2) >=0 && (kingCol+1<=7) )
		if (board[kingCol+1][kingRow-2] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow-2) >=0 && (kingCol-1>=0) )
		if (board[kingCol-1][kingRow-2] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow+1) <=7 && (kingCol+2<=7) )
		if (board[kingCol+2][kingRow+1] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow-1) >=0 && (kingCol+2<=7) )
		if (board[kingCol+2][kingRow-1] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow+1) <=7 && (kingCol-2>=0) )
		if (board[kingCol-2][kingRow+1] == (pow(-1, col)*3)){
			return 1;
		}
	if ( (kingRow-1) >=0 && (kingCol-2>=0) )
		if (board[kingCol-2][kingRow-1] == (pow(-1, col)*3)){
			return 1;
		}

	//checking all 8 directions for threatening the opposite king
	int i;
	int j;
	//straight up
	//the king is the threatening
	if (kingRow+1<8 && board[kingCol][kingRow+1] == (pow(-1, col)*6 )){
		return 1;
	}
	for (i=kingCol, j=kingRow+1; j<8; j++){
		//if its rook or queen
		if ( (board[i][j] == pow(-1, col)*2) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j] ==0)
			continue;
		else
			break;
	}
	//straight down
	//the king is the threatening
	if (kingRow-1>=0 && board[kingCol][kingRow-1] == (pow(-1, col)*6 )){
		return 1;
	}
	for (i=kingCol, j=kingRow-1; j>=0; j--){
		//if its rook or queen
		if ( (board[i][j] == pow(-1, col)*2) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j] ==0)
			continue;
		else
			break;
	}
	//straight right
	//the king is the threatening
	if (kingCol+1<8 && board[kingCol+1][kingRow] == (pow(-1, col)*6 )){
		return 1;
	}
	for (i=kingCol+1, j=kingRow; i<8; i++){
		//if its rook or queen
		if ( (board[i][j] == pow(-1, col)*2) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j] ==0)
			continue;
		else
			break;
	}
	//straight left
	//the king is the threatening
	if (kingCol-1>=0 && board[kingCol-1][kingRow] == (pow(-1, col)*6 )){
		return 1;
	}
	for (i=kingCol-1, j=kingRow; i>=0; i--){
		//if its rook or queen
		if ( (board[i][j] == pow(-1, col)*2) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j] ==0)
			continue;
		else
			break;
	}
	//diagonal upright
	if (kingCol+1<8 && kingRow+1<8){
		//the king is the threatening
		if (board[kingCol+1][kingRow+1] == (pow(-1, col)*6 )){
			return 1;
		}
		//black pawn is the threatening
		if (col==1 && board[kingCol+1][kingRow+1] == (pow(-1, col)*1 )){
			return 1;
		}
	}
	for (i=kingCol+1, j=kingRow+1; i<8 && j<8; i++,j++){
		//if its bishop or queen
		if ( (board[i][j] == pow(-1, col)*4) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j]==0)
			continue;
		else
			break;
	}
	// diagonal upleft
	if (kingCol-1>=0 && kingRow+1<8){
		//the king is the threatening
		if (board[kingCol-1][kingRow+1] == (pow(-1, col)*6 )){
			return 1;
		}
		//black pawn is the threatening
		if (col==1 && board[kingCol-1][kingRow+1] == (pow(-1, col)*1 )){
			return 1;
		}
	}
	for (i=kingCol-1, j=kingRow+1; i>=0 && j<8; i--,j++){
		//if its bishop or queen
		if ( (board[i][j] == pow(-1, col)*4) || (board[i][j] == pow(-1, col)*5) ){
			return 1;
		}
		else if (board[i][j]==0)
			continue;
		else
			break;
	}
	//diagonal downleft
	if (kingCol-1>=0 && kingRow-1>=0){
		//the king is the threatening
		if (board[kingCol-1][kingRow-1] == (pow(-1, col)*6 )){
			return 1;
		}
		//white pawn is the threatening
		if (col==0 && board[kingCol-1][kingRow-1] == (pow(-1, col)*1 )){
			return 1;
		}
	}
	for (i=kingCol-1, j=kingRow-1; i>=0 && j>=0; i--,j--){
		//if its bishop or queen
		if ( (board[i][j] == pow(-1, col)*4) || (board[i][j] == pow(-1, col)*5)  ){
			return 1;
		}
		else if (board[i][j]==0)
			continue;
		else
			break;
	}
	//diagonal downright
	if (kingCol+1<8 && kingRow-1>=0){
		//the king is the threatening
		if (board[kingCol+1][kingRow-1] == (pow(-1, col)*6 )){
			return 1;
		}
		//white pawn is the threatening
		if (col==0 && board[kingCol+1][kingRow-1] == (pow(-1, col)*1 )){
			return 1;
		}
	}
	for (i=kingCol+1, j=kingRow-1; i<8 && j>=0; i++,j--){
		//if its bishop or queen
		if ( (board[i][j] == pow(-1, col)*4) || (board[i][j] == pow(-1, col)*5)  ){
			return 1;
		}
		else if (board[i][j]==0)
			continue;
		else
			break;
	}
	return 0;
}

/*	check if a given color has moves (color is the OPPOSITE of the player that moved)	*/
int isTie(int col){
	for (int i =0 ; i < 8 ; i++){
		for (int j =0 ; j < 8 ; j++){
			//check only places that got the right color piece)
			if ( (board[i][j] > 0 && col == 0) || (board[i][j] < 0 && col == 1) ){
				loc occupiedLoc = createLoc(i,j);
				int type = abs(board[i][j]) -1;
				moveNode* currList = getMoves(occupiedLoc, col, type);
				if (currList != NULL){
					//if a move was found it isn't a tie
					delMoveList(currList);
					return 0;
				}
			}
		}
	}
	return 2;
}

/*end specific validation	*/
/*end move	*/



/*	scoring functions	*/

/*	current piece array scoring function	*/

int scoreA(int col){
	int score = 0;
	for(int i = 0; i < 6; i++){
		score += (pieces[i][col] - pieces[i][1 - col]) * num2score(i + 1);
	}
	return score;
}
/*	advanced scoring function (called when "best" difficulty is defined)	*/
int scoreB(int turn){
	int score = 0;
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board[i][j] > 0){
				score = (turn == 0) ? score + num2scoreB(board[i][j], turn, i, j) : score - num2scoreB(board[i][j], turn, i, j);
			} else if (board[i][j] < 0){
				score = (turn == 0) ? score - num2scoreB(board[i][j], turn, i, j) : score + num2scoreB(board[i][j], turn, i, j);
			}
		}
	}
	score += 2*(diversityDiff(turn));
	return score;
}

/*	calculate the difference of the diversity of pieces between two players (used for socreB function)	*/
int diversityDiff (int turn){
	int whiteDiversity = 0;
	int blackDiversity = 0;
	for(int i = 0; i < 6; i++){
			if (pieces[i][0] != 0) whiteDiversity++;
			if (pieces[i][1] != 0) blackDiversity++;
	}
	return pow(-1, turn) * (whiteDiversity - blackDiversity);
}



/*	extracts the data from a given line in a XML file	*/
int extractData(char* line){
	if(line == NULL){
		return 0;
	}
	if(strstr(line, "<next_turn>") != NULL){
		if(strstr(line, "White") != NULL){
			whosTurn = 0;
			free(line);
			return 1;
		} else {
			whosTurn = 1;
			free(line);
			return 1;
		}
	} else if(strstr(line, "<game_mode>") != NULL){
		if(strstr(line, "1") != NULL){
			gameMode = 1;
			free(line);
			return 1;
		} else {
			gameMode = 2;
			free(line);
			return 1;
		}
	} else if(strstr(line, "<difficulty>") != NULL){
		if(strstr(line, "1") != NULL){
			minimax_depth = 1;
			free(line);
			return 1;
		} else if(strstr(line, "2") != NULL){
			minimax_depth = 2;
			free(line);
			return 1;
		} else if(strstr(line, "3") != NULL){
			minimax_depth = 3;
			free(line);
			return 1;
		} else if(strstr(line, "4") != NULL){
			minimax_depth = 4;
			free(line);
			return 1;
		} else if(strstr(line, "Best") != NULL){
			minimax_depth = -1;
			free(line);
			return 1;
		} else {
			minimax_depth = 1;
			free(line);
			return 1;
		}
	} else if(strstr(line, "<user_color>") != NULL){
		if(strstr(line, "Black") != NULL){
			userColor = 1;
			free(line);
			return 1;
		} else {
			userColor = 0;
			free(line);
			return 1;
		}
	} else if(strstr(line, "<row_8>") != NULL) return loadRow(line, 7);
	else if(strstr(line, "<row_7>") != NULL) return loadRow(line, 6);
	else if(strstr(line, "<row_6>") != NULL) return loadRow(line, 5);
	else if(strstr(line, "<row_5>") != NULL) return loadRow(line, 4);
	else if(strstr(line, "<row_4>") != NULL) return loadRow(line, 3);
	else if(strstr(line, "<row_3>") != NULL) return loadRow(line, 2);
	else if(strstr(line, "<row_2>") != NULL) return loadRow(line, 1);
	else if(strstr(line, "<row_1>") != NULL) return loadRow(line, 0);
	free(line);
	return 1;
}

int loadRow(char* line, int num){
	int start = 0;
	int pos = 0;
	int charNum;
	for(char* p = line; *p != '\0'; p++){
		if(*p == '<'){
			start = 0;
			continue;
		}
		if(*p == '>') {
			start = 1;
			continue;
		}
		if(start == 1 && *p != '<' && pos < 8){
			charNum = char2num(*p);
			board[pos++][num] = charNum;
			if(charNum > 0)	pieces[abs(charNum) - 1][0]++;
			else if (charNum < 0) pieces[abs(charNum) - 1][1]++;
		}
	}
	free(line);
	return 1;
}


int board2file(FILE* ofp) {
	for(int i = BOARD_SIZE; i > 0; i--){
		fprintf(ofp, "\t\t<row_%d>", i);
		for (int j = 0; j < BOARD_SIZE; j++){
			if(board[j][i-1] == 0){
				fputc('_', ofp);
			} else {
				fputc(num2char(board[j][i-1]), ofp);
			}
		}
		fprintf(ofp, "</row_%d>\n", i);
	}
	return 0;
}


void clearPieces(){
	for(int i = 0; i < 6; i++){
		for (int j = 0; j < 2; j++){
			pieces[i][j] = 0;
		}
	}
}

/*	end load/save mechanism	*/



/*	struct processing functions	*/



/*	convert 2 integers to location struct	*/
loc createLoc(int c, int r) {
	loc location;
	location.column = c;
	location.row = r;
	return location;
}

/*	 compare between two locations	*/
int compareLoc(loc loc1, loc loc2) {
	if (loc1.row != loc2.row || loc1.column != loc2.column) {
		return 0;
	}
	return 1;
}

/*	create a move from two location structs	*/
move *createmove(loc from, loc to){
	move* out = (move*)malloc(sizeof(move));
	if (out == NULL){
		perror("Error: standard function malloc has failed");
		quit();
	}
	out->from = from;
	out->target = to;
	return out;
}

/*	update the targetMove fields to be like the originMove ones	*/
int dupMov(move* originMove, move* targetMove){
	if(originMove != NULL && targetMove != NULL){
		targetMove->from.column = originMove->from.column;
		targetMove->from.row = originMove->from.row;

		targetMove->target.column = originMove->target.column;
		targetMove->target.row = originMove->target.row;

		targetMove->value = originMove->value;

		return 1;
	}
	return 0;
}

/*	make new locations and update them to the same values as the originMove	*/
int copyMove(move* originMove, move* targetMove){
	if(originMove != NULL && targetMove != NULL){
		targetMove->from = createLoc(originMove->from.column, originMove->from.row);
		targetMove->target = createLoc(originMove->target.column, originMove->target.row);
		targetMove->value = originMove->value;
		return 1;
	}
	return 0;
}




/*	struct deleting functions	*/

void delMove (move* mov){
	if (mov!=NULL){
		free(mov);
	}
}

void delMoveNodes(moveNode* node){
	if(node != NULL){
		delMoveNodes(node->nextMove);
		free(node->data);
		free(node);
	}
}


/*end struct processing functions	*/


/* board printing functions */

void print_line() {
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++) {
		printf("-");
	}
	printf("|\n");
}

void print_board() {
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--) {
		printf((j < 8 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++) {
			printf("| %c ", num2char(board[i][j]));
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++) {
		printf(" %c  ", (char) ('a' + j));
	}
	printf("\n");
}

/*	update the board to its standard form (all pieces are in place)	*/
void init_board() {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if(j == 7){
				if(i == 0 || i == 7) board[i][j] = -2;
				else if(i == 1 || i == 6) board[i][j] = -3;
				else if(i == 2 || i == 5) board[i][j] = -4;
				else if(i == 3) board[i][j] = -5;
				else if(i == 4) board[i][j] = -6;
			}
			else if(j == 6) board[i][j] = -1;
			else if(j == 1) board[i][j] = 1;
			else if(j == 0){
				if(i == 0 || i == 7) board[i][j] = 2;
				else if(i == 1 || i == 6) board[i][j] = 3;
				else if(i == 2 || i == 5) board[i][j] = 4;
				else if(i == 3) board[i][j] = 5;
				else if(i == 4) board[i][j] = 6;
			} else {
				board[i][j] = 0;
			}
		}
	}
	for(int i = 0; i < 6; i++){
		if(i == 0) pieces[i][0] = pieces[i][1] = 8;
		else if (i < 4) pieces[i][0] = pieces[i][1] = 2;
		else pieces[i][0] = pieces[i][1] = 1;
	}
}


void print_move(move *mov) {
	//print from +to
	char a = '<';
	char b = 'a' + mov->from.column;
	char c = ',';
	int d = mov->from.row + 1;
	char e = '>';
	printf("%c%c%c%d%c to ", a, b, c, d, e);
	//print target
	a = '<';
	b = 'a' + mov->target.column;
	c = ',';
	d = mov->target.row + 1;
	e = '>';
	printf("%c%c%c%d%c", a, b, c, d, e);
	printf("\n");
}
/* end print board functions */



/*	string proccesing functions	*/



/*	converts from a string to value	*/
int piece2num(char* piece){
	if (!strcmp(piece, "pawn") || !strcmp(piece, "Pawn")) return 0;
	else if (!strcmp(piece, "rook") || !strcmp(piece, "Rook")) return 1;
	else if (!strcmp(piece, "knight") || !strcmp(piece, "Knight")) return 2;
	else if (!strcmp(piece, "bishop") || !strcmp(piece, "Bishop")) return 3;
	else if (!strcmp(piece, "queen") || !strcmp(piece, "Queen")) return 4;
	else if (!strcmp(piece, "king") || !strcmp(piece, "King")) return 5;
	return -1;
}

/*	converts from num to char	*/
char num2char(int num){
	if(num == 0) return EMPTY;
	else if(num == 1) return 'm';
	else if(num == -1) return 'M';
	else if(num == 2) return 'r';
	else if(num == -2) return 'R';
	else if(num == 3) return 'n';
	else if(num == -3) return 'N';
	else if(num == 4) return 'b';
	else if(num == -4) return 'B';
	else if(num == 5) return 'q';
	else if(num == -5) return 'Q';
	else if(num == 6) return 'k';
	else if(num == -6) return 'K';
	return 'x';
}

/*	converts from char to num	*/
char char2num(char piece){
	if(piece == '_') return 0;
	else if(piece == 'm') return 1;
	else if(piece == 'M') return -1;
	else if(piece == 'r') return 2;
	else if(piece == 'R') return -2;
	else if(piece == 'n') return 3;
	else if(piece == 'N') return -3;
	else if(piece == 'b') return 4;
	else if(piece == 'B') return -4;
	else if(piece == 'q') return 5;
	else if(piece == 'Q') return -5;
	else if(piece == 'k') return 6;
	else if(piece == 'K') return -6;
	return 10;
}

/*	converts from piece number (resembling a piece) on the board to its score	*/
int num2score (int num){
	if(abs(num) == 1) return 1;
	else if(abs(num) == 2) return 5;
	else if(abs(num) == 3) return 3;
	else if(abs(num) == 4) return 3;
	else if(abs(num) == 5) return 9;
	else if(abs(num) == 6) return 400;
	return 0;
}

/*	converts from piece number (resembling a piece) on the board to its score.
 *  used for "best"	difficulty only
 *  the scoring in this function is according to advanced chess engines
 */
int num2scoreB (int type, int col, int row, int colm){
	int score = 0;
	switch (abs(type)) {
		case (1) :
			score = 10;
			if(col == 0){
				if((colm > 2 || colm < 5) && row > 2){
					score += 3;
				}
				for (int i = row + 1; i < 7; i++){
					if(board[i][colm] == 1){
						score -= 3;
						break;
					}
				}
			} else {
				if((colm > 2 || colm < 5) && row < 5){
					score += 3;
				}
				for (int i = row - 1; i > 0; i--){
					if(board[i][colm] == -1){
						score -= 3;
						break;
					}
				}
			}
			break;
		case (2) :
			score = 50;
			if(col == 0){
				for (int i = row + 1; i < 7; i++){
					if(board[i][colm] == 1){
						score -= 6;
					}
					if(board[i][colm] == 5){
						score += 6;
					}
				}
				if(row == 7) score += 5;
			} else {
				for (int i = row - 1; i > 0; i--){
					if(board[i][colm] == -1){
						score -= 6;
					}
					if(board[i][colm] == -5){
						score += 6;
					}
				}
				if(row == 0) score += 5;
				score -= (int) floor(pieces[0][col] * 0.5);
			}
			break;
		case (3) :
			if(colm == 0 && colm == 7){
				if(row == 0 || row == 7){
					score -= 8;
				} else {
					score -= 5;
				}
			}
			score = 30;
			if( (colm >= 6 && row >= 6) || (colm <= 1 && row <= 1) ) score -= 5;
			if(col == 0){
				if(row > 2 && (colm < 5 && colm> 2)) score += 3;
			} else {
				if(row > 5 && (colm < 5 && colm> 2)) score += 3;
			}
			score += (int) floor(pieces[0][1 - col] * 0.5);
			break;
		case (4) :
			if(board[type - 1][col] == 2) score = 35;
			else score = 30;
			if( (colm >= 6 && row >= 6) || (colm <= 1 && row <= 1) ) score -= 5;
			if(col == 0){
				if(row > 2 && (colm < 5 && colm> 2)) score += 3;
			} else {
				if(row > 5 && (colm < 5 && colm> 2)) score += 3;
			}
			break;
		case (5) :
			score = 90;
			break;
		case (6) :
			score = 4000;
			break;
		default :
			break;
	}
	return score;
}

/*end string proccesing funcrions	*/

