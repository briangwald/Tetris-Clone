#ifndef SQUARE_H
#define SQUARE_H

#include "Enums.h"
#include "Defines.h"
#include "Game.h"

class Square
{
public:

	Square(int centerX, int centerY, SDL_Surface* sprite, BlockType blockType);

	void Draw(SDL_Surface* screenSurface);

	void Move(Direction dir);

	bool CheckEntityCollision(Direction dir);

	bool CheckWallCollision(Direction dir);

	int GetCenterX() { return centerX; }
	int GetCenterY() { return centerY; }

	void SetCenterX(int x) { centerX = x; }
	void SetCenterY(int y) { centerY = y; }

private:

	int centerX;
	int centerY;

	BlockType blockType;

	SDL_Surface* sprite;

};

#endif