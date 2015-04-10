#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
public:
	
	virtual void HandleInput() = 0;

	virtual void RunState() = 0;

};

#endif