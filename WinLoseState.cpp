#include "WinLoseState.h"
#include "Game.h"
#include "ExitState.h"
#include "MenuState.h"

void WinLoseState::HandleInput()
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
			if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_y)
			{
				TheGame::Instance()->GetGameStates()->pop();
				return;
			}
			if (event.key.keysym.sym == SDLK_n)
			{
				TheGame::Instance()->GetGameStates()->pop();

				ExitState* exit = new ExitState;
				TheGame::Instance()->GetGameStates()->push(exit);

				MenuState* menu = new MenuState;
				TheGame::Instance()->GetGameStates()->push(menu);
				return;
			}
		}
	}
}