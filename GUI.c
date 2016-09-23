#include "Chess.h"

#define WIN_W 640
#define WIN_H 480
#define startButtsW 140
#define startButtsH 30
#define loadButtsW 60
#define loadButtsH 30
#define settingsBtnW 100
#define settingsBtnH 30

#define settingsOptX 390
#define settingsOptBtnW 125
#define settingsOptBtnH 30

#define gameBtnX 420
#define gameBtnY 200
#define gameBtnW 140
#define gameBtnH 30

#define boardOffset 20
#define slotSize 45
#define pieceSize 35

SDL_Surface *window;
int firstWindowLoadFlag = 0;

//second window flags
int nextFlag = 0;
int nextColFlag = 0;
int gameModeFlag = 0;
int usrColFlag = 0;
int diffFlag = 0;
int removeFlag = 0;
int setFlag = 0;


//third window flags
int saveFlag = 0;
int pickedCol = -1;
int pickedRow = -1;
int pickedColTo = -1;
int pickedRowTo = -1;

int pawnFlag = 0;
int promFlag = 0;

int bestMoveFlag = 0;
int checkFlag = 0;
int tieFlag = 0;
int mateFlag = 0;

SDL_Event prevEvent;

SDL_Rect GUIBoard[8][8];
SDL_Rect GUIPieces[8][8];
SDL_Rect setPieces[2][6];
SDL_Rect promPieces[4];
SDL_Rect bestMovesDepths[5];
SDL_Rect wholeWindow = { 0, 0, WIN_W, WIN_H };
SDL_Surface *boardBGCol;
SDL_Surface *boardBGCol2;
SDL_Surface *errorWindow;

/*	first menu	declarations	*/
SDL_Rect welcomeBtn = { WIN_W / 2 - 70, 50, startButtsW, startButtsH };
SDL_Rect newBtn = { 90, 175, startButtsW, startButtsH };
SDL_Rect loadBtn = { 250, 175, startButtsW,	startButtsH };
SDL_Rect quitBtn = { 410, 175, startButtsW,	startButtsH };
SDL_Surface *menuBGcol;
SDL_Surface *welcomeImg;
SDL_Surface *newImg;
SDL_Surface *loadImg;
SDL_Surface *quitImg;

//load buttons
SDL_Rect firstLoad = { 40, 325, loadButtsW, loadButtsH };
SDL_Rect secondLoad = { 60 + loadButtsW, 325, loadButtsW, loadButtsH };
SDL_Rect thirdLoad = { 80 + 2 * loadButtsW, 325, loadButtsW, loadButtsH };
SDL_Rect fourthLoad = { 100 + 3 * loadButtsW, 325, loadButtsW, loadButtsH };
SDL_Rect fifthLoad = { 120 + 4 * loadButtsW, 325, loadButtsW, loadButtsH };
SDL_Rect sixthLoad = { 140 + 5 * loadButtsW, 325, loadButtsW, loadButtsH };
SDL_Rect seventhLoad = { 160 + 6 * loadButtsW, 325, loadButtsW, loadButtsH };

/*	second menu	declarations	*/

SDL_Rect settingsHeader = { WIN_W / 2 - 50, 5, startButtsW, startButtsH };

SDL_Rect gameModeBtn = { WIN_W - 10 - settingsBtnW, 50, settingsBtnW,
		settingsBtnH };
SDL_Rect clearBtn = { WIN_W - 10 - settingsBtnW, 70 + settingsBtnH,
		settingsBtnW, settingsBtnH };
SDL_Rect nextPlayerBtn = { WIN_W - 10 - settingsBtnW, 90 + 2 * settingsBtnH,
		settingsBtnW, settingsBtnH };
SDL_Rect removeBtn = { WIN_W - 10 - settingsBtnW, 110 + 3 * settingsBtnH,
		settingsBtnW, settingsBtnH };
SDL_Rect setBtn = { WIN_W - 10 - settingsBtnW, 130 + 4 * settingsBtnH,
		settingsBtnW, settingsBtnH };
SDL_Rect diffBtn = { WIN_W - 10 - settingsBtnW, 150 + 5 * settingsBtnH,
		settingsBtnW, settingsBtnH };
SDL_Rect usrColBtn = { WIN_W - 10 - settingsBtnW, 170 + 6 * settingsBtnH,
		settingsBtnW, settingsBtnH };

SDL_Rect backBtn = { 10, WIN_H - settingsBtnH - 10, settingsBtnW, settingsBtnH };
SDL_Rect startBtn = { WIN_W - settingsBtnW - 10, WIN_H - settingsBtnH - 10,
		settingsBtnW, settingsBtnH };

SDL_Rect pVpBtn = { settingsOptX, WIN_H / 4, settingsOptBtnW, settingsOptBtnH };
SDL_Rect pVaBtn = { settingsOptX, WIN_H / 4 + settingsOptBtnH + 20,
		settingsOptBtnW, settingsOptBtnH };

SDL_Rect click2changeBtn = { settingsOptX, WIN_H / 4, settingsOptBtnW,
		settingsOptBtnH };
SDL_Rect nextColBtn = { settingsOptX, WIN_H / 4 + settingsOptBtnH + 20,
		settingsOptBtnW, settingsOptBtnH };

SDL_Rect depth1Btn =
		{ settingsOptX, WIN_H / 4, settingsOptBtnW, settingsOptBtnH };
SDL_Rect depth2Btn = { settingsOptX, WIN_H / 4 + settingsOptBtnH + 20,
		settingsOptBtnW, settingsOptBtnH };
SDL_Rect depth3Btn = { settingsOptX, WIN_H / 4 + 2 * settingsOptBtnH + 40,
		settingsOptBtnW, settingsOptBtnH };
SDL_Rect depth4Btn = { settingsOptX, WIN_H / 4 + 3 * settingsOptBtnH + 60,
		settingsOptBtnW, settingsOptBtnH };
SDL_Rect depthBestBtn = { settingsOptX, WIN_H / 4 + 4 * settingsOptBtnH + 80,
		settingsOptBtnW, settingsOptBtnH };

SDL_Rect slotFirstBtn = { settingsOptX, WIN_H / 4 - 40, settingsOptBtnW,
		settingsOptBtnH };

/*	third menu	declarations	*/
SDL_Rect gameHeader = { WIN_W / 2 - 50, 5, startButtsW, startButtsH };
SDL_Rect bestMoveBtn = { gameBtnX, gameBtnY + 25, gameBtnW, gameBtnH };
SDL_Rect saveBtn = { gameBtnX, gameBtnY + 70, gameBtnW, gameBtnH };
SDL_Rect mainMenu = { gameBtnX, gameBtnY + 115, gameBtnW, gameBtnH };
SDL_Rect quit2Btn = { gameBtnX, gameBtnY + gameBtnH + 130, gameBtnW, gameBtnH };
SDL_Rect currPlayerBtn = { WIN_W - 170 - 10, 20, 170, 30 };
SDL_Rect specialStatusBtn = { WIN_W - 170 - 10, 70, 170, 30 };

//save Btns
SDL_Rect firstSave = { boardOffset, WIN_H - settingsBtnH - 10, loadButtsW,
		loadButtsH };
SDL_Rect secondSave = { boardOffset + loadButtsW + 20, WIN_H - settingsBtnH
		- 10, loadButtsW, loadButtsH };
SDL_Rect thirdSave = { boardOffset + 2 * (loadButtsW + 20), WIN_H - settingsBtnH
		- 10, loadButtsW, loadButtsH };
SDL_Rect fourthSave = { boardOffset + 3 * (loadButtsW + 20), WIN_H
		- settingsBtnH - 10, loadButtsW, loadButtsH };
SDL_Rect fifthSave = { boardOffset + 4 * (loadButtsW + 20), WIN_H - settingsBtnH
		- 10, loadButtsW, loadButtsH };
SDL_Rect sixthSave = { boardOffset + 5 * (loadButtsW + 20), WIN_H - settingsBtnH
		- 10, loadButtsW, loadButtsH };
