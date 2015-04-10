#include "ExitState.h"
#include "Defines.h"
#include "Game.h"
#include "MenuState.h"

void ExitState::HandleInput()
{
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
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->GetGameStates()->pop();
				break;

			case SDLK_y:
				TheGame::Instance()->GetGameStates()->pop();
				break;

			case SDLK_n:
				MenuState* menu = new MenuState;
				TheGame::Instance()->GetGameStates()->push(menu);
				break;
			}
		}
	}
}

void ExitState::RunState()
{
	if (SDL_GetTicks() - TheGame::Instance()->GetTimer() >= FRAME_RATE)
	{
		HandleInput();

		TheGame::Instance()->ClearScreen();

		TheGame::Instance()->DisplayText("Quit Game (Y or N)?", 100, 150, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(TheGame::Instance()->GetWindow());

		TheGame::Instance()->SetTimer(SDL_GetTicks());
	}
}