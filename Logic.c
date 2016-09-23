
#include "Chess.h"

/*	scan the players input from the command line until \n character is encountered	*/
char* scanInput() {
	unsigned int len_max = 50;
	unsigned int current_size = 0;
	//initial length allocation
	char *pStr = (char *) malloc(len_max);
	if (pStr == NULL){
		perror("Error: standard function malloc has failed");
		quit();
	}
	current_size = len_max;
	setbuf(stdout, NULL);
	if (pStr != NULL) {
		int c = EOF;
		unsigned int i = 0;
		//accept user input until hit enter or end of file
		while ((c = getchar()) != '\n' && c != EOF) {
			pStr[i++] = (char) c;
			//if i reached maximum size- realloc size
			if (i == current_size) {
				current_size = i + len_max;
				pStr = (char*) realloc(pStr, current_size);
				if (pStr == NULL) {
					perror("Error: standard function realloc has failed");
					quit();
				}
			}
		}
		pStr[i] = '\0';
	} else {
		perror("Error: standard function malloc has failed");
		quit();
	}
	return pStr;
}


/*	move functions	*/

/*	executing the a move on the board	*/
int movePiece (move* mov, int col, int type, char* prom, int checkValid){
	//checking the move is valid
	if(checkValid){
		if (!isValid(*mov, col, type, 1)) return 0;
	}

	//move the piece
	/*	remove "from" disc and possibly eaten enemy pieces 	*/
	rm(mov->from.column, mov->from.row, 0);
	int toType = abs(board[mov->target.column][mov->target.row]) - 1;
	if (toType >=0){
		// update the pieces array if toType is piece
		rm(mov->target.column, mov->target.row, 0);
	}
	//set the piece in target
	loc newPos = createLoc(mov->target.column , mov->target.row);
	//check if it's a promoted pawn
	if (type == 0 && mov->target.row == 7*(1-col)) {
		type = piece2num(prom);
	}
	set(newPos.column, newPos.row, col , type, 0);
	return 1;
}


/* checks for a mate / tie / check
 * 0 for nothing , 1 for check, 2 for tie, 3 for mate (mate = tie + check)
 */
int CMT(int col){
	return (isCheck(col)) + (isTie(1-col));
}

/* creates a linked list of all possible moves for a given color from a specific location	*/
moveNode* getMoves(loc pos, int col, int type){
	int flag = 0;
	moveNode *head = NULL, *tail = NULL;
	move* mov = (move*)malloc(sizeof(move));
	if(mov == NULL){
		perror("Error: standard function malloc has failed");
		quit();
	}
	mov->from = pos;
	//go through all the board
	for(int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			// create a "target" and check if it's a
			// legal move only if the target position is not
			// occupied by same color piece
			if( (board[i][j] <= 0 && col == 0) || (board[i][j] >= 0 && col == 1) ){
				// create the target
				mov->target = createLoc(i, j);
				// create / append list only if the move is valid
				if(isValid(*mov, col, type, 0)){
					if(!flag){
						// if flag is false create the list by allocating
						// space for the head
						head = (moveNode*)malloc(sizeof(moveNode));
						if(head == NULL) {
							perror("Error: standard function malloc has failed");
							quit();
						}
						tail = head;
						flag++;
					} else {
						// if flag is true make tail pointer to point
						// at the nextMove field of the last node in
						// the list and allocate space for it
						tail->nextMove = (moveNode*)malloc(sizeof(moveNode));
						if(tail->nextMove == NULL){
							perror("Error: standard function malloc has failed");
							delMoveList(head);
							quit();
						}
						tail = tail->nextMove;
					}
					// allocate space for the node date
					tail->data = (move*)malloc(sizeof(move));
					if(tail->data == NULL) {
						perror("Error: standard function malloc has failed");
						quit();
					}
					// create a "from" and "target" location structs.
					// assign them to the node
					tail->data->from = createLoc(pos.column, pos.row);
					tail->data->target = createLoc(i, j);
					// seal the list
					tail->nextMove = NULL;
				}
			}
		}
	}
	free(mov);
	// make sure the list is sealed
	if(head != NULL) tail->nextMove = NULL;
	return head;
}

/*	end move functions	*/



/*	load / save functions	*/

