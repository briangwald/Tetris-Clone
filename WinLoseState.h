#ifndef WIN_LOSE_STATE_H
#define WIN_LOSE_STATE_H

#include "GameState.h"
#include <SDL.h>

class WinLoseState : public GameState
{
public:

	void HandleInput();

	virtual void RunState() { return; }

private:

	SDL_Event event;
};

#endif