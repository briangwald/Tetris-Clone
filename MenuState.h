#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <string>

#include "Game.h"
#include "GameState.h"

class MenuState : public GameState
{
public:

	void HandleInput();

	void RunState();

private:

	SDL_Event event;
};

#endif