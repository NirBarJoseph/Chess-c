#ifndef CHESS_
#define CHESS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "SDL/SDL.h"
#include "SDL/SDL_video.h"

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'




#define EMPTY ' '

#define BOARD_SIZE 8

// board initialization
#define WELCOME_TO_CHESS "Welcome to Chess!\n"
#define ENTER_SETTINGS "Enter game settings:\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n"
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"

#define TIE "The game ends in a tie\n"

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));




//struct declaration

typedef struct loc{
	int row;
	int column;
}loc;

typedef struct move{
	loc from;
	loc target;
	int value;
}move;

typedef struct moveNode{
	move* data;
	struct moveNode* nextMove;
}moveNode;

/* globals */
extern int minimax_depth;
//0 for white, 1 for black
extern int userColor;
//0 for white, 1 for black
extern int whosTurn;
//1 for two players, 2 for AI
extern int gameMode;
//current board
extern int board[BOARD_SIZE][BOARD_SIZE];
//pieces on the board summery
extern int pieces[6][2];
//-1 for main / 0 for set mode / 1 for game mode
extern int gameState;
//white rook moved flag
extern int WRM;
//white king moved flag
extern int WKM;
//black rook moved flag
extern int BRM;
//black king moved flag
extern int BKM;
//string builder flag
extern int stringFlag;
/*	First cmd GUI*/
extern char* cmdGUI;
/*	AI Move*/
extern move* AIMove;



/* Function declaration	*/

/* main functions	*/
int mainConsole();
int mainGUI();


/*	main initiating	*/
void init_board();
void clear();
char* scanInput();


/*	flow functions	*/
int setFuncs(char *input);
int setGameMode();
int setDifficulty();
int setUserColor();
int setLoad();
int setTurn();
int setRm();
int setSet();

int gameFuncs(char *input);
int gameMove();
int gameGetMoves();
int gameGetBestMoves();
int gameGetScore();
int gameSave();

void quit();
int isLegalStart();
int load(FILE* ifp);
int save(FILE* ifp);

//piece and board
int rm(int c,int r, int flag);
int set(int column, int row, int col, int type, int flag);
int checkSetLimit(int col, int piece);


/*	logic functions	*/

//move functions
int movePiece (move *mov, int col, int type, char *prom, int checkValid);
int isValid(move mov, int col, int type, int printFlag);

//get moves functions
moveNode* getMoves(loc pos, int col, int type);
moveNode* getBestMoves(int depth, int col, int type);



/*	utility functions	*/
int CMT(int col);
int isCheck(int col);
int isOwnCheck(move mov, int col, int type);
int isTie(int col);
loc wheresOppKing (int col);

int isValidPawn(move mov, int col);
int isValidRook(move mov);
int isValidKnight(move mov);
int isValidBishop(move mov);
int isValidQueen(move mov);
int isValidKing(move mov);


int extractData(char* line);
int loadRow(char* line, int num);
void clearPieces();
int board2file(FILE* ofp);

//printing functions
void print_board();
void print_line();
void print_move(move *mov);
void print_moveList(moveNode *head);


/*	minimax functions	*/
move* minimaxRoot (int col, int depth, int alpha, int beta, int printingBestMoves);
move* minimax(move* node,int alpha, int beta, int col, int depth, int maxi, char* prom);
int scoreB(int turn);
int scoreA(int col);
int diversityDiff (int turn);


/*	struct functions / proccessing functions	*/

//creating
loc processLoc(char *location);
loc createLoc(int c, int r);
move *processMove(char *from, char *target);
move *createmove(loc from, loc to);

//copying
int dupMov(move* from, move* to);
int copyMove(move* from, move* to);

//deleting
void delMove (move* mov);
void delMoveList(moveNode *mov);
void delMoveNodes(moveNode* node);

//comparing
int compareLoc(loc loc1, loc loc2);

//transforming
char num2char(int num);
char char2num(char piece);
int piece2num(char* piece);
int num2score (int num);
int num2scoreB (int type, int col, int row, int colm);

/*	GUI	*/
int init();
void quitGUI();
void DrawFirstWindow();
void DrawSecondWindow();
void DrawThirdWindow();

void analyzeFirst(SDL_Event e);
void analyzeSecond(SDL_Event e);
void analyzeThird(SDL_Event e);
void exeGUI();

int isOnButton (SDL_Event click, SDL_Rect Butt);

void boardGUI(int slotColor, int column, int row);
void piecesGUI(int col, int column, int row);
void zeroFlags ();

#endif
