#include "Chess.h"

/* globals */
int minimax_depth = 1;
//0 for white, 1 for black
int userColor = 0;
//0 for white, 1 for black
int whosTurn = 0;
//1 for two players, 2 for AI
int gameMode = 1;
//current board
int board[BOARD_SIZE][BOARD_SIZE];
//pieces on the board summary
int pieces[6][2];
//first Window in GUI: -1 / settings mode: 0 / game mode: 1
int gameState = 0;
//string builder flag
int stringFlag = 0;
//First cmd Input
char* cmdInput = NULL;
//First cmd GUI
char* cmdGUI = NULL;
//AI Move
move* AIMove = NULL;
//end game flag
int endGameFlag = 0;


int main( int argc, char* args[] ){

	if (argc == 2){
		// if the argument is "GUI" run the game on GUI mode
		if ( (strcmp(args[1], "GUI") == 0) || (strcmp(args[1], "gui") == 0) || (strcmp(args[1], "Gui") == 0) ){
			mainGUI();
			return 0;
		}
	}

	mainConsole();
	return 0;
}

int mainGUI(){

	/*	GUI Event*/
	SDL_Event e;
	
	// input from the player to be executed
	cmdInput = (char*) malloc (50*sizeof(char));
	if (cmdInput == NULL){
		perror("Error: standard function malloc has failed");
		quitGUI();
		quit();
		return 0;
	}
	cmdInput[0] = '\0';

	// the input from the GUI to be parsed into a string command
	cmdGUI = (char*) malloc (50*sizeof(char));
	if (cmdGUI == NULL){
		perror("Error: standard function malloc has failed");
		quitGUI();
		quit();
		return 0;
	}
	cmdGUI[0] = '\0';
	
	/*	initiating the game	*/
	init();
	gameState = -1;

	while (strcmp(cmdInput, "quit")){
		/*	game setting area	*/
		if (gameState == -1){
			//Draw the window
			DrawFirstWindow();
			//starting board initialization
			init_board(board);

			//get Event
			while (SDL_PollEvent(&e) != 0) {
				//analyze the GUI event
				analyzeFirst(e);
				strcpy(cmdInput, cmdGUI);
				if(strcmp(cmdInput, "continue") != 0) {
					//execute command in the game
					setFuncs(cmdInput);
				}
			}
		}

		/*Settings area*/
		else if (gameState == 0){
			//Draw
			DrawSecondWindow();
			//get Event
			while (SDL_PollEvent(&e) != 0) {
				if(stringFlag==2 && strcmp(cmdInput, "continue") != 0) {
					//execute command in the game
				   	exeGUI(setFuncs(cmdInput));
					stringFlag=0;
					break;
				}
				if (stringFlag==1){
					//analyze the GUI event
					analyzeSecond(e);
					strcat(cmdInput, cmdGUI);
				}
				else if (!stringFlag){
					//analyze the GUI event
					analyzeSecond(e);
					strcpy(cmdInput, cmdGUI);
				}
			}
		}

		/*game area*/
		else if (gameState == 1){

			//Draw
			DrawThirdWindow();

			if (gameMode == 2 && endGameFlag < 3){
				//computers turn
				if(whosTurn != userColor){
					//get move from the minimax
					AIMove = minimaxRoot(whosTurn, minimax_depth, INT_MIN, INT_MAX, 0);
					//make the given move
					movePiece(AIMove, whosTurn, abs(board[AIMove->from.column][AIMove->from.row]) -1, "queen",1);
					printf("AI Move:\n");
					print_move(AIMove);
					//check for tie mate or check
					switch (CMT(whosTurn)){
						case(0):
							exeGUI(1);
							whosTurn = 1 - whosTurn;
							break;
						case (1) :
							exeGUI(2);
							whosTurn = 1 - whosTurn;
							break;
						case (2) :
							exeGUI(3);
							whosTurn = 1 - whosTurn;
							break;
						case (3) :
							exeGUI(4);
							gameMode = 1;
							break;
					}
					delMove(AIMove);
					AIMove = NULL;
				}
			}

			//Draw
			DrawThirdWindow();

			//get Event
			if (SDL_WaitEvent(&e)) {
				if(stringFlag==2 && strcmp(cmdInput, "continue") != 0) {
					//execute command in the game
					endGameFlag = gameFuncs(cmdInput);
					exeGUI(endGameFlag);
					stringFlag=0;
				}
				if (stringFlag==1){
					//analyze the GUI event
					analyzeThird(e);
					strcat(cmdInput, cmdGUI);
				}
				else if (!stringFlag){
					//analyze the GUI event
					analyzeThird(e);
					strcpy(cmdInput, cmdGUI);
				}
			}
		}
	}
	quitGUI();
	return 0;
}


int mainConsole(){

	/*	initiating the game	*/
	init_board(board);
	print_board(board);
	print_message(WELCOME_TO_CHESS);

	while (endGameFlag != 3 && endGameFlag != 4 ){
		/*game setting mode*/
		if (gameState == 0){
			print_message(ENTER_SETTINGS);
			//get input from the console
			cmdInput = scanInput();
			if (strcmp(cmdInput, "quit") == 0) {
				if(cmdInput != NULL){
					free(cmdInput);
				}
				quit();
			} else {
				//execute command in the game
				setFuncs(cmdInput);
				free(cmdInput);
			}
		}

		/*game play area*/
		if (gameState == 1){
			//player vs player
			if (gameMode == 2){
				if(whosTurn != userColor){
					AIMove = minimaxRoot(whosTurn, minimax_depth, INT_MIN, INT_MAX, 0);
					movePiece(AIMove, whosTurn, abs(board[AIMove->from.column][AIMove->from.row]) -1, "queen",1);
					printf("AI Move:\n");
					print_move(AIMove);
					switch (CMT(whosTurn)){
						case (1) :
							printf ("Check!\n");
							break;
						case (2) :
							printf ("The game ends with a tie\n");
							break;
						case (3) :
							printf ("Mate! %s player wins the game\n", (whosTurn==0) ? "White" : "Black");
							break;
					}
					print_board(board);
					whosTurn = 1 - whosTurn;
					delMove(AIMove);
					AIMove = NULL;
				}
			}
			printf("%s player - enter your move:\n", (whosTurn == 0) ? "White" : "Black");
			//get input from the console
			cmdInput = scanInput();

			if (strcmp(cmdInput, "quit") == 0){
				free(cmdInput);
				quit();
			} else {
				//execute command in the game
				endGameFlag = gameFuncs(cmdInput);
				if(AIMove != NULL){
					delMove(AIMove);
					AIMove = NULL;
				}
				free(cmdInput);
			}
		}
	}
	quit();
	return 0;
}
