#include "Game.h"
#include "Defines.h"
#include "Enums.h"
#include "LoseState.h"
#include "ExitState.h"
#include "Block.h"
#include "WinState.h"
#include "MenuState.h"

Game::Game()
{
	level = 1;
	score = 0;

	spriteSheet = NULL;
	screenSurface = NULL;
	window = NULL;

	focusBlockSpeed = 60;
}

void Game::Init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	window = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		cout << "Failed to create window" << endl;
	}

	timer = SDL_GetTicks();

	spriteSheet = SDL_LoadBMP("Assets/Fibonacci.bmp");
	if (spriteSheet == NULL)
	{
		cout << "Failed to load bitmap" << endl;
	}

	screenSurface = SDL_GetWindowSurface(window);

	srand(time(0));

	focusBlock = new Block(BLOCK_START_X, BLOCK_START_Y, spriteSheet, (BlockType)(rand() % 7));
	nextBlock = new Block(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, spriteSheet, (BlockType)(rand() % 7));

	ExitState* exit = new ExitState;
	GameStates.push(exit);

	MenuState* menu = new MenuState;
	GameStates.push(menu);

	TTF_Init();
}

void Game::Shutdown()
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

void Game::DrawBackground()
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

void Game::ClearScreen()
{
	SDL_FillRect(screenSurface, 0, 0);
}

void Game::DisplayText(string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB)
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


void Game::HandleBottomCollision()
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

void Game::ChangeFocusBlock()
{
	Square** focusBlockSquares = focusBlock->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		placedSquares.push_back(focusBlockSquares[i]);
	}

	//delete focusBlock;

	focusBlock = nextBlock;

	focusBlock->SetupSquares(BLOCK_START_X, BLOCK_START_Y, spriteSheet);

	nextBlock = new Block(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, spriteSheet, (BlockType)(rand() % 7));
}

void Game::SwapBlock()
{
	Block* temp = focusBlock;
	focusBlock->DestroyBlocks();
	focusBlock = nextBlock;
	nextBlock->DestroyBlocks();
	nextBlock = temp;

}

int Game::CheckCompletedLines()
{
	int squares_per_row[13] = {};

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

void Game::CheckWin()
{
	if (level > NUM_LEVELS)
	{
		while (!GameStates.empty())
		{
			GameStates.pop();
		}

		WinState* win = new WinState;
		GameStates.push(win);
	}
}

void Game::CheckLoss()
{
	if (focusBlock->CheckEntityCollision(DOWN))
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

		LoseState* lose = new LoseState;
		GameStates.push(lose);
	}
}

Game* Game::instance = NULL;