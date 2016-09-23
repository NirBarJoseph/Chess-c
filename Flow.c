#include "Chess.h"



/*setting mode functions*/
int setFuncs(char *input) {
	char* cmd = strtok(input, " ");

	/*setting game mode*/
	if (strcmp(cmd, "game_mode") == 0){
		return setGameMode();

	/*setting minimax depth*/
	} else if (strcmp(cmd, "difficulty") == 0) {
		return setDifficulty();

		/*setting user color*/
	} else if (strcmp(cmd, "user_color") == 0) {
		return setUserColor();

		/*load game*/
	} else if (strcmp(cmd, "load") == 0) {
		return setLoad();

		/*clear board*/
	} else if (strcmp(input, "clear") == 0) {
		clear(board);
		printf("Cleared\n");
		return 1;

		/*next paly*/
	} else if(strcmp(input, "next_player") == 0) {
		return setTurn();

		/*remove disc*/
	} else if (strcmp(input, "rm") == 0) {
		return setRm();

		/*set disc*/
	} else if (strcmp(input, "set") == 0) {
		return setSet();

		/*print board*/
	} else if (strcmp(input, "print") == 0) {
		print_board(board);
		return 1;

		/*check legal start*/
	} else if (strcmp(input, "start") == 0){
		if(isLegalStart()){
			gameState = 1;
			return 1;
		}
	}
	return 0;
}

int setGameMode(){
	int num = atoi(strtok(NULL, " "));
	if(num == 1){
		gameMode = num;
		print_message(TWO_PLAYERS_GAME_MODE);
		return 1;
	} else if(num == 2){
		gameMode = num;
		print_message(PLAYER_VS_AI_GAME_MODE);
		return 1;
	}
	print_message(WRONG_GAME_MODE);
	return 0;
}

int setDifficulty(){
	if(gameMode == 1){
		print_message(ILLEGAL_COMMAND);
		return 0;
	} else {
		char* cmd = strtok(NULL, " ");
		if(strcmp(cmd, "best") == 0){
			minimax_depth = -1;
			printf("Best difficulty set\n");
			return 1;
		} else {
			int num = atoi(strtok(NULL, " "));
			if(num <=4 && num >= 1){
				minimax_depth = num;
				printf("Difficulty set\n");
				return 1;
			} else {
				print_message(WRONG_MINIMAX_DEPTH);
				return 0;
			}
		}
	}
}

int setUserColor() {
	if(gameMode == 1){
		print_message(ILLEGAL_COMMAND);
		return 0;
	} else {
		char *color = (strtok(NULL, " "));
		if (strcmp(color, "white") == 0) userColor = 0;
		else userColor = 1;
		printf("User color was set to %s\n", color);
		return 1;
	}
}

int setLoad() {
	char* path = (strtok(NULL, " "));
	FILE* ifp = NULL;
	ifp = fopen(path, "r");
	if(ifp == NULL) {
		print_message(WRONG_FILE_NAME);
		return 0;
	}
	clearPieces();
	load(ifp);
	fclose(ifp);
	print_board(board);
	return 1;
}

int setTurn() {
	char *color = (strtok(NULL, " "));
	if (strcmp(color, "white") == 0) whosTurn = 0;
	else whosTurn = 1;
	printf("Next player is %s", color);
	return 1;
}

int setRm(){
	char *location = (strtok(NULL, " "));
	loc pos = processLoc(location);
	if(rm(pos.column, pos.row, 1) == 1) {
		printf("Removed\n");
		return 1;
	}
	else {
		return 0;
	}
}

int setSet() {
	char *location = (strtok(NULL, " "));
	loc pos = processLoc(location);
	char *color = (strtok(NULL, " "));
	int col = (strcmp(color, "white") == 0) ? 0 : 1;
	int type;
	if((type = piece2num(strtok(NULL, " "))) == -1) {
		print_message("Wrong Input");
		return 0;
	}
	int setSucc = set(pos.column, pos.row, col, type, 1);
	if (setSucc==1) {
		printf("Requested disc has been set\n");
		return 1;
	}
	return 0;
}



