#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <time.h>
#include <vector>
#include <stack>

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameState.h"

using namespace std;

class Square;
class Block;
class PlayState;
class ExitState;
class WinState;
class LoseState;
class MenuState;

class Game
{
public:

	Game();
	~Game();

	static Game* Instance()
	{
		if (instance == NULL)
		{
			instance = new Game();
		}
		return instance;
	}

	void Init();

	void Shutdown();

	void DrawBackground();
	void ClearScreen();
	void DisplayText(string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB);

	void Win();
	void Lose();

	void HandleBottomCollision();

	void ChangeFocusBlock();

	void SwapBlock();

	int CheckCompletedLines();

	void CheckWin();
	void CheckLoss();

	int GetLevel() { return level; }
	int GetScore() { return score; }

	int GetFocusBlockSpeed() { return focusBlockSpeed; }
	
	Block* GetFocusBlock() { return focusBlock; }
	Block* GetNextBlock()  { return nextBlock; }
	
	SDL_Surface* GetSpriteSheet() { return spriteSheet; }
	SDL_Surface* GetScreenSurface() { return screenSurface; }
	SDL_Window* GetWindow() { return window; }


	int GetTimer() { return timer; }
	void SetTimer(int newTimer)
	{
		timer = newTimer;
	}

	vector<Square*> GetPlacedSquares() { return placedSquares; }

	stack<GameState*>* GetGameStates() { return &GameStates; }


private:

	static Game* instance;

	int level;
	int score;

	int focusBlockSpeed;

	Block* focusBlock;
	Block* nextBlock;

	SDL_Surface* spriteSheet;
	SDL_Surface* screenSurface;
	SDL_Window* window;

	int timer;

	vector<Square*> placedSquares;

	stack<GameState*> GameStates;

	const int window_width = 300;
	const int window_height = 400;
	const string window_caption = "Tetris Clone";
	/*
	int GAME_AREA_LEFT = 53;
	int GAME_AREA_RIGHT = 251;
	int GAME_AREA_BOTTOM = 298;

	int FRAMES_PER_SECOND = 30;
	int FRAME_RATE = 1000 / FRAMES_PER_SECOND;

	int NUM_LEVELS = 5;
	int POINTS_PER_LINE = 525;
	int POINTS_PER_LEVEL = 6300;

	int INITIAL_SPEED = 60;
	int SPEED_CHANGE = 10;

	int SLIDE_TIME = 15;
	int SQUARES_PER_ROW = 10;
	int SQUARE_MEDIAN = 10;

	int BLOCK_START_X = 151;
	int BLOCK_START_Y = 59;

	int LEVEL_RECT_X = 42;
	int LEVEL_RECT_Y = 320;
	int SCORE_RECT_X = 42;
	int SCORE_RECT_Y = 340;
	int NEEDED_SCORE_RECT_X = 42;
	int NEEDED_SCORE_RECT_Y = 360;
	int NEXT_BLOCK_CIRCLE_X = 214;
	int NEXT_BLOCK_CIRCLE_Y = 347;

	int LEVEL_ONE_X = 0;
	int LEVEL_ONE_Y = 0;
	int LEVEL_TWO_X = 300;
	int LEVEL_TWO_Y = 0;
	int LEVEL_THREE_X = 300;
	int LEVEL_THREE_Y = 0;
	int LEVEL_FOUR_X = 0;
	int LEVEL_FOUR_Y = 396;
	int LEVEL_FIVE_X = 300;
	int LEVEL_FIVE_Y = 396;

	int RED_SQUARE_X = 600;
	int RED_SQUARE_Y = 400;
	int PURPLE_SQUARE_X = 620;
	int PURPLE_SQUARE_Y = 400;
	int GREY_SQUARE_X = 640;
	int GREY_SQUARE_Y = 400;
	int BLUE_SQUARE_X = 660;
	int BLUE_SQUARE_Y = 400;
	int GREEN_SQUARE_X = 680;
	int GREEN_SQUARE_Y = 400;
	int BLACK_SQUARE_X = 700;
	int BLACK_SQUARE_Y = 400;
	int YELLOW_SQUARE_X = 720;
	int YELLOW_SQUARE_Y = 400;

	*/
};

typedef Game TheGame;

#endif