#ifndef EXIT_STATE_H
#define EXIT_STATE_H

#include <string>
#include <SDL.h>
#include "GameState.h"

class ExitState : public GameState
{
public:

	void HandleInput();

	void RunState();

private:

	SDL_Event event;
};

#endif