SDL_Rect seventhSave = { boardOffset + 6 * (loadButtsW + 20), WIN_H
		- settingsBtnH - 10, loadButtsW, loadButtsH };

/*	Surface decelerations	*/
SDL_Surface *firstImg;
SDL_Surface *secondImg;
SDL_Surface *thirdImg;
SDL_Surface *fourthImg;
SDL_Surface *fifthImg;
SDL_Surface *sixthImg;
SDL_Surface *seventhImg;

SDL_Surface *settingHeaderImg;
SDL_Surface *whiteSlot;
SDL_Surface *whiteSlotChecked;
SDL_Surface *blackSlot;
SDL_Surface *blackSlotChecked;
SDL_Surface *whiteSlotUp;
SDL_Surface *blackSlotUp;
SDL_Surface *whiteSlotDown;
SDL_Surface *blackSlotDown;
SDL_Surface *whiteSlotRight;
SDL_Surface *blackSlotRight;
SDL_Surface *whiteSlotLeft;
SDL_Surface *blackSlotLeft;
SDL_Surface *whiteSlotLeftUp;
SDL_Surface *blackSlotRightUp;
SDL_Surface *whiteSlotRightDown;
SDL_Surface *blackSlotLeftDown;

SDL_Surface *blackRook;

SDL_Surface *gameModeImg;
SDL_Surface *diffImg;
SDL_Surface *usrColImg;
SDL_Surface *clearImg;
SDL_Surface *nextPlayerImg;
SDL_Surface *removeImg;
SDL_Surface *removeEnlight;
SDL_Surface *setImg;
SDL_Surface *backImg;
SDL_Surface *startImg;
SDL_Surface *pVpImg;
SDL_Surface *pVaImg;
SDL_Surface *click2changeImg;
SDL_Surface *nextBlackImg;
SDL_Surface *nextWhiteImg;
SDL_Surface *depth1Img;
SDL_Surface *depth2Img;
SDL_Surface *depth3Img;
SDL_Surface *depth4Img;
SDL_Surface *depthBestImg;
SDL_Surface *slotFirstImg;

SDL_Surface *gameHeaderImg;
SDL_Surface *bestMoveImg;
SDL_Surface *saveImg;
SDL_Surface *mainMenuImg;
SDL_Surface *quit2Img;
SDL_Surface *currWhiteImg;
SDL_Surface *currBlackImg;
SDL_Surface *CheckImg;
SDL_Surface *TieImg;
SDL_Surface *MateImg;

SDL_Surface *depth1MoveImg;
SDL_Surface *depth2MoveImg;
SDL_Surface *depth3MoveImg;
SDL_Surface *depth4MoveImg;
SDL_Surface *depthBMoveImg;


//set pieces
SDL_Surface *whitePawnSet;
SDL_Surface *blackPawnSet;
SDL_Surface *whiteRookSet;
SDL_Surface *blackRookSet;
SDL_Surface *whiteKnightSet;
SDL_Surface *blackKnightSet;
SDL_Surface *whiteBishopSet;
SDL_Surface *blackBishopSet;
SDL_Surface *whiteQueenSet;
SDL_Surface *blackQueenSet;
SDL_Surface *whiteKingSet;
SDL_Surface *blackKingSet;

//board pieces
SDL_Surface *whitePawnWhite;
SDL_Surface *whitePawnBlack;
SDL_Surface *blackPawnWhite;
SDL_Surface *blackPawnBlack;
SDL_Surface *whiteRookWhite;
SDL_Surface *whiteRookBlack;
SDL_Surface *blackRookWhite;
SDL_Surface *blackRookBlack;
SDL_Surface *whiteKnightWhite;
SDL_Surface *whiteKnightBlack;
SDL_Surface *blackKnightWhite;
SDL_Surface *blackKnightBlack;
SDL_Surface *whiteBishopWhite;
SDL_Surface *whiteBishopBlack;
SDL_Surface *blackBishopWhite;
SDL_Surface *blackBishopBlack;
SDL_Surface *whiteQueenWhite;
SDL_Surface *whiteQueenBlack;
SDL_Surface *blackQueenWhite;
SDL_Surface *blackQueenBlack;
SDL_Surface *whiteKingWhite;
SDL_Surface *whiteKingBlack;
SDL_Surface *blackKingWhite;
SDL_Surface *blackKingBlack;


/*	main functions	*/

