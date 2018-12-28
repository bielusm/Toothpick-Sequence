#include "world.h"

World::World(float pickWidth, float pickOffset, int maxGen)
{
	N = 1;
	this->maxGen = maxGen;
	this->pickWidth = pickWidth;
	picks.push_back(Toothpick(0.0f, 0.0f, false, pickWidth));
}

void World::update()
{
	if (N < maxGen)
	{
		size_t size = picks.size();
		for (size_t j = 0; j < size; j++)
		{
			Toothpick::Dir dir = picks[j].freeSide();
			while (dir != Toothpick::Dir::none)
			{
				Toothpick temp;
				switch (dir)
				{
				case Toothpick::Dir::left:
					temp = Toothpick(picks[j].getLeft() - pickOffset, picks[j].getTop(), true, pickWidth);
					break;
				case Toothpick::Dir::right:
					temp = Toothpick(picks[j].getRight() + pickOffset, picks[j].getTop(), true, pickWidth);
					break;
				case Toothpick::Dir::top:
					temp = Toothpick(picks[j].getLeft(), picks[j].getTop() + pickOffset, false, pickWidth);
					break;
				case Toothpick::Dir::bottom:
					temp = Toothpick(picks[j].getLeft(), picks[j].getBottom() - pickOffset, false, pickWidth);
					break;
				}
				picks[j].setCaptured(dir);
				dir = picks[j].freeSide();
				//if (noPerpPicks(temp))
				{
					picks.push_back(temp);
					setSides(&picks[picks.size() - 1]);
				}
			}
		}
		for (int i = size; i > 0; i--)
		{
			deadPicks.push_back(picks[0]);
			picks.erase(picks.begin());
		}
					N++;
	}
}

void World::setSides(Toothpick *pick)
{
	if (pick->getFacingUp())
	{
		float top = pick->getTop();
		float bottom = pick->getBottom();
		float plane = pick->getLeft();
		float top2, bottom2, plane2;
		for (size_t i = 0; i < picks.size(); i++)
		{
			top2 = picks[i].getTop();
			bottom2 = picks[i].getBottom();
			plane2 = picks[i].getLeft();
			bool samePlane = (abs(plane - plane2) < 0.1);
			bool topEqual = (abs(top2 - bottom) < 0.1);
			bool bottomEqual = (abs(bottom2 - top) < 0.1);
			if (samePlane && (topEqual || bottomEqual))
			{
				if (!topEqual)
				{
					picks[i].setCaptured(3);
					pick->setCaptured(2);
				}
				else
				{
					picks[i].setCaptured(2);
					pick->setCaptured(3);

				}

			}
		}
	}
	else
	{
		float left = pick->getLeft();
		float right = pick->getRight();
		float plane = pick->getTop();
		float left2, right2, plane2;
		for (size_t i = 0; i < picks.size(); i++)
		{
			left2 = picks[i].getLeft();
			right2 = picks[i].getRight();
			plane2 = picks[i].getTop();
			bool samePlane = (abs(plane - plane2) < 0.1);
			bool leftEqual = (abs(left2 - right) < 0.1);
			bool rightEqual = (abs(right2 - left) < 0.1);
			if (samePlane && (leftEqual || rightEqual))
			{
				if (leftEqual)
				{
					picks[i].setCaptured(0);
					pick->setCaptured(1);
				}
				else
				{
					picks[i].setCaptured(1);
					pick->setCaptured(0);
				}
			}
		}
	}
}