int load(FILE* ifp){

	//reset board and pieces variables
	for(int i = 0; i < 6; i++){
		pieces[i][0] = 0;
		pieces[i][1] = 0;
	}
	clear();

	char c;
	char* line = NULL;
	while(ifp != NULL){
		int size = 0;
		line = (char*)malloc((size + 1)*sizeof(char));
		line[0] = '\n';
		if(line == NULL) {
			perror("Error: standard function malloc has failed");
			quit();
		}
		while((c = fgetc(ifp)) != '\n'){
			if(c == EOF) return 0;
			line[size++] = c;
			line = (char*)realloc(line, (size+1)*sizeof(char));
			if(line == NULL){
				perror("Error: standard function realloc has failed");
				return 0;
			}
			line[size] = '\0';
			if(strcmp(line, "</game>") == 0){
				free(line);
				return 1;
			}
		}
		line[size] = '\n';
		if(!extractData(line)) return 0;
	}
	return 1;
}

int save(FILE* ofp){
	char* colour = (whosTurn == 0) ? "White" : "Black";
	fprintf(ofp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<game>\n"
			"\t<next_turn>%s</next_turn>\n"
			"\t<game_mode>%d</game_mode>\n", colour, gameMode);
	if(gameMode == 1){
		fprintf(ofp, "\t<difficulty></difficulty>\n\t<user_color></user_color>\n\t<board>");
	} else {
		char diff[4];
		char* uColour;
		if(userColor == 0){
			uColour = "White";
		} else {
			uColour = "Black";
		}
		if(minimax_depth == -1){
			strcpy(diff, "best");
		} else {
			if(sprintf(diff, "%d", minimax_depth) < 0){
				perror("Error: standard function sprintf has failed");
			}
		}
		fprintf(ofp, "\t<difficulty>%s</difficulty>\n\t<user_color>%s</user_color>\n\t<board>\n",
				diff, uColour);
	}
	board2file(ofp);
	fprintf(ofp, "\t</board>\n</game>");

	return 0;
}



/*	board functions	*/

/*	clear the board	*/
void clear() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = 0;
			if(i<2 && j<6){
				pieces[j][i] = 0;
			}
		}
	}
}

/*	remove a piece from a given board	*/
int rm(int c, int r, int flag) {
	/*	if the input is invalid	*/
	if ( (r > 7 || c > 7 || r < 0 || c < 0 || board[c][r] == 0) && flag ) {
		print_message(WRONG_POSITION);
		return 0;
	}
		//piece count update
	if(board[c][r] == 1) pieces[0][0]--;
	else if(board[c][r] == -1) pieces[0][1]--;
	else if(board[c][r] == 2) pieces[1][0]--;
	else if(board[c][r] == -2) pieces[1][1]--;
	else if(board[c][r] == 3) pieces[2][0]--;
	else if(board[c][r] == -3) pieces[2][1]--;
	else if(board[c][r] == 4) pieces[3][0]--;
	else if(board[c][r] == -4) pieces[3][1]--;
	else if(board[c][r] == 5) pieces[4][0]--;
	else if(board[c][r] == -5) pieces[4][1]--;
	else if(board[c][r] == 6) pieces[5][0]--;
	else if(board[c][r] == -6) pieces[5][1]--;
	board[c][r] = 0;
	return 1;
}

/*	set a given piece on a given board	*/
int set(int column, int row, int col, int type, int flag) {
	/*	if the input is invalid	*/
	if (row > 7 || column > 7 || row < 0 || column < 0) {
		print_message(WRONG_POSITION)
		return 0;
	}
	/*	if the input is invalid	*/
	if (col != 0 && col != 1) {
		print_message("Wrong Input");
		return 0;
	}
	// if called from set mode a flag should be raised
	if (flag){
		/* limit check */
		if(!checkSetLimit(col, type)){
			printf("Setting this piece creates an invalid board\n" );
			return 0;
		}
	}
	if(type >= 0){
		pieces[type][col]++;
	}
	/*	set the disc	*/
	board[column][row] = (col == 0) ? (type+1) : (-1)*(type+1);
	return 1;
}

/*	end board functions	*/



/*	other	*/

/*	create move from a given string (same string)	*/
loc processLoc(char *location) {
	loc output = {atoi((location + 3)) - 1, *(location + 1) - 'a'};
	return output;
}

/*	creating a move from two given strings (separately)	*/
move* processMove(char *from, char *target) {
	move *out = (move*) malloc(sizeof(move));
	if (out == NULL) {
		perror("Error: standard function malloc has failed");
		exit(0);
	}
	out->from = processLoc(from);
	out->target = processLoc(target);
	return out;
}

/*	print a whole list of moves	*/
void print_moveList(moveNode *head) {
	moveNode *i = head;
	moveNode *tmp = head;
	while (i != NULL) {
		print_move(i->data);
		tmp = i->nextMove;
		i = tmp;
	}
}

/*	delete a whole move list and the data it holds	*/
void delMoveList(moveNode *node) {
	if (node != NULL) {
		delMoveList(node->nextMove);
		delMove(node->data);
		free(node);
	}
}
