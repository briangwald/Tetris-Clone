#include "Square.h"

Square::Square(int centerX, int centerY, SDL_Surface* sprite, BlockType blockType) :
	centerX(centerX),
	centerY(centerY),
	sprite(sprite),
	blockType(blockType)
{
}

void Square::Draw(SDL_Surface* screenSurface)
{
	SDL_Rect source;
	SDL_Rect temp;

	switch (blockType)
	{
	case SQUARE_BLOCK:
		temp = { YELLOW_SQUARE_X, YELLOW_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case STRAIGHT_BLOCK:
		temp = { BLUE_SQUARE_X, BLUE_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case T_BLOCK:
		temp = { PURPLE_SQUARE_X, PURPLE_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case L_BLOCK:
		temp = { BLACK_SQUARE_X, BLACK_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case L_BLOCK_BACKWARDS:
		temp = { GREY_SQUARE_X, GREY_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case S_BLOCK:
		temp = { RED_SQUARE_X, RED_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;

	case S_BLOCK_BACKWARDS:
		temp = { GREEN_SQUARE_X, GREEN_SQUARE_Y, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };
		source = temp;
		break;
	}
	SDL_Rect destination = { centerX - SQUARE_MEDIAN, centerY - SQUARE_MEDIAN, SQUARE_MEDIAN * 2, SQUARE_MEDIAN * 2 };

	SDL_BlitSurface(sprite, &source, screenSurface, &destination);

}

void Square::Move(Direction dir)
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
}

bool Square::CheckEntityCollision(Direction dir)
{
	int distance = SQUARE_MEDIAN * 2;

	int centerX = this->GetCenterX();
	int centerY = this->GetCenterY();

	switch (dir)
	{
	case DOWN:
		centerY += distance;
		break;

	case LEFT:
		centerX -= distance;
		break;

	case RIGHT:
		centerX += distance;
		break;
	}
	
	vector<Square*> placedSquares = TheGame::Instance()->GetPlacedSquares();

	for (int i = 0; i < placedSquares.size(); i++)
	{
		if ((abs(centerX - placedSquares[i]->GetCenterX()) < distance) && (abs(centerY - placedSquares[i]->GetCenterY()) < distance))
		{
			return true;
		}
	}

	return false;
}

bool Square::CheckWallCollision(Direction dir)
{
	int centerX = this->GetCenterX();
	int centerY = this->GetCenterY();

	int distance = SQUARE_MEDIAN * 2;

	switch (dir)
	{
	case DOWN:
		if ((centerY + distance) > GAME_AREA_BOTTOM)
		{
			return true;
		}
		break;
	case LEFT:
		if ((centerX - distance) < GAME_AREA_LEFT)
		{
			return true;
		}
		break;

	case RIGHT:
		if ((centerX + distance) > GAME_AREA_RIGHT)
		{
			return true;
		}
		break;
	}

	return false;
}