/*	initialize the main window	*/
int init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}
	atexit(SDL_Quit);

	window = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Chess", "Chess");

	welcomeImg = SDL_LoadBMP("chessBmps/welcome.bmp");

	newImg = SDL_LoadBMP("chessBmps/newGame.bmp");
	loadImg = SDL_LoadBMP("chessBmps/loadGame.bmp");
	quitImg = SDL_LoadBMP("chessBmps/quit.bmp");

	firstImg = SDL_LoadBMP("chessBmps/chess1.bmp");
	secondImg = SDL_LoadBMP("chessBmps/chess2.bmp");
	thirdImg = SDL_LoadBMP("chessBmps/chess3.bmp");
	fourthImg = SDL_LoadBMP("chessBmps/chess4.bmp");
	fifthImg = SDL_LoadBMP("chessBmps/chess5.bmp");
	sixthImg = SDL_LoadBMP("chessBmps/chess6.bmp");
	seventhImg = SDL_LoadBMP("chessBmps/chess7.bmp");

	settingHeaderImg = SDL_LoadBMP("chessBmps/settingsHeader.bmp");

	boardBGCol = SDL_LoadBMP("chessBmps/wood.bmp");
	menuBGcol = SDL_LoadBMP("chessBmps/mainMenu.bmp");
	boardBGCol2 = SDL_LoadBMP("chessBmps/wood2.bmp");
	errorWindow = SDL_LoadBMP("chessBmps/errorWindow.bmp");

	whiteSlot = SDL_LoadBMP("chessBmps/board/whiteSquare.bmp");
	whiteSlotChecked = SDL_LoadBMP("chessBmps/board/whiteSquare - check45.bmp");
	blackSlot = SDL_LoadBMP("chessBmps/board/blackSquare.bmp");
	blackSlotChecked = SDL_LoadBMP("chessBmps/board/blackSquare - check45.bmp");
	whiteSlotUp = SDL_LoadBMP("chessBmps/board/whiteSquare - up.bmp");
	blackSlotUp = SDL_LoadBMP("chessBmps/board/blackSquare - up.bmp");
	whiteSlotDown = SDL_LoadBMP("chessBmps/board/whiteSquare - down.bmp");
	blackSlotDown = SDL_LoadBMP("chessBmps/board/blackSquare - down.bmp");
	whiteSlotRight = SDL_LoadBMP("chessBmps/board/whiteSquare - right.bmp");
	blackSlotRight = SDL_LoadBMP("chessBmps/board/blackSquare - right.bmp");
	whiteSlotLeft = SDL_LoadBMP("chessBmps/board/whiteSquare - left.bmp");
	blackSlotLeft = SDL_LoadBMP("chessBmps/board/blackSquare - left.bmp");
	whiteSlotLeftUp = SDL_LoadBMP(
			"chessBmps/board/whiteSquare - left up.bmp");
	blackSlotRightUp = SDL_LoadBMP(
			"chessBmps/board/blackSquare - right up.bmp");
	whiteSlotRightDown = SDL_LoadBMP(
			"chessBmps/board/whiteSquare - right down.bmp");
	blackSlotLeftDown = SDL_LoadBMP(
			"chessBmps/board/blackSquare - left down.bmp");

	gameModeImg = SDL_LoadBMP("chessBmps/gameMode.bmp");
	diffImg = SDL_LoadBMP("chessBmps/difficulty.bmp");
	usrColImg = SDL_LoadBMP("chessBmps/userColor.bmp");
	clearImg = SDL_LoadBMP("chessBmps/clear.bmp");
	nextPlayerImg = SDL_LoadBMP("chessBmps/nextPlayer.bmp");
	removeImg = SDL_LoadBMP("chessBmps/remove.bmp");
	removeEnlight = SDL_LoadBMP("chessBmps/removeEnlight.bmp");
	setImg = SDL_LoadBMP("chessBmps/set.bmp");
	backImg = SDL_LoadBMP("chessBmps/back.bmp");
	startImg = SDL_LoadBMP("chessBmps/start.bmp");
	pVpImg = SDL_LoadBMP("chessBmps/pVp.bmp");
	pVaImg = SDL_LoadBMP("chessBmps/pVAI.bmp");
	click2changeImg = SDL_LoadBMP("chessBmps/click2change.bmp");
	nextWhiteImg = SDL_LoadBMP("chessBmps/nextWhite.bmp");
	nextBlackImg = SDL_LoadBMP("chessBmps/nextBlack.bmp");
	depth1Img = SDL_LoadBMP("chessBmps/depth1.bmp");
	depth2Img = SDL_LoadBMP("chessBmps/depth2.bmp");
	depth3Img = SDL_LoadBMP("chessBmps/depth3.bmp");
	depth4Img = SDL_LoadBMP("chessBmps/depth4.bmp");
	depthBestImg = SDL_LoadBMP("chessBmps/depthBest.bmp");

	slotFirstImg = SDL_LoadBMP("chessBmps/slotFirst.bmp");

	gameHeaderImg = SDL_LoadBMP("chessBmps/goodLuck.bmp");
	bestMoveImg = SDL_LoadBMP("chessBmps/bestMove.bmp");
	saveImg = SDL_LoadBMP("chessBmps/save.bmp");
	mainMenuImg = SDL_LoadBMP("chessBmps/mainMenuGame.bmp");
	quit2Img = SDL_LoadBMP("chessBmps/quit.bmp");
	currWhiteImg = SDL_LoadBMP("chessBmps/currWhite.bmp");
	currBlackImg = SDL_LoadBMP("chessBmps/currBlack.bmp");
	CheckImg = SDL_LoadBMP("chessBmps/check.bmp");
	TieImg = SDL_LoadBMP("chessBmps/tie.bmp");
	MateImg = SDL_LoadBMP("chessBmps/mate.bmp");

	depth1MoveImg = SDL_LoadBMP("chessBmps/depth1Move.bmp");
	depth2MoveImg = SDL_LoadBMP("chessBmps/depth2Move.bmp");
	depth3MoveImg = SDL_LoadBMP("chessBmps/depth3Move.bmp");
	depth4MoveImg = SDL_LoadBMP("chessBmps/depth4Move.bmp");
	depthBMoveImg = SDL_LoadBMP("chessBmps/depthBMove.bmp");

	//Pieces

	whitePawnSet = SDL_LoadBMP("chessBmps/pieces/wps.bmp");
	blackPawnSet = SDL_LoadBMP("chessBmps/pieces/bps.bmp");
	whiteRookSet = SDL_LoadBMP("chessBmps/pieces/wrs.bmp");
	blackRookSet = SDL_LoadBMP("chessBmps/pieces/brs.bmp");
	whiteKnightSet = SDL_LoadBMP("chessBmps/pieces/wns.bmp");
	blackKnightSet = SDL_LoadBMP("chessBmps/pieces/bns.bmp");
	whiteBishopSet = SDL_LoadBMP("chessBmps/pieces/wbs.bmp");
	blackBishopSet = SDL_LoadBMP("chessBmps/pieces/bbs.bmp");
	whiteQueenSet = SDL_LoadBMP("chessBmps/pieces/wqs.bmp");
	blackQueenSet = SDL_LoadBMP("chessBmps/pieces/bqs.bmp");
	whiteKingSet = SDL_LoadBMP("chessBmps/pieces/wks.bmp");
	blackKingSet = SDL_LoadBMP("chessBmps/pieces/bks.bmp");



	whitePawnWhite = SDL_LoadBMP("chessBmps/pieces/wpw.bmp");
	whitePawnBlack = SDL_LoadBMP("chessBmps/pieces/wpb.bmp");
	blackPawnWhite = SDL_LoadBMP("chessBmps/pieces/bpw.bmp");
	blackPawnBlack = SDL_LoadBMP("chessBmps/pieces/bpb.bmp");
	whiteRookWhite = SDL_LoadBMP("chessBmps/pieces/wrw.bmp");
	whiteRookBlack = SDL_LoadBMP("chessBmps/pieces/wrb.bmp");
	blackRookWhite = SDL_LoadBMP("chessBmps/pieces/brw.bmp");
	blackRookBlack = SDL_LoadBMP("chessBmps/pieces/brb.bmp");
	whiteKnightWhite = SDL_LoadBMP("chessBmps/pieces/wnw.bmp");
	whiteKnightBlack = SDL_LoadBMP("chessBmps/pieces/wnb.bmp");
	blackKnightWhite = SDL_LoadBMP("chessBmps/pieces/bnw.bmp");
	blackKnightBlack = SDL_LoadBMP("chessBmps/pieces/bnb.bmp");
	whiteBishopWhite = SDL_LoadBMP("chessBmps/pieces/wbw.bmp");
	whiteBishopBlack = SDL_LoadBMP("chessBmps/pieces/wbb.bmp");
	blackBishopWhite = SDL_LoadBMP("chessBmps/pieces/bbw.bmp");
	blackBishopBlack = SDL_LoadBMP("chessBmps/pieces/bbb.bmp");
	whiteQueenWhite = SDL_LoadBMP("chessBmps/pieces/wqw.bmp");
	whiteQueenBlack = SDL_LoadBMP("chessBmps/pieces/wqb.bmp");
	blackQueenWhite = SDL_LoadBMP("chessBmps/pieces/bqw.bmp");
	blackQueenBlack = SDL_LoadBMP("chessBmps/pieces/bqb.bmp");
	whiteKingWhite = SDL_LoadBMP("chessBmps/pieces/wkw.bmp");
	whiteKingBlack = SDL_LoadBMP("chessBmps/pieces/wkb.bmp");
	blackKingWhite = SDL_LoadBMP("chessBmps/pieces/bkw.bmp");
	blackKingBlack = SDL_LoadBMP("chessBmps/pieces/bkb.bmp");




	return 1;
}

/*	check if a given click event is clicked on a given button	*/
int isOnButton(SDL_Event click, SDL_Rect Butt) {
	if ((click.button.x > Butt.x) && (click.button.x < Butt.x + Butt.w)
			&& (click.button.y > Butt.y)
			&& (click.button.y < Butt.y + Butt.h)) {
		return 1;
	}
	return 0;
}

