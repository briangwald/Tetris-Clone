#ifndef Block_H
#define Block_H

#include "Square.h"
#include "Enums.h"

class Square;

class Block
{
	friend class Game;

public:

	Block(int x, int y, SDL_Surface* sprite, BlockType blocktype);

	Block(Block &obj);

	void SetupSquares(int x, int y, SDL_Surface* sprite);

	void Draw(SDL_Surface* screenSurface);

	void Move(Direction dir);

	void DestroyBlocks();

	void Rotate();

	Square** GetSquares();

	int* GetRotatedSquares();

	bool CheckEntityCollision(Direction dir);

	bool CheckWallCollision(Direction dir);

	bool CheckRotationCollision();

private:

	int centerX;
	int centerY;

	BlockType blockType;

	SDL_Surface* sprite;

	Square* squares[4];

};

#endif