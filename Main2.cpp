#include "SDL.h"
#include "SDL_TTF.h"

#include "Defines.h"
#include "Enums.h"
#include "Block.h"

#include <stack>
#include <iostream>
#include <vector>
#include <string>

#include "time.h"
#include "math.h"

using namespace std;

struct State
{
	void(*StatePointer)();
};

stack<State> GameStates;

SDL_Surface* spriteSheet =   NULL;
SDL_Surface* screenSurface = NULL;
SDL_Window* window =         NULL;

SDL_Event event;

Block* focusBlock = NULL;
Block* nextBlock  =	NULL;

vector<Square*> placedSquares;
int score = 0;
int level = 1;
int focusBlockSpeed = INITIAL_SPEED;

int timer;

void Menu();
void Game();
void Exit();

void Win();
void Lose();

void Init();
void Shutdown();

void DrawBackground();
void ClearScreen();
void DisplayText(string text, int x, int y, int size, int fR,
	int fG, int fB, int bR, int bG, int bB);

void HandleMenuInput();
void HandleGameInput();
void HandleExitInput();

void HandleWinLoseInput();

bool CheckEntityCollision(Square* square, Direction dir);
bool CheckWallCollision(Square* square, Direction dir);

bool CheckEntityCollision(Block* block, Direction dir);
bool CheckWallCollision(Block* square, Direction dir);

bool CheckRotationCollision(Block* block);

void CheckWin();
void CheckLoss();

void HandleBottomCollision();
void ChangeFocusBlock();
int CheckCompletedLines();

void Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	window = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		cout << "Failed to create window" << endl;
	}

	timer = SDL_GetTicks();

	spriteSheet = SDL_LoadBMP("Assets/FallingBlocks.bmp");
	if (spriteSheet == NULL)
	{
		cout << "Failed to load bitmap" << endl;
	}

	screenSurface = SDL_GetWindowSurface(window);

	srand(time(0));

	focusBlock = new Block(BLOCK_START_X, BLOCK_START_Y, spriteSheet,
		(BlockType)(rand() % 7));
	nextBlock = new Block(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, spriteSheet,
		(BlockType)(rand() % 7));

	State state;
	state.StatePointer = Exit;
	GameStates.push(state);

	state.StatePointer = Menu;
	GameStates.push(state);

	TTF_Init();
}

void Shutdown()
{
	Square** temp1 = focusBlock->GetSquares();
	Square** temp2 = nextBlock->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		delete temp1[i];
		delete temp2[i];
	}

	for (int i = 0; i < placedSquares.size(); i++)
	{
		delete placedSquares[i];
	}

	delete focusBlock;
	delete nextBlock;
	
	TTF_Quit();

	SDL_FreeSurface(spriteSheet);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Menu()
{
	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{
		HandleMenuInput();

		ClearScreen();

		DisplayText("Start (G)ame", 120, 120, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("(Q)uit Game", 120, 150, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(window);

		timer = SDL_GetTicks();
	}
}

void Game()
{
	static int forceDownCounter = 0;
	static int slideCounter = SLIDE_TIME;

	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{
		HandleGameInput();

		forceDownCounter++;

		if (forceDownCounter >= focusBlockSpeed)
		{
			if (!CheckWallCollision(focusBlock, DOWN) &&
				!CheckEntityCollision(focusBlock, DOWN))
			{
				focusBlock->Move(DOWN);
				forceDownCounter = 0;
			}
		}

		if (CheckWallCollision(focusBlock, DOWN) ||
			CheckEntityCollision(focusBlock, DOWN))
		{
			slideCounter--;
		}	
		else
		{
			slideCounter = SLIDE_TIME;
		}
			

		if (slideCounter == 0)
		{
			slideCounter = SLIDE_TIME;
			HandleBottomCollision();
		}

		ClearScreen();

		DrawBackground();

		focusBlock->Draw(screenSurface);
		nextBlock->Draw(screenSurface);

		for (int i = 0; i < placedSquares.size(); i++)
		{
			placedSquares[i]->Draw(screenSurface);
		}

		string scoreDisplay = "Score: ";
		string neededScoreDisplay = "Til Next Level: ";
		string levelDisplay = "Level: ";

		scoreDisplay = scoreDisplay.append(to_string(score));
		neededScoreDisplay = neededScoreDisplay.append(to_string(level * POINTS_PER_LEVEL));
		levelDisplay = levelDisplay.append(to_string(level));

		DisplayText(scoreDisplay, SCORE_RECT_X, SCORE_RECT_Y, 8, 0, 0, 0, 255, 255, 255);
		DisplayText(neededScoreDisplay, NEEDED_SCORE_RECT_X, NEEDED_SCORE_RECT_Y, 8, 0, 0, 0, 255, 255, 255);
		DisplayText(levelDisplay, LEVEL_RECT_X, LEVEL_RECT_Y, 8, 0, 0, 0, 255, 255, 255);

		SDL_UpdateWindowSurface(window);

		timer = SDL_GetTicks();
	}
}

void Exit()
{
	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{
		HandleExitInput();

		ClearScreen();

		DisplayText("Quit Game (Y or N)?", 100, 150, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(window);

		timer = SDL_GetTicks();
	}
}

void DrawBackground()
{
	SDL_Rect source;

	switch (level)
	{
	case 1:
		source = { LEVEL_ONE_X, LEVEL_ONE_Y, WINDOW_WIDTH, WINDOW_HEIGHT };
		break;

	case 2:
		source = { LEVEL_TWO_X, LEVEL_TWO_Y, WINDOW_WIDTH, WINDOW_HEIGHT };
		break;

	case 3:
		source = { LEVEL_THREE_X, LEVEL_THREE_Y, WINDOW_WIDTH, WINDOW_HEIGHT };
		break;

	case 4:
		source = { LEVEL_FOUR_X, LEVEL_FOUR_Y, WINDOW_WIDTH, WINDOW_HEIGHT };
		break;

	case 5:
		source = { LEVEL_FIVE_X, LEVEL_FIVE_Y, WINDOW_WIDTH, WINDOW_HEIGHT };
		break;
	}

	SDL_Rect destination = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	SDL_BlitSurface(spriteSheet, &source, screenSurface, &destination);
}

void ClearScreen()
{
	SDL_FillRect(screenSurface, 0, 0);
}

void DisplayText(string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB)
{
	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", size);

	SDL_Color fground = { fR, fG, fB };
	SDL_Color bground = { bR, bG, bB };

	SDL_Surface* tempSurface = TTF_RenderText_Shaded(font, text.c_str(), fground, bground);

	SDL_Rect destination = { x, y, 0, 0 };

	SDL_BlitSurface(tempSurface, NULL, screenSurface, &destination);

	SDL_FreeSurface(tempSurface);

	TTF_CloseFont(font);
}

void HandleMenuInput()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			while (!GameStates.empty())
			{
				GameStates.pop();
			}
			return;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					GameStates.pop();
					break;

				case SDLK_q:
					GameStates.pop();
					break;

				case SDLK_g:
					State tempState;
					tempState.StatePointer = Game;
					GameStates.push(tempState);
					break;
			}
			return;
		}
	}
}