/*	quit the GUI by freeing all surfaces	*/
void quitGUI() {
	SDL_FreeSurface(welcomeImg);
	SDL_FreeSurface(newImg);
	SDL_FreeSurface(loadImg);
	SDL_FreeSurface(quitImg);
	SDL_FreeSurface(firstImg);
	SDL_FreeSurface(secondImg);
	SDL_FreeSurface(thirdImg);
	SDL_FreeSurface(fourthImg);
	SDL_FreeSurface(fifthImg);
	SDL_FreeSurface(sixthImg);
	SDL_FreeSurface(seventhImg);

	SDL_FreeSurface(settingHeaderImg);
	SDL_FreeSurface(whiteSlot);
	SDL_FreeSurface(whiteSlotChecked);
	SDL_FreeSurface(blackSlot);
	SDL_FreeSurface(blackSlotChecked);
	SDL_FreeSurface(whiteSlotUp);
	SDL_FreeSurface(blackSlotUp);
	SDL_FreeSurface(whiteSlotDown);
	SDL_FreeSurface(blackSlotDown);
	SDL_FreeSurface(whiteSlotRight);
	SDL_FreeSurface(blackSlotRight);
	SDL_FreeSurface(whiteSlotLeft);
	SDL_FreeSurface(blackSlotLeft);
	SDL_FreeSurface(whiteSlotLeftUp);
	SDL_FreeSurface(blackSlotRightUp);
	SDL_FreeSurface(whiteSlotRightDown);
	SDL_FreeSurface(blackSlotLeftDown);

	SDL_FreeSurface(blackRook);

	SDL_FreeSurface(gameModeImg);
	SDL_FreeSurface(diffImg);
	SDL_FreeSurface(usrColImg);
	SDL_FreeSurface(clearImg);
	SDL_FreeSurface(nextPlayerImg);
	SDL_FreeSurface(removeImg);
	SDL_FreeSurface(removeEnlight);
	SDL_FreeSurface(setImg);
	SDL_FreeSurface(backImg);
	SDL_FreeSurface(startImg);
	SDL_FreeSurface(pVpImg);
	SDL_FreeSurface(pVaImg);
	SDL_FreeSurface(click2changeImg);
	SDL_FreeSurface(nextBlackImg);
	SDL_FreeSurface(nextWhiteImg);
	SDL_FreeSurface(depth1Img);
	SDL_FreeSurface(depth2Img);
	SDL_FreeSurface(depth3Img);
	SDL_FreeSurface(depth4Img);
	SDL_FreeSurface(depthBestImg);
	SDL_FreeSurface(slotFirstImg);

	SDL_FreeSurface(gameHeaderImg);
	SDL_FreeSurface(bestMoveImg);
	SDL_FreeSurface(saveImg);
	SDL_FreeSurface(mainMenuImg);
	SDL_FreeSurface(quit2Img);
	SDL_FreeSurface(currWhiteImg);
	SDL_FreeSurface(currBlackImg);
	SDL_FreeSurface(CheckImg);
	SDL_FreeSurface(TieImg);
	SDL_FreeSurface(MateImg);
	SDL_FreeSurface(depth1MoveImg);
	SDL_FreeSurface(depth2MoveImg);
	SDL_FreeSurface(depth3MoveImg);
	SDL_FreeSurface(depth4MoveImg);
	SDL_FreeSurface(depthBMoveImg);


	//set pieces
	SDL_FreeSurface(whitePawnSet);
	SDL_FreeSurface(blackPawnSet);
	SDL_FreeSurface(whiteRookSet);
	SDL_FreeSurface(blackRookSet);
	SDL_FreeSurface(whiteKnightSet);
	SDL_FreeSurface(blackKnightSet);
	SDL_FreeSurface(whiteBishopSet);
	SDL_FreeSurface(blackBishopSet);
	SDL_FreeSurface(whiteQueenSet);
	SDL_FreeSurface(blackQueenSet);
	SDL_FreeSurface(whiteKingSet);
	SDL_FreeSurface(blackKingSet);

	//board pieces
	SDL_FreeSurface(whitePawnWhite);
	SDL_FreeSurface(whitePawnBlack);
	SDL_FreeSurface(blackPawnWhite);
	SDL_FreeSurface(blackPawnBlack);
	SDL_FreeSurface(whiteRookWhite);
	SDL_FreeSurface(whiteRookBlack);
	SDL_FreeSurface(blackRookWhite);
	SDL_FreeSurface(blackRookBlack);
	SDL_FreeSurface(whiteKnightWhite);
	SDL_FreeSurface(whiteKnightBlack);
	SDL_FreeSurface(blackKnightWhite);
	SDL_FreeSurface(blackKnightBlack);
	SDL_FreeSurface(whiteBishopWhite);
	SDL_FreeSurface(whiteBishopBlack);
	SDL_FreeSurface(blackBishopWhite);
	SDL_FreeSurface(blackBishopBlack);
	SDL_FreeSurface(whiteQueenWhite);
	SDL_FreeSurface(whiteQueenBlack);
	SDL_FreeSurface(blackQueenWhite);
	SDL_FreeSurface(blackQueenBlack);
	SDL_FreeSurface(whiteKingWhite);
	SDL_FreeSurface(whiteKingBlack);
	SDL_FreeSurface(blackKingWhite);
	SDL_FreeSurface(blackKingBlack);
	SDL_FreeSurface(menuBGcol);
	SDL_FreeSurface(boardBGCol);
	SDL_FreeSurface(boardBGCol2);
	SDL_FreeSurface(errorWindow);

	free(cmdGUI);
	SDL_Quit();

}

/*	end main functions	*/

/*	first window functions	*/

