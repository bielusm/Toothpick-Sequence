#include "Toothpick.hpp"

	Toothpick::Toothpick(float x, float y, bool facingUp, float width)
	{
		this->x = x;
		this->y = y;
		this->facingUp = facingUp;
		this->width = width;
		if (facingUp)
		{
			leftFree = false;
				rightFree = false;
		}
		else
		{
			bottomFree = false;
			topFree = false;
		}
		noneFree = false;
	}
	Toothpick::Toothpick()
	{
	}
	bool Toothpick::getFacingUp()
	{
		return facingUp;
	}

	float Toothpick::getX()
	{
		return x;
	}
	float Toothpick::getY()
	{
		return y;
	}


	float Toothpick::getLeft()
	{
		if (!facingUp)
			return x - width;
		else
			return x;
	}
	float Toothpick::getRight()
	{
		if (!facingUp)
			return x + width;
		else
			return x;
	}

	float Toothpick::getTop()
	{
		if (facingUp)
			return y + width;
		else
			return y;


	}
	float Toothpick::getBottom()
	{
		if (facingUp)
			return y - width;
		else
			return y;
	}
	Toothpick::Dir Toothpick::freeSide()
	{
		if (noneFree)
			return Dir::none;
		if (leftFree)
			return Dir::left;
		if (rightFree)
			return Dir::right;
		if (topFree)
			return Dir::top;
		if (bottomFree)
			return Dir::bottom;
		return Dir::none;
	}
	void Toothpick::setCaptured(Dir dir)
	{
		switch (dir)
		{
		case Dir::left:
			leftFree = false;
			break;
		case Dir::right:
			rightFree = false;
			break;
		case Dir::top:
			topFree = false;
			break;
		case Dir::bottom:
			bottomFree = false;

			if (!leftFree && !rightFree && !topFree && !bottomFree)
				noneFree = true;
		}
	}
	void Toothpick::setCaptured(int dir)
	{
		switch (dir)
		{
		case 0:
			leftFree = false;
			break;
		case 1:
			rightFree = false;
			break;
		case 2:
			topFree = false;
			break;
		case 3:
			bottomFree = false;

			if (!leftFree && !rightFree && !topFree && !bottomFree)
				noneFree = true;
		}
	}