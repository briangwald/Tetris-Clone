#include "PlayState.h"
#include "Game.h"
#include "Defines.h"
#include "Enums.h"
#include "Block.h"
#include <SDL.h>
#include <string>

void PlayState::HandleInput()
{
	static bool down_is_pressed = false;
	static bool left_is_pressed = false;
	static bool right_is_pressed = false;


	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			while (!TheGame::Instance()->GetGameStates()->empty())
			{
				TheGame::Instance()->GetGameStates()->pop();
			}
			return;
		}
		if (event.type == SDL_KEYDOWN)
		{
			
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				TheGame::Instance()->GetGameStates()->pop();

				return;
			}

			Block* nextBlock = TheGame::Instance()->GetNextBlock();

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				if (!nextBlock->CheckEntityCollision(LEFT) && !nextBlock->CheckEntityCollision(RIGHT) && !nextBlock->CheckEntityCollision(DOWN))
				{
					TheGame::Instance()->SwapBlock();
				}
			}

			Block* focusBlock = TheGame::Instance()->GetFocusBlock();

			if (event.key.keysym.sym == SDLK_UP)
			{
				if (!focusBlock->CheckRotationCollision())
				{
					focusBlock->Rotate();
				}
			}
			// TODO: Consolidate Wall and Entity Collision into a signle function for readability.

			if (event.key.keysym.sym == SDLK_LEFT)
			{
				left_is_pressed = true;
				if (!focusBlock->CheckWallCollision(LEFT) && !focusBlock->CheckEntityCollision(LEFT))
				{
					focusBlock->Move(LEFT);
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				right_is_pressed = true;
				if (!focusBlock->CheckWallCollision(RIGHT) && !focusBlock->CheckEntityCollision(RIGHT))
				{
					focusBlock->Move(RIGHT);
				}
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				down_is_pressed = true;
				if (!focusBlock->CheckWallCollision(DOWN) && !focusBlock->CheckEntityCollision(DOWN))
				{
					focusBlock->Move(DOWN);
				}
			}
		}
	}
}

void PlayState::RunState()
{
	static int forceDownCounter = 0;
	static int slideCounter = SLIDE_TIME;

	if (SDL_GetTicks() - TheGame::Instance()->GetTimer() >= FRAME_RATE)
	{
		this->HandleInput();

		forceDownCounter++;

		if (forceDownCounter >= TheGame::Instance()->GetFocusBlockSpeed())
		{
			if (!TheGame::Instance()->GetFocusBlock()->CheckWallCollision(DOWN) && !TheGame::Instance()->GetFocusBlock()->CheckEntityCollision(DOWN))
			{
				TheGame::Instance()->GetFocusBlock()->Move(DOWN);
				forceDownCounter = 0;
			}
		}

		if (TheGame::Instance()->GetFocusBlock()->CheckWallCollision(DOWN) || TheGame::Instance()->GetFocusBlock()->CheckEntityCollision(DOWN))
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
			TheGame::Instance()->HandleBottomCollision();
		}

		TheGame::Instance()->ClearScreen();

		TheGame::Instance()->DrawBackground();

		TheGame::Instance()->GetFocusBlock()->Draw(TheGame::Instance()->GetScreenSurface());
		TheGame::Instance()->GetNextBlock()->Draw(TheGame::Instance()->GetScreenSurface());

		for (int i = 0; i < TheGame::Instance()->GetPlacedSquares().size(); i++)
		{
			TheGame::Instance()->GetPlacedSquares()[i]->Draw(TheGame::Instance()->GetScreenSurface());
		}

		string scoreDisplay = "Score: ";
		string neededScoreDisplay = "Til Next Level: ";
		string levelDisplay = "Level: ";

		scoreDisplay = scoreDisplay.append(to_string(TheGame::Instance()->GetScore()));
		neededScoreDisplay = neededScoreDisplay.append(to_string(TheGame::Instance()->GetLevel() * POINTS_PER_LEVEL));
		levelDisplay = levelDisplay.append(to_string(TheGame::Instance()->GetLevel()));

		TheGame::Instance()->DisplayText(scoreDisplay, SCORE_RECT_X, SCORE_RECT_Y, 8, 0, 0, 0, 255, 255, 255);
		TheGame::Instance()->DisplayText(neededScoreDisplay, NEEDED_SCORE_RECT_X, NEEDED_SCORE_RECT_Y, 8, 0, 0, 0, 255, 255, 255);
		TheGame::Instance()->DisplayText(levelDisplay, LEVEL_RECT_X, LEVEL_RECT_Y, 8, 0, 0, 0, 255, 255, 255);

		SDL_UpdateWindowSurface(TheGame::Instance()->GetWindow());
		
		TheGame::Instance()->SetTimer(SDL_GetTicks());
	}
}