void HandleGameInput()
{
	static bool down_is_pressed = false;
	static bool left_is_pressed = false;
	static bool right_is_pressed = false;

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			while (!GameStates.empty())
			{
				GameStates.pop();
			}
			return;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				GameStates.pop();

				return;
			}

			if (event.key.keysym.sym == SDLK_UP)
			{
				if (!CheckRotationCollision(focusBlock))
				{
					focusBlock->Rotate();
				}
			}

			if (event.key.keysym.sym == SDLK_LEFT)
			{
				left_is_pressed = true;
				if (!CheckWallCollision(focusBlock, LEFT) && 
					!CheckEntityCollision(focusBlock, LEFT))
				{
					focusBlock->Move(LEFT);
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				right_is_pressed = true;
				if (!CheckWallCollision(focusBlock, RIGHT) && 
					!CheckEntityCollision(focusBlock, RIGHT))
				{
					focusBlock->Move(RIGHT);
				}
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				down_is_pressed = true;
				if (!CheckWallCollision(focusBlock, DOWN) && 
					!CheckEntityCollision(focusBlock, DOWN))
				{
					focusBlock->Move(DOWN);
				}
			}
		}
	}
}

void HandleExitInput()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			while (!GameStates.empty())
			{
				GameStates.pop();
			}
			return;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				GameStates.pop();
				break;

			case SDLK_y:
				GameStates.pop();
				break;

			case SDLK_n:
				State tempState;
				tempState.StatePointer = Menu;
				GameStates.push(tempState);
				break;
			}
		}
	}
}

void GameWon()
{
	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{
		HandleWinLoseInput();

		ClearScreen();

		DisplayText("You Win!", 100, 120, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game (Y or N)?", 100, 140, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(window);

		timer = SDL_GetTicks();
	}
}

void GameLost()
{
	if (SDL_GetTicks() - timer >= FRAME_RATE)
	{
		

		ClearScreen();

		DisplayText("You Lose.", 100, 120, 12, 255, 255, 255, 0, 0, 0);
		DisplayText("Quit Game (Y or N)?", 100, 140, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(window);

		timer = SDL_GetTicks();

		HandleWinLoseInput();
	}
}

void HandleWinLoseInput()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			while (!GameStates.empty())
			{
				GameStates.pop();
			}
			return;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_y)
			{
				GameStates.pop();
				return;
			}
			if (event.key.keysym.sym == SDLK_n)
			{
				GameStates.pop();

				State temp;
				temp.StatePointer = Exit;
				GameStates.push(temp);

				temp.StatePointer = Menu;
				GameStates.push(temp);
				return;
			}
		}
	}
}

bool CheckEntityCollision(Square* square, Direction dir)
{
	int distance = SQUARE_MEDIAN * 2;

	int centerX = square->GetCenterX();
	int centerY = square->GetCenterY();

	switch (dir)
	{
		case DOWN:
			centerY += distance;
			break;
		
		case LEFT:
			centerX -= distance;
			break;

		case RIGHT:
			centerX += distance;
			break;
	}

	for (int i = 0; i < placedSquares.size(); i++)
	{
		if ((abs(centerX - placedSquares[i]->GetCenterX()) < distance) && (abs(centerY - placedSquares[i]->GetCenterY()) < distance))
		{
			return true;
		}
	}

	return false; 
}

