#include "Block.h"
#include "Game.h"

Block::Block(int x, int y, SDL_Surface* sprite, BlockType blockType) :
	centerX(x),
	centerY(y),
	blockType(blockType),
	sprite(sprite)
{
	for (int i = 0; i < 4; i++)
		squares[i] = NULL;

	SetupSquares(x, y, sprite);
}

Block::Block(Block &obj)
{
	centerX = obj.centerX;
	centerY = obj.centerY;

	blockType = obj.blockType;

	sprite = obj.sprite;

	SetupSquares(centerX, centerY, sprite);
}

void Block::SetupSquares(int x, int y, SDL_Surface* sprite)
{
	centerX = x;
	centerY = y;

	for (int i = 0; i < 4; i++)
	{
		if (squares[i])
			delete squares[i];
	}

	switch (blockType)
	{
	case SQUARE_BLOCK:
		/*
		[0][1]
		[2][3]
		*/
		squares[0] = new Square(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[3] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		break;

	case STRAIGHT_BLOCK:
		/*
		[0]
		[1]
		[2]
		[3]
		*/
		squares[0] = new Square(x + SQUARE_MEDIAN, y - (SQUARE_MEDIAN * 3), sprite, blockType);
		squares[1] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[3] = new Square(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), sprite, blockType);
		break;

	case T_BLOCK:
		/*
		   [0]
		[1][2][3]
		*/
		squares[0] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[3] = new Square(x + (SQUARE_MEDIAN * 3), y + SQUARE_MEDIAN, sprite, blockType);
		break;

	case L_BLOCK:
		/*
		[0]
		[1]
		[2][3]
		*/
		squares[0] = new Square(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x - SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), sprite, blockType);
		squares[3] = new Square(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), sprite, blockType);
		break;

	case L_BLOCK_BACKWARDS:
		/*
		   [0]
		   [1]
		[3][2]
		*/
		squares[0] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x + SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), sprite, blockType);
		squares[3] = new Square(x - SQUARE_MEDIAN, y + (SQUARE_MEDIAN * 3), sprite, blockType);
		break;

	case S_BLOCK:
		/*
		   [0][1]
		[2][3]
		*/
		squares[0] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x + (SQUARE_MEDIAN * 3), y - SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x - SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[3] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		break;

	case S_BLOCK_BACKWARDS:
		/*
		[0][1]
		   [2][3]
		*/
		squares[0] = new Square(x - SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[1] = new Square(x + SQUARE_MEDIAN, y - SQUARE_MEDIAN, sprite, blockType);
		squares[2] = new Square(x + SQUARE_MEDIAN, y + SQUARE_MEDIAN, sprite, blockType);
		squares[3] = new Square(x + (SQUARE_MEDIAN * 3), y + SQUARE_MEDIAN, sprite, blockType);
		break;
	}
}

void Block::Draw(SDL_Surface* screenSurface)
{
	for (int i = 0; i < 4; i++)
	{
		squares[i]->Draw(screenSurface);
	}
}

void Block::Move(Direction dir)
{
	switch (dir)
	{
		case LEFT:
			centerX -= SQUARE_MEDIAN * 2;
			break;

		case RIGHT:
			centerX += SQUARE_MEDIAN * 2;
			break;

		case DOWN:
			centerY += SQUARE_MEDIAN * 2;
			break;
	}

	for (int i = 0; i < 4; i++)
		squares[i]->Move(dir);
}

void Block::DestroyBlocks()
{
	for (int i = 0; i < 4; i++)
		delete squares[i];
}

void Block::Rotate()
{
	int x1, x2, y1, y2;

	for (int i = 0; i < 4; i++)
	{
		x1 = squares[i]->GetCenterX();
		y1 = squares[i]->GetCenterY();

		x1 -= centerX;
		y1 -= centerY;

		x2 = -y1;
		y2 = x1;

		x2 += centerX;
		y2 += centerY;

		squares[i]->SetCenterX(x2);
		squares[i]->SetCenterY(y2);
	}
}

Square** Block::GetSquares()
{
	return squares;
}

int* Block::GetRotatedSquares()
{
	int* temp = new int[8];

	int x1, y1, x2, y2;

	for (int i = 0; i < 4; i++)
	{
		x1 = squares[i]->GetCenterX();
		y1 = squares[i]->GetCenterY();

		x1 -= centerX;
		y1 -= centerY;

		x2 = -y1;
		y2 = x1;

		x2 += centerX;
		y2 += centerY;

		// x, y pairs. x at even indicies, y at odd
		temp[i * 2] = x2;
		temp[i * 2 + 1] = y2;

		//Returned value must be deleted once finished with it!
		
	}
	return temp;
}

bool Block::CheckEntityCollision(Direction dir)
{
	Square** temp = this->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		if (temp[i]->CheckEntityCollision(dir)) return true;
	}
	return false;
}

bool Block::CheckWallCollision(Direction dir)
{
	Square** temp = this->GetSquares();

	for (int i = 0; i < 4; i++)
	{
		if (temp[i]->CheckWallCollision(dir)) return true;
	}
	return false;
}

bool Block::CheckRotationCollision()
{
	//Must be deleted before returning
	int* temp = this->GetRotatedSquares();

	int distance = SQUARE_MEDIAN * 2;

	bool flag = false;

	vector<Square*> placedSquares = TheGame::Instance()->GetPlacedSquares();

	for (int i = 0; i < 4; i++)
	{
		if (temp[i * 2] < GAME_AREA_LEFT || temp[i * 2] > GAME_AREA_RIGHT)
		{
			flag = true;
		}
		if (temp[i * 2 + 1] > GAME_AREA_BOTTOM)
		{
			flag = true;
		}
		for (int j = 0; j < placedSquares.size(); j++)
		{
			if ((abs(temp[i * 2] - placedSquares[j]->GetCenterX()) < distance) && (abs(temp[i * 2 + 1] - placedSquares[j]->GetCenterY()) < distance))
			{
				flag = true;
			}
		}
	}
	delete temp;
	return flag;
}