/*game mode functions*/
int gameFuncs(char *input) {
	char* cmd = strtok(input, " ");

	if (strcmp(cmd, "move") == 0) {
		return gameMove();
	} else if (strcmp(cmd, "get_moves") == 0) {
		return gameGetMoves();

	}else if (strcmp(cmd, "get_best_moves") == 0) {
		return gameGetBestMoves();

	} else if (strcmp(cmd, "get_score") == 0) {
		return gameGetScore();

	} else if (strcmp(cmd, "save") == 0) {
		return gameSave();

	} else if (strcmp(input, "main") == 0){
		clear(board);
		init_board(board);
		gameState = 0;
		return 1;
	}
	printf("Please try again!\n");
	return 0;
}

/*	execute a given move	*/
int gameMove() {
	char *from = (strtok(NULL, " "));
	strtok(NULL, " "); //skip the "to" word
	char *target = (strtok(NULL, " "));
	move *mov = processMove(from, target);
	char* prom = (strtok(NULL, " ")); //the pawn promotion optional parameter
	if (prom == NULL)
		prom = "Queen";
	int type = abs(board [mov->from.column][mov->from.row]) - 1;
	int moveSucsses = movePiece (mov, whosTurn, type, prom,1);
	delMove (mov);
	if (moveSucsses == 1){
		print_board(board);
		//check if the move leads to check/tie/mate
		switch (CMT(whosTurn)){
		case (1) :
			printf ("Check!\n");
			whosTurn = 1 - whosTurn;
			return 2;
		case (2) :
			printf ("The game ends in a tie\n");
			whosTurn = 1 - whosTurn;
			return 3;
		case (3) :
			printf ("Mate! %s player wins the game\n", (whosTurn==0) ? "White" : "Black");
			return 4;
		}
		whosTurn = 1 - whosTurn;
		return 1;
	}
	return 0;
}

/*	get the possible moves for a given location	*/
int gameGetMoves() {
	loc pos = processLoc(strtok(NULL, " "));
	if (pos.column > 7 || pos.row > 7 || pos.column < 0 || pos.row < 0){
			print_message(WRONG_POSITION);
			return 0;
	}else if (board[pos.column][pos.row]==0) {
		print_message("The specified position does not contain your piece\n");
		return 0;
	} else if ( (whosTurn==0 && board[pos.column][pos.row]<0) ||
			(whosTurn==1 && board[pos.column][pos.row]>0) ) {
		printf("The specified position does not contain your piece\n");
		return 0;
	}else {
		int type = abs(board [pos.column][pos.row]) - 1;
		moveNode* movList = getMoves(pos, whosTurn, type);
		if(movList != NULL){
			print_moveList(movList);
			delMoveList(movList);
		}
	}
	return 1;
}

/* get the best moves for a given depth	*/
int gameGetBestMoves() {
	move* aMove = NULL;
	char* num = strtok(NULL, " ");
	int depth;
	if(strcmp(num, "best") == 0){
		depth = -1;
	} else {
		depth = atoi(num);
	}
	//call the minimax algorithm to achive the best move
	aMove = minimaxRoot(whosTurn, depth, INT_MIN, INT_MAX, 1);
	delMove(aMove);
	return 1;
}

/*	get the score for a given depth and move	*/
int gameGetScore() {
	char* num = strtok(NULL, " ");
	int depth;
	if(strcmp(num, "best") == 0){
		depth = -1;
	} else {
		depth = atoi(num);
	}
	//skip "move" word
	strtok(NULL, " ");
	char *from = (strtok(NULL, " "));
	//skip "to" word
	strtok(NULL, " ");
	char *target = (strtok(NULL, " "));
	move *mov = processMove(from, target);
	 //the pawn promotion optional parameter
	char* prom = (strtok(NULL, " "));
	if (prom == NULL) {
		prom = "Queen";
	}
	mov = minimax(mov, INT_MIN, INT_MAX, whosTurn, depth - 1, 0, prom);
	printf("%d\n", mov->value);
	delMove(mov);
	return 1;
}

/*	save the current game	*/
int gameSave() {
	char* path = strtok(NULL, " ");
	FILE* ofp = NULL;
	ofp = fopen(path, "w");
	if(ofp == NULL) {
		print_message(WRONG_FILE_NAME);
		return 0;
	}
	save(ofp);
	fclose(ofp);
	return 1;
}

