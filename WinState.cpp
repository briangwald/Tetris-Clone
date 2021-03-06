#include "WinState.h"
#include "Game.h"
#include "Defines.h"

void WinState::RunState()
{
	if (SDL_GetTicks() - TheGame::Instance()->GetTimer() >= FRAME_RATE)
	{
		HandleInput();

		TheGame::Instance()->ClearScreen();

		TheGame::Instance()->DisplayText("You Win!", 100, 120, 12, 255, 255, 255, 0, 0, 0);
		TheGame::Instance()->DisplayText("Quit Game (Y or N)?", 100, 140, 12, 255, 255, 255, 0, 0, 0);

		SDL_UpdateWindowSurface(TheGame::Instance()->GetWindow());

		TheGame::Instance()->SetTimer(SDL_GetTicks());
	}
}