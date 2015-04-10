#include "MenuState.h"
#include "Defines.h"
#include "PlayState.h"
#include <SDL.h>
#include <iostream>

void MenuState::HandleInput()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			
			while (!TheGame::Instance()->GetGameStates()->empty())
			{
				cout << "popping from GameStates..." << endl;
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

			case SDLK_q:
				TheGame::Instance()->GetGameStates()->pop();
				break;

			case SDLK_g:
				PlayState* play = new PlayState;
				TheGame::Instance()->GetGameStates()->push(play);
				break;
			}
			return;
		}
		
	}
}

void MenuState::RunState()
{
	if (SDL_GetTicks() - TheGame::Instance()->GetTimer() >= FRAME_RATE)
	{
		HandleInput();

		TheGame::Instance()->ClearScreen();

		TheGame::Instance()->DisplayText("Start (G)ame", 120, 120, 12, 255, 255, 255, 0, 0, 0);
		TheGame::Instance()->DisplayText("(Q)uit Game", 120, 150, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(TheGame::Instance()->GetWindow());

		TheGame::Instance()->SetTimer(SDL_GetTicks());
	}
}