/*	draw main menu window	*/
void DrawFirstWindow() {

	if (SDL_BlitSurface(menuBGcol, NULL, window, &wholeWindow) != 0) { //background
		SDL_FreeSurface(menuBGcol);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	if (SDL_BlitSurface(welcomeImg, NULL, window, &welcomeBtn) != 0) { //background
		SDL_FreeSurface(welcomeImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	if (SDL_BlitSurface(newImg, NULL, window, &newBtn) != 0) { //background
		SDL_FreeSurface(newImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(loadImg, NULL, window, &loadBtn) != 0) { //background
		SDL_FreeSurface(loadImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(quitImg, NULL, window, &quitBtn) != 0) { //background
		SDL_FreeSurface(quitImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	if (firstWindowLoadFlag) {
		if (SDL_BlitSurface(firstImg, NULL, window, &firstLoad) != 0) { //background
			SDL_FreeSurface(firstImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(secondImg, NULL, window, &secondLoad) != 0) { //background
			SDL_FreeSurface(secondImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(thirdImg, NULL, window, &thirdLoad) != 0) { //background
			SDL_FreeSurface(thirdImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(fourthImg, NULL, window, &fourthLoad) != 0) { //background
			SDL_FreeSurface(fourthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(fifthImg, NULL, window, &fifthLoad) != 0) { //background
			SDL_FreeSurface(fifthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(sixthImg, NULL, window, &sixthLoad) != 0) { //background
			SDL_FreeSurface(sixthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(seventhImg, NULL, window, &seventhLoad) != 0) { //background
			SDL_FreeSurface(seventhImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	//print window
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}
}

/*	analyze the event within the main menu, mainly:
 * flipping the relevant flags
 * building the execution string
 *  */
void analyzeFirst(SDL_Event e) {
	prevEvent = e;
	//check if needs to quit
	switch (e.type) {
	case (SDL_QUIT):
		strcpy(cmdGUI, "quit");
		break;
	case (SDL_KEYUP):
		if (e.key.keysym.sym == SDLK_ESCAPE)
			strcpy(cmdGUI, "quit");
		break;
	case (SDL_MOUSEBUTTONUP):
		if (isOnButton(e, quitBtn)) {
			strcpy(cmdGUI,"quit");
		}
		if (isOnButton(e, newBtn)) {
			gameState = 0;
			zeroFlags();
			strcpy(cmdGUI,"continue");
		}
		if (isOnButton(e, loadBtn)) {
			//lift flag for seven buttons
			firstWindowLoadFlag = 1 - firstWindowLoadFlag;
			strcpy(cmdGUI,"continue");
		}
		if (firstWindowLoadFlag) { //load buttons appear in the window
			if (isOnButton(e, firstLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess1.xml");
			}
			if (isOnButton(e, secondLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess2.xml");
			}
			if (isOnButton(e, thirdLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess3.xml");
			}
			if (isOnButton(e, fourthLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess4.xml");
			}
			if (isOnButton(e, fifthLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess5.xml");
			}
			if (isOnButton(e, sixthLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess6.xml");
			}
			if (isOnButton(e, seventhLoad)) {
				gameState = 0;
				zeroFlags();
				strcpy(cmdGUI,"load savedGames/chess7.xml");
			}
		}
		break;
	default:
		strcpy(cmdGUI, "continue");
		break;
	}

}


/*	end first window functions	*/

/*	second window functions	*/

/*	draw the setting stage window	*/
void DrawSecondWindow() {

	//background
	if (SDL_BlitSurface(boardBGCol, NULL, window, &wholeWindow) != 0) {
		SDL_FreeSurface(blackSlot);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(gameModeImg, NULL, window, &gameModeBtn) != 0) {
		SDL_FreeSurface(gameModeImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (gameModeFlag) {
		if (SDL_BlitSurface(pVpImg, NULL, window, &pVpBtn) != 0) {
			SDL_FreeSurface(pVpImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(pVaImg, NULL, window, &pVaBtn) != 0) {
			SDL_FreeSurface(pVaImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (gameMode == 2) {
		if (SDL_BlitSurface(diffImg, NULL, window, &diffBtn) != 0) {
			SDL_FreeSurface(diffImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (diffFlag) {
			if (SDL_BlitSurface(depth1Img, NULL, window, &depth1Btn) != 0) {
				SDL_FreeSurface(depth1Img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			if (SDL_BlitSurface(depth2Img, NULL, window, &depth2Btn) != 0) {
				SDL_FreeSurface(depth2Img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			if (SDL_BlitSurface(depth3Img, NULL, window, &depth3Btn) != 0) {
				SDL_FreeSurface(depth3Img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			if (SDL_BlitSurface(depth4Img, NULL, window, &depth4Btn) != 0) {
				SDL_FreeSurface(depth4Img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			if (SDL_BlitSurface(depthBestImg, NULL, window, &depthBestBtn)
					!= 0) {
				SDL_FreeSurface(depthBestImg);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		if (SDL_BlitSurface(usrColImg, NULL, window, &usrColBtn) != 0) {
			SDL_FreeSurface(usrColImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (usrColFlag) {
			if (SDL_BlitSurface(click2changeImg, NULL, window, &click2changeBtn)
					!= 0) {
				SDL_FreeSurface(click2changeImg);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			if (!userColor) {
				if (SDL_BlitSurface(nextWhiteImg, NULL, window, &nextColBtn)
						!= 0) {
					SDL_FreeSurface(nextWhiteImg);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			} else {
				if (SDL_BlitSurface(nextBlackImg, NULL, window, &nextColBtn)
						!= 0) {
					SDL_FreeSurface(nextBlackImg);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		}
	}
	if (SDL_BlitSurface(clearImg, NULL, window, &clearBtn) != 0) {
		SDL_FreeSurface(clearImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(nextPlayerImg, NULL, window, &nextPlayerBtn) != 0) {
		SDL_FreeSurface(nextPlayerImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (nextFlag) {
		if (SDL_BlitSurface(click2changeImg, NULL, window, &click2changeBtn)
				!= 0) {
			SDL_FreeSurface(click2changeImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (!whosTurn) {
			if (SDL_BlitSurface(nextWhiteImg, NULL, window, &nextColBtn) != 0) {
				SDL_FreeSurface(nextWhiteImg);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(nextBlackImg, NULL, window, &nextColBtn) != 0) {
				SDL_FreeSurface(nextBlackImg);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	}

	if (SDL_BlitSurface(removeImg, NULL, window, &removeBtn) != 0) {
		SDL_FreeSurface(removeImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (removeFlag) {
		if (SDL_BlitSurface(removeEnlight, NULL, window, &removeBtn) != 0) {
			SDL_FreeSurface(removeEnlight);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (SDL_BlitSurface(setImg, NULL, window, &setBtn) != 0) {
		SDL_FreeSurface(setImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (setFlag) {
		if (SDL_BlitSurface(slotFirstImg, NULL, window, &slotFirstBtn) != 0) {
			SDL_FreeSurface(slotFirstImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		for (int j = 0; j < 6; j++) {
			setPieces[0][j].x = settingsOptX + 10;
			setPieces[0][j].y = WIN_H / 4 + j * pieceSize;
			setPieces[0][j].w = pieceSize;
			setPieces[0][j].h = pieceSize;
			switch(j){
				case(0):
					if (SDL_BlitSurface(blackPawnSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(blackPawnSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(1):
					if (SDL_BlitSurface(blackRookSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(blackRookSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(2):
					if (SDL_BlitSurface(blackKnightSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(whiteKnightSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(3):
					if (SDL_BlitSurface(blackBishopSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(blackBishopSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(4):
					if (SDL_BlitSurface(blackQueenSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(blackQueenSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(5):
					if (SDL_BlitSurface(blackKingSet, NULL, window, &setPieces[1][j])	!= 0) {
						SDL_FreeSurface(blackKingSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
			}
		}
		for (int j = 0; j < 6; j++) {
			setPieces[1][j].x = settingsOptX + pieceSize + 30;
			setPieces[1][j].y = WIN_H / 4 + j * pieceSize;
			setPieces[1][j].w = pieceSize;
			setPieces[1][j].h = pieceSize;
			switch(j){
				case(0):
					if (SDL_BlitSurface(whitePawnSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whitePawnSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(1):
					if (SDL_BlitSurface(whiteRookSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whiteRookSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(2):
					if (SDL_BlitSurface(whiteKnightSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whiteKnightSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(3):
					if (SDL_BlitSurface(whiteBishopSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whiteBishopSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(4):
					if (SDL_BlitSurface(whiteQueenSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whiteQueenSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
				case(5):
					if (SDL_BlitSurface(whiteKingSet, NULL, window, &setPieces[0][j])	!= 0) {
						SDL_FreeSurface(whiteKingSet);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
					break;
			}
		}
	}
	if (SDL_BlitSurface(startImg, NULL, window, &startBtn) != 0) {
		SDL_FreeSurface(startImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(backImg, NULL, window, &backBtn) != 0) {
		SDL_FreeSurface(backImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//draw the board
			GUIBoard[i][j].x = boardOffset + i * slotSize;
			GUIBoard[i][j].y = boardOffset + 20 + j * slotSize;
			GUIBoard[i][j].w = slotSize;
			GUIBoard[i][j].h = slotSize;
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					boardGUI(0, i, j);
				} else {
					boardGUI(1, i, j);
				}
			} else {
				if (j % 2 == 0) {
					boardGUI(1, i, j);
				} else {
					boardGUI(0, i, j);
				}
			}
			if (board[i][7 - j] != 0) {
				GUIPieces[i][7 - j].x = boardOffset + i * slotSize + 5;
				GUIPieces[i][7 - j].y = boardOffset + 20 + j * slotSize + 5;
				GUIPieces[i][j].w = pieceSize;
				GUIPieces[i][j].h = pieceSize;
				if (i % 2 == 0) {
					if (j % 2 == 0) {
						piecesGUI(0, i, 7 - j);
					} else {
						piecesGUI(1, i, 7 - j);
					}
				} else {
					if (j % 2 == 0) {
						piecesGUI(1, i, 7 - j);
					} else {
						piecesGUI(0, i, 7 - j);
					}
				}
			}
		}
	}

	//window title
	if (SDL_BlitSurface(settingHeaderImg, NULL, window, &settingsHeader) != 0) {
		SDL_FreeSurface(settingHeaderImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}
}

/*	analyze the event within the settings menu, mainly:
 * flipping the relevant flags
 * building the execution string
 *  */
void analyzeSecond(SDL_Event e) {
	prevEvent = e;
	//check if needs to quit
	switch (e.type) {
	case (SDL_QUIT):
		stringFlag = 2;
		strcpy(cmdGUI,"quit");
		break;
	case (SDL_KEYUP):
		if (e.key.keysym.sym == SDLK_ESCAPE)
			strcpy(cmdGUI,"quit");
		break;
	case (SDL_MOUSEBUTTONUP):
		if (isOnButton(e, backBtn)) {
			gameState = -1;
			zeroFlags();
			strcpy(cmdGUI,"continue");
		}
		if (isOnButton(e, startBtn)) {
			stringFlag = 2;
			zeroFlags();
			strcpy(cmdGUI,"start");
		}
		if (isOnButton(e, clearBtn)) {
			stringFlag = 2;
			strcpy(cmdGUI,"clear");
		}

		if (isOnButton(e, nextPlayerBtn)) {
			nextFlag = 1 - nextFlag;
			if (nextFlag) {
				zeroFlags();
				nextFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (nextFlag) {
			if (isOnButton(e, nextColBtn)) {
				whosTurn = 1 - whosTurn;
				strcpy(cmdGUI,"continue");
			}
		}
		if (isOnButton(e, gameModeBtn)) {
			gameModeFlag = 1 - gameModeFlag;
			if (gameModeFlag) {
				zeroFlags();
				gameModeFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (gameModeFlag) {
			if (isOnButton(e, pVpBtn)) {
				stringFlag = 2;
				strcpy(cmdGUI,"game_mode 1");
			} else if (isOnButton(e, pVaBtn)) {
				stringFlag = 2;
				strcpy(cmdGUI,"game_mode 2");
			}
		}
		if (isOnButton(e, usrColBtn)) {
			usrColFlag = 1 - usrColFlag;
			if (usrColFlag) {
				zeroFlags();
				usrColFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (usrColFlag) {
			if (isOnButton(e, nextColBtn)) {
				userColor = 1 - userColor;
				strcpy(cmdGUI,"continue");
			}
		}
		if (isOnButton(e, diffBtn)) {
			diffFlag = 1 - diffFlag;
			if (diffFlag) {
				zeroFlags();
				diffFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (diffFlag){
			if(isOnButton(e, depth1Btn)){
				diffFlag = 1 - diffFlag;
				stringFlag = 2;
				strcpy(cmdGUI,"difficulty depth 1");
			}
			else if(isOnButton(e, depth2Btn)){
				diffFlag = 1 - diffFlag;
				stringFlag = 2;
				strcpy(cmdGUI,"difficulty depth 2");
			}
			else if(isOnButton(e, depth3Btn)){
				diffFlag = 1 - diffFlag;
				stringFlag = 2;
				strcpy(cmdGUI,"difficulty depth 3");
			}
			else if(isOnButton(e, depth4Btn)){
				diffFlag = 1 - diffFlag;
				stringFlag = 2;
				strcpy(cmdGUI,"difficulty depth 4");
			}
			else if(isOnButton(e, depthBestBtn)){
				diffFlag = 1 - diffFlag;
				stringFlag = 2;
				strcpy(cmdGUI,"difficulty best");
			}
		}
		if (isOnButton(e, removeBtn)) {
			removeFlag = 1 - removeFlag;
			if (removeFlag) {
				zeroFlags();
				removeFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (removeFlag) {
			strcpy(cmdGUI, "rm <");
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (isOnButton(e, GUIBoard[i][j])) {
						char locCol[5];
						char locRow[5];
						sprintf(locCol, "%c", i + 'a');
						sprintf(locRow, "%d", 8 - j);
						strcat(cmdGUI, locCol);
						strcat(cmdGUI, ",");
						strcat(cmdGUI, locRow);
						strcat(cmdGUI, "> ");
						stringFlag = 2;
						strcpy(cmdGUI,cmdGUI);
					}
		}
		if (isOnButton(e, setBtn)) {
			setFlag = 1 - setFlag;
			if (setFlag) {
				zeroFlags();
				setFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (setFlag && stringFlag == 0) {
			strcpy(cmdGUI, "set <");
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (isOnButton(e, GUIBoard[i][j])) {
						char locCol[5];
						char locRow[5];
						sprintf(locCol, "%c", i + 'a');
						sprintf(locRow, "%d", 8 - j);
						strcat(cmdGUI, locCol);
						strcat(cmdGUI, ",");
						strcat(cmdGUI, locRow);
						strcat(cmdGUI, "> ");
						stringFlag = 1;
						strcpy(cmdGUI,cmdGUI);
					}
		}
		if (setFlag && stringFlag == 1) {
			if (isOnButton(e, setPieces[0][0])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white pawn");
			}
			else if (isOnButton(e, setPieces[0][1])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white rook");
			}
			else if (isOnButton(e, setPieces[0][2])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white knight");
			}
			else if (isOnButton(e, setPieces[0][3])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white bishop");
			}
			else if (isOnButton(e, setPieces[0][4])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white queen");
			}
			else if (isOnButton(e, setPieces[0][5])) {
				stringFlag = 2;
				strcpy(cmdGUI,"white king");
			}
			else if (isOnButton(e, setPieces[1][0])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black pawn");
			}
			else if (isOnButton(e, setPieces[1][1])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black rook");
			}
			else if (isOnButton(e, setPieces[1][2])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black knight");
			}
			else if (isOnButton(e, setPieces[1][3])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black bishop");
			}
			else if (isOnButton(e, setPieces[1][4])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black queen");
			}
			else if (isOnButton(e, setPieces[1][5])) {
				stringFlag = 2;
				strcpy(cmdGUI,"black king");
			}

		}

		break;
	default:
		strcpy(cmdGUI,"");
		break;
	}
}

/*	end second window functions	*/

/*	third window functions	*/
/*	draw the setting stage window	*/
void DrawThirdWindow() {

	//background
	if (SDL_BlitSurface(boardBGCol2, NULL, window, &wholeWindow) != 0) {
		SDL_FreeSurface(boardBGCol2);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (whosTurn) {
		if (SDL_BlitSurface(currBlackImg, NULL, window, &currPlayerBtn) != 0) {
			SDL_FreeSurface(currBlackImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	} else {
		if (SDL_BlitSurface(currWhiteImg, NULL, window, &currPlayerBtn) != 0) {
			SDL_FreeSurface(currWhiteImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}

	if (bestMoveFlag == 1){
		for (int i=0; i < 5; i++){
			bestMovesDepths[i].x = settingsOptX + 10 + i * (pieceSize);
			bestMovesDepths[i].y = WIN_H / 4;
			bestMovesDepths[i].w = pieceSize - 5;
			bestMovesDepths[i].h = pieceSize - 5;
			switch (i){
			case (0):
					if (SDL_BlitSurface(depth1MoveImg, NULL, window, &bestMovesDepths[i]) != 0) {
						SDL_FreeSurface(depth1MoveImg);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
			break;
			case(1):
					if (SDL_BlitSurface(depth2MoveImg, NULL, window, &bestMovesDepths[i]) != 0) {
						SDL_FreeSurface(depth2MoveImg);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
			break;
			case(2):
					if (SDL_BlitSurface(depth3MoveImg, NULL, window, &bestMovesDepths[i]) != 0) {
						SDL_FreeSurface(depth3MoveImg);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
			break;
			case(3):
					if (SDL_BlitSurface(depth4MoveImg, NULL, window, &bestMovesDepths[i]) != 0) {
						SDL_FreeSurface(depth4MoveImg);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
			break;
			case(4):
					if (SDL_BlitSurface(depthBMoveImg, NULL, window, &bestMovesDepths[i]) != 0) {
						SDL_FreeSurface(depthBMoveImg);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
			break;
			}
		}
	}
	if(bestMoveFlag == 2){
		if(AIMove != NULL){
			pickedCol = AIMove->from.column;
			pickedRow = 7 - AIMove->from.row;
			pickedColTo = AIMove->target.column;
			pickedRowTo = 7 - AIMove->target.row;
			delMove(AIMove);
			AIMove = NULL;
		}
	}
	if(bestMoveFlag == 3){
		SDL_Delay(1000);
		zeroFlags();
	}
	//Blit the 4 piece PromArray according to the whosTurn
	if (promFlag){
		for (int i=0; i<4; i++){
			promPieces[i].x = settingsOptX + 10 + i * (pieceSize + 3);
			promPieces[i].y = WIN_H / 4;
			promPieces[i].w = pieceSize;
			promPieces[i].h = pieceSize;
			if (!whosTurn){
				switch (i){
				case (0):
						if (SDL_BlitSurface(whiteRookSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(whiteRookSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(1):
						if (SDL_BlitSurface(whiteKnightSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(whiteKnightSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(2):
						if (SDL_BlitSurface(whiteBishopSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(whiteBishopSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(3):
						if (SDL_BlitSurface(whiteQueenSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(whiteQueenSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				}
			}
			else{
				switch (i){
				case (0):
						if (SDL_BlitSurface(blackRookSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(blackRookSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(1):
						if (SDL_BlitSurface(blackKnightSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(blackKnightSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(2):
						if (SDL_BlitSurface(blackBishopSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(blackBishopSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				case(3):
						if (SDL_BlitSurface(blackQueenSet, NULL, window, &promPieces[i]) != 0) {
							SDL_FreeSurface(blackQueenSet);
							printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						}
				break;
				}
			}
		}
	}

	if (checkFlag){
		if (SDL_BlitSurface(CheckImg, NULL, window, &specialStatusBtn) != 0) { //background
			SDL_FreeSurface(CheckImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (tieFlag){
		if (SDL_BlitSurface(TieImg, NULL, window, &specialStatusBtn) != 0) { //background
			SDL_FreeSurface(TieImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (mateFlag){
		if (SDL_BlitSurface(MateImg, NULL, window, &specialStatusBtn) != 0) { //background
			SDL_FreeSurface(MateImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (SDL_BlitSurface(bestMoveImg, NULL, window, &bestMoveBtn) != 0) {
		SDL_FreeSurface(bestMoveImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (SDL_BlitSurface(saveImg, NULL, window, &saveBtn) != 0) {
		SDL_FreeSurface(saveImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}
	if (saveFlag) {
		if (SDL_BlitSurface(firstImg, NULL, window, &firstSave) != 0) { //background
			SDL_FreeSurface(firstImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(secondImg, NULL, window, &secondSave) != 0) { //background
			SDL_FreeSurface(secondImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(thirdImg, NULL, window, &thirdSave) != 0) { //background
			SDL_FreeSurface(thirdImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(fourthImg, NULL, window, &fourthSave) != 0) { //background
			SDL_FreeSurface(fourthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(fifthImg, NULL, window, &fifthSave) != 0) { //background
			SDL_FreeSurface(fifthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(sixthImg, NULL, window, &sixthSave) != 0) { //background
			SDL_FreeSurface(sixthImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
		if (SDL_BlitSurface(seventhImg, NULL, window, &seventhSave) != 0) { //background
			SDL_FreeSurface(seventhImg);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if (SDL_BlitSurface(quit2Img, NULL, window, &quit2Btn) != 0) {
		SDL_FreeSurface(quit2Img);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//draw the board
			GUIBoard[i][j].x = boardOffset + i * slotSize;
			GUIBoard[i][j].y = boardOffset + 20 + j * slotSize;
			GUIBoard[i][j].w = slotSize;
			GUIBoard[i][j].h = slotSize;
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					boardGUI(0, i, j);
				} else {
					boardGUI(1, i, j);
				}
			} else {
				if (j % 2 == 0) {
					boardGUI(1, i, j);
				} else {
					boardGUI(0, i, j);
				}
			}
			if (board[i][7 - j] != 0) {
				GUIPieces[i][7 - j].x = boardOffset + i * slotSize + 5;
				GUIPieces[i][7 - j].y = boardOffset + 20 + j * slotSize + 5;
				GUIPieces[i][j].w = pieceSize;
				GUIPieces[i][j].h = pieceSize;
				if (i % 2 == 0) {
					if (j % 2 == 0) {
						piecesGUI(0, i, 7 - j);
					} else {
						piecesGUI(1, i, 7 - j);
					}
				} else {
					if (j % 2 == 0) {
						piecesGUI(1, i, 7 - j);
					} else {
						piecesGUI(0, i, 7 - j);
					}
				}
			}
		}
	}

	if (SDL_BlitSurface(mainMenuImg, NULL, window, &mainMenu) != 0) {
		SDL_FreeSurface(mainMenuImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	//window title
	if (SDL_BlitSurface(gameHeaderImg, NULL, window, &gameHeader) != 0) {
		SDL_FreeSurface(gameHeaderImg);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	/* We finished drawing*/
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}
}

/*	analyze the event within the game menu, mainly:
 * flipping the relevant flags
 * building the execution string
 *  */
void analyzeThird(SDL_Event e) {
	prevEvent = e;
	//check if needs to quit
	switch (e.type) {
	case (SDL_QUIT):
		stringFlag = 2;
		strcpy(cmdGUI,"quit");
		break;
	case (SDL_KEYUP):
		stringFlag = 2;
		if (e.key.keysym.sym == SDLK_ESCAPE)
			strcpy(cmdGUI,"quit");
		break;
	case (SDL_MOUSEBUTTONUP):
		if (isOnButton(e, mainMenu)) {
			gameState = -1;
			zeroFlags();
			stringFlag = 0;
			strcpy(cmdGUI,"continue");
		}
		if (isOnButton(e, quit2Btn)) {
			stringFlag = 2;
			zeroFlags();
			strcpy(cmdGUI,"quit");
		}
		if (isOnButton(e, saveBtn)) {
			saveFlag = 1 - saveFlag;
			if (saveFlag) {
				pickedCol = -1;
				pickedRow = -1;
				bestMoveFlag = 0;
				saveFlag = 1;
			}
			strcpy(cmdGUI,"continue");
		}
		if (saveFlag) {
			if (isOnButton(e, firstSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess1.xml");
			}
			if (isOnButton(e, secondSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess2.xml");
			}
			if (isOnButton(e, thirdSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess3.xml");
			}
			if (isOnButton(e, fourthSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess4.xml");
			}
			if (isOnButton(e, fifthSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess5.xml");
			}
			if (isOnButton(e, sixthSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess6.xml");
			}
			if (isOnButton(e, seventhSave)) {
				stringFlag = 2;
				strcpy(cmdGUI,"save savedGames/chess7.xml");
			}
		}

		if (!tieFlag && !mateFlag){
			if (isOnButton(e, bestMoveBtn)) {
				zeroFlags();
				bestMoveFlag = 1;
				stringFlag = 1;
				strcpy(cmdGUI,"get_best_moves");
			}
			if( bestMoveFlag == 1 ){
				for (int i=0; i<5; i++){
					if (isOnButton(e, bestMovesDepths[i])) {
						switch(i){
						case (0):
							bestMoveFlag = 2;
							stringFlag=2;
							strcpy(cmdGUI," 1");
						break;
						case (1):
							bestMoveFlag = 2;
							stringFlag=2;
							strcpy(cmdGUI," 2");
							break;
						case (2):
							bestMoveFlag = 2;
							stringFlag=2;
							strcpy(cmdGUI," 3");
							break;
						case (3):
							bestMoveFlag = 2;
							stringFlag=2;
							strcpy(cmdGUI," 4");
							break;
						case (4):
							bestMoveFlag = 2;
							stringFlag=2;
							strcpy(cmdGUI," best");
							break;
						}
					}
				}
			}
			if (stringFlag == 0 && !bestMoveFlag) {
				strcpy(cmdGUI, "move <");
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						if (isOnButton(e, GUIBoard[i][j])) {
							pickedCol = i;
							pickedRow = j;
							char locCol[5];
							char locRow[5];
							sprintf(locCol, "%c", pickedCol + 'a');
							sprintf(locRow, "%d", 8 - pickedRow);
							strcat(cmdGUI, locCol);
							strcat(cmdGUI, ",");
							strcat(cmdGUI, locRow);
							strcat(cmdGUI, "> to ");
							stringFlag = 1;
						}
			}
			else if (stringFlag == 1 && !bestMoveFlag) {
				strcpy(cmdGUI, "<");
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						if (isOnButton(e, GUIBoard[i][j])) {
							char locCol[5];
							char locRow[5];
							sprintf(locCol, "%c", i + 'a');
							sprintf(locRow, "%d", 8 - j);
							strcat(cmdGUI, locCol);
							strcat(cmdGUI, ",");
							strcat(cmdGUI, locRow);
							strcat(cmdGUI, ">");
							if ((board[pickedCol][7-pickedRow] == 1 && j == 0 && pickedRow == 1 && pickedCol == i && whosTurn == 0)
									|| (board[pickedCol][7-pickedRow] == -1 && j==7 && pickedRow == 6 && pickedCol == i && whosTurn == 1) )
								promFlag = 1;
							else
								stringFlag = 2;
							pickedCol = -1;
							pickedRow = -1;
						}
			}
			if (promFlag){ // check which piece, make stringflag 2 make promflag 0,strcpy(cmdGUI,the piece name
				for (int i=0; i<4; i++){
					if (isOnButton(e, promPieces[i])) {
						switch(i){
						case (0):
										stringFlag=2;
						promFlag=0;
						strcpy(cmdGUI," rook");
						break;
						case (1):
										stringFlag=2;
						promFlag=0;
						strcpy(cmdGUI," knight");
						break;
						case (2):
										stringFlag=2;
						promFlag=0;
						strcpy(cmdGUI," bishop");
						break;
						case (3):
										stringFlag=2;
						promFlag=0;
						strcpy(cmdGUI," queen");
						break;

						}
					}
				}
			}
		}
		break;
	default:
		strcpy(cmdGUI,"");
		break;
	}

}

/*	end third window functions	*/

/*	updating check/mate/tie flags or drawing error window after executing command	*/
void exeGUI(int opSucsses) {
	switch (opSucsses){

	case(0):
			if (SDL_BlitSurface(errorWindow, NULL, window, &wholeWindow) != 0) { //background
				SDL_FreeSurface(errorWindow);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			//print window
			if (SDL_Flip(window) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			}
			SDL_Delay(800);
			break;
	case(1):
			checkFlag=0;
			break;
	case (2):
			checkFlag=1;
			break;
	case (3):
			tieFlag=1;
			break;
	case (4):
			mateFlag=1;
			break;
	}
}

/*	reset all the flags	*/
void zeroFlags() {
	//turn off all main menu flags
	firstWindowLoadFlag = 0;
	//turn off all settings state window flags
	nextFlag = 0;
	nextColFlag = 0;
	gameModeFlag = 0;
	usrColFlag = 0;
	diffFlag = 0;
	removeFlag = 0;
	setFlag = 0;
	checkFlag=0;
	tieFlag=0;
	mateFlag=0;
	promFlag=0;
	//turn off all game state flags
	saveFlag = 0;
	bestMoveFlag = 0;

	pickedCol=-1;
	pickedRow=-1;
	pickedColTo=-1;
	pickedRowTo=-1;
}

/*	draw a specific slot on the board	*/
void boardGUI(int slotColor, int column, int row) {
	if (column == 0) {
		if (row == 7) {
			if (slotColor) {
				if (SDL_BlitSurface(blackSlotLeftDown, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(blackSlotLeftDown);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		} else if (row == 0) {
			if (!slotColor) {
				if (SDL_BlitSurface(whiteSlotLeftUp, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(whiteSlotLeftUp);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		} else {
			if (slotColor) {
				if (SDL_BlitSurface(blackSlotLeft, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(blackSlotLeft);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			} else {
				if (SDL_BlitSurface(whiteSlotLeft, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(whiteSlotLeft);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		}
	} else if (column == 7) {
		if (row == 7) {
			if (!slotColor) {
				if (SDL_BlitSurface(whiteSlotRightDown, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(whiteSlotRightDown);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		} else if (row == 0) {
			if (slotColor) {
				if (SDL_BlitSurface(blackSlotRightUp, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(blackSlotRightUp);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		} else {
			if (slotColor) {
				if (SDL_BlitSurface(blackSlotRight, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(blackSlotRight);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			} else {
				if (SDL_BlitSurface(whiteSlotRight, NULL, window,
						&GUIBoard[column][row]) != 0) {
					SDL_FreeSurface(whiteSlotRight);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		}
	} else if (row == 7) {
		if (slotColor) {
			if (SDL_BlitSurface(blackSlotDown, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(blackSlotDown);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(whiteSlotDown, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(whiteSlotDown);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	} else if (row == 0) {
		if (slotColor) {
			if (SDL_BlitSurface(blackSlotUp, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(blackSlotUp);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(whiteSlotUp, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(whiteSlotUp);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	} else {
		if (slotColor) {
			if (SDL_BlitSurface(blackSlot, NULL, window, &GUIBoard[column][row])
					!= 0) {
				SDL_FreeSurface(blackSlot);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(whiteSlot, NULL, window, &GUIBoard[column][row])
					!= 0) {
				SDL_FreeSurface(whiteSlot);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	}
	if (pickedCol == column && pickedRow == row) {
		if (!slotColor) {
			if (SDL_BlitSurface(whiteSlotChecked, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(whiteSlotChecked);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(blackSlotChecked, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(blackSlotChecked);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	}
	if (pickedColTo == column && pickedRowTo == row) {
		bestMoveFlag = 3;
		if (!slotColor) {
			if (SDL_BlitSurface(whiteSlotChecked, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(whiteSlotChecked);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(blackSlotChecked, NULL, window,
					&GUIBoard[column][row]) != 0) {
				SDL_FreeSurface(blackSlotChecked);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	}
}

/*	draw a specific piece on the board	*/
void piecesGUI(int col, int column, int row) {

	switch (board[column][row]) {
	case (1):
		if(col == 1){
			if (SDL_BlitSurface(whitePawnBlack, NULL, window,
					&GUIPieces[column][row]) != 0) {
				SDL_FreeSurface(whitePawnBlack);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(whitePawnWhite, NULL, window,
					&GUIPieces[column][row]) != 0) {
				SDL_FreeSurface(whitePawnWhite);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		break;
	case (-1):
		if(col == 1){
			if (SDL_BlitSurface(blackPawnBlack, NULL, window,
					&GUIPieces[column][row]) != 0) {
				SDL_FreeSurface(blackPawnBlack);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		} else {
			if (SDL_BlitSurface(blackPawnWhite, NULL, window,
					&GUIPieces[column][row]) != 0) {
				SDL_FreeSurface(blackPawnWhite);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		break;
	case (2):
			if(col == 1){
				if (SDL_BlitSurface(whiteRookBlack, NULL, window,
						&GUIPieces[column][row]) != 0) {
					SDL_FreeSurface(whitePawnBlack);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			} else {
				if (SDL_BlitSurface(whiteRookWhite, NULL, window,
						&GUIPieces[column][row]) != 0) {
					SDL_FreeSurface(whitePawnWhite);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		break;
	case (-2):
				if(col == 1){
					if (SDL_BlitSurface(blackRookBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(blackRookWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (3):
			if(col == 1){
				if (SDL_BlitSurface(whiteKnightBlack, NULL, window,
						&GUIPieces[column][row]) != 0) {
					SDL_FreeSurface(whitePawnBlack);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			} else {
				if (SDL_BlitSurface(whiteKnightWhite, NULL, window,
						&GUIPieces[column][row]) != 0) {
					SDL_FreeSurface(whitePawnWhite);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				}
			}
		break;
	case (-3):
				if(col == 1){
					if (SDL_BlitSurface(blackKnightBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(blackKnightWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (4):
				if(col == 1){
					if (SDL_BlitSurface(whiteBishopBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(whiteBishopWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (-4):
				if(col == 1){
					if (SDL_BlitSurface(blackBishopBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(blackBishopWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (5):
				if(col == 1){
					if (SDL_BlitSurface(whiteQueenBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(whiteQueenWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (-5):
				if(col == 1){
					if (SDL_BlitSurface(blackQueenBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(blackQueenWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (6):
				if(col == 1){
					if (SDL_BlitSurface(whiteKingBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(whiteKingWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	case (-6):
				if(col == 1){
					if (SDL_BlitSurface(blackKingBlack, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnBlack);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				} else {
					if (SDL_BlitSurface(blackKingWhite, NULL, window,
							&GUIPieces[column][row]) != 0) {
						SDL_FreeSurface(whitePawnWhite);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					}
				}
		break;
	}

}