bool CheckEntityCollision(Block* block, Direction dir)
{
	Square** temp = block->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		if (CheckEntityCollision(temp[i], dir)) return true;
	}
	return false;
}

bool CheckWallCollision(Square* square, Direction dir)
{
	int centerX = square->GetCenterX();
	int centerY = square->GetCenterY();

	int distance = SQUARE_MEDIAN * 2;

	switch (dir)
	{
		case DOWN:
			if ((centerY + distance) > GAME_AREA_BOTTOM)
			{
				return true;
			}
			break;
		case LEFT:
			if ((centerX - distance) < GAME_AREA_LEFT)
			{
				return true;
			}
			break;

		case RIGHT:
			if ((centerX + distance) > GAME_AREA_RIGHT)
			{
				return true;
			}

			break;
	}

	return false;
}

bool CheckWallCollision(Block* block, Direction dir)
{
	Square** temp = block->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		if (CheckWallCollision(temp[i], dir)) return true;
	}
	return false;
}

bool CheckRotationCollision(Block* block)
{
	//Must be deleted before returning
	int* temp = block->GetRotatedSquares();

	int distance = SQUARE_MEDIAN * 2;

	bool flag = false;

	for (int i = 0; i < 4; i++)
	{
		if (temp[i * 2] < GAME_AREA_LEFT || temp[i * 2] > GAME_AREA_RIGHT)
		{
			flag = true;
		}
		if (temp[i * 2 + 1] > GAME_AREA_BOTTOM)
		{
			flag = true;
		}
		for (int j = 0; j < placedSquares.size(); j++)
		{
			if ((abs(temp[i * 2] - placedSquares[j]->GetCenterX()) < distance) && (abs(temp[i * 2 + 1] - placedSquares[j]->GetCenterY()) < distance))
			{
				flag = true;
			}
		}
	}
	delete temp;
	return flag;
}

void HandleBottomCollision()
{
	ChangeFocusBlock();

	int num_lines = CheckCompletedLines();

	if (num_lines > 0)
	{

		score += POINTS_PER_LINE * num_lines;

		if (score >= level * POINTS_PER_LEVEL)
		{
			level++;
			CheckWin();
			focusBlockSpeed -= SPEED_CHANGE;
		}
	}
	CheckLoss();
}

void ChangeFocusBlock()
{
	Square** focusBlockSquares = focusBlock->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		placedSquares.push_back(focusBlockSquares[i]);
	}
	
	delete focusBlock;

	focusBlock = nextBlock;

	focusBlock->SetupSquares(BLOCK_START_X, BLOCK_START_Y, spriteSheet);

	nextBlock = new Block(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, spriteSheet, (BlockType)(rand() % 7));
}

int CheckCompletedLines()
{
	int squares_per_row[13] = { };

	int row_size = SQUARE_MEDIAN * 2;
	int bottom = GAME_AREA_BOTTOM - SQUARE_MEDIAN;
	int top = bottom - (12 * row_size);

	int num_lines = 0;
	int row;

	for (int i = 0; i < placedSquares.size(); i++)
	{
		row = (placedSquares[i]->GetCenterY() - top) / row_size;

		squares_per_row[row]++;
	}

	for (int line = 0; line < 13; line++)
	{
		if (squares_per_row[line] == SQUARES_PER_ROW)
		{
			num_lines++;

			for (int index = 0; index < placedSquares.size(); index++)
			{
				if (((placedSquares[index]->GetCenterY() - top) / row_size) == line)
				{
					delete placedSquares[index];

					placedSquares.erase(placedSquares.begin() + index);

					index--;
					
				}
			}
		}
	}

	for (int i = 0; i < placedSquares.size(); i++)
	{
		for (int line = 0; line < 13; line++)
		{
			if (squares_per_row[line] == SQUARES_PER_ROW)
			{
				row = (placedSquares[i]->GetCenterY() - top) / row_size;

				if (row < line) placedSquares[i]->Move(DOWN);
			}
		}
	}

	return num_lines;
}

void CheckWin()
{
	if (level > NUM_LEVELS)
	{
		while (!GameStates.empty())
		{
			GameStates.pop();
		}

		State win;
		win.StatePointer = GameWon;

		GameStates.push(win);
	}
}

void CheckLoss()
{
	if (CheckEntityCollision(focusBlock, DOWN))
	{
		for (int i = 0; i < placedSquares.size(); i++)
		{
			delete placedSquares[i];
		}
		placedSquares.clear();
		
		while (!GameStates.empty())
		{
			GameStates.pop();
		}

		State lose;
		lose.StatePointer = GameLost;

		GameStates.push(lose);
	}
}


int main(int argc, char** argv)
{
	Init();

	while (!GameStates.empty())
	{
		GameStates.top().StatePointer();
	}

	Shutdown();

	return 0;
}