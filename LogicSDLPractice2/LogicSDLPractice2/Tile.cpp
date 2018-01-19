#include <iostream>
#include <cstdlib> 
#include "Tile.h"
#include <string.h>
using namespace std;
Tile::Tile(void)
{
	cout << "Class Tile" << endl;

}
//----------------------------------------------STATE-----------------------------------------------------------------------
void Tile::setState(bool statePassed)
{
	if (statePassed)
		state = true;
	else
		state = false;

}
bool Tile::getState(void)
{
	return state;
}
void Tile::sortDirection(void)
{
	int i = 0;
	int NORTH = 0;int EAST = 0;int SOUTH = 0;int WEST = 0;
	char typestoCompare[][10] = { "AND","OR","XOR","NEG","WIRE_T", "WIRE_LU","WIRE_LD","WIRE_RU", 
		"WIRE_RD","WIRE_LR","WIRE_UD", "INPUT","OUTPUT","NULL" };
	for (i; i <= 13; i++)
	{
		if (strcmp(typestoCompare[i], type) == 0)
		{
			break;
		}
	}
	switch (i) //0 is NONE, 1 is Input, 2 is Output, 3 is both Input and Output
	{
	case 0:           //GATES
		NORTH = 1;
		EAST = 2;
		SOUTH = 1;
		WEST = 1;
		break;
	case 1:           
		NORTH = 1;
		EAST = 2;
		SOUTH = 1;
		WEST = 1;
		break;
	case 2:
		NORTH = 1;
		EAST = 2;
		SOUTH = 1;
		WEST = 1;
		break;
	case 3:
		NORTH = 1;
		EAST = 2;
		SOUTH = 1;
		WEST = 1;
		break;
	case 4:             //T
		NORTH = 3;
		EAST = 0;
		SOUTH = 3;
		WEST = 3;
		break;
	case 5:				//LU
		NORTH = 3;
		EAST = 0;
		SOUTH = 0;
		WEST = 3;
		break;
	case 6:				//LD
		NORTH = 0;
		EAST = 0;
		SOUTH = 3;
		WEST = 3;
		break;			//RU
	case 7:
		NORTH = 3;
		EAST = 3;
		SOUTH = 0;
		WEST = 0;
		break;
	case 8:				//RD
		NORTH = 0;
		EAST = 3;
		SOUTH = 3;
		WEST = 0;
		break;
	case 9:				//LR only works
		NORTH = 0;
		EAST = 3;
		SOUTH = 0;
		WEST = 3;
		break;
	case 10:			//UD
		NORTH = 3;
		EAST = 0;
		SOUTH = 3;
		WEST = 0;
		break;
	case 11:
		NORTH = 2;
		EAST = 2;
		SOUTH = 2;
		WEST = 2;
		break;
	case 12:
		NORTH = 1;
		EAST = 1;
		SOUTH = 1;
		WEST = 1;
		break;
	case 13:
		NORTH = 0;
		EAST = 0;
		SOUTH = 0;
		WEST = 0;
		break;
	}
	set_directionNorth(NORTH);
	set_directionEast(EAST);
	set_directionSouth(SOUTH);
	set_directionWest(WEST);

}

//----------------------------------------------TYPE-----------------------------------------------------------------------

void Tile::setType(char *setType)
{
		strcpy(type, setType);
		sortDirection();
}
char * Tile::getType(void)
{
	return type;
}
//----------------------------------------------COORDS-----------------------------------------------------------------------

void Tile::setX(int X)
{
	x = X;
}
void Tile::setY(int Y)
{
	y = Y;
}
int Tile::getX(void)
{
	return x;

}
int Tile::getY(void)
{
	return y;

}

//----------------------------------------------DIRECTIONS-------------------------------------------------------------------
void Tile::set_directionNorth(int NORTH)
{
	north = NORTH;
}
void Tile::set_directionEast(int EAST)
{
	east = EAST;
}
void Tile::set_directionSouth(int SOUTH)
{
	south = SOUTH;
}
void Tile::set_directionWest(int WEST)
{
	west = WEST;
}

int Tile::get_directionNorth(void)
{
return north;

}
int Tile::get_directionEast(void)
{
return east;

}
int Tile::get_directionSouth(void)
{
return south;

}
int Tile::get_directionWest(void)
{
return west;

}

//----------------------------------------------CurrentVals-------------------------------------------------------------------
void Tile::set_CurrentValN(bool val1)
{
	currentValN = val1;
}
void Tile::set_CurrentValS(bool val2)
{
	currentValS = val2;
}
void Tile::set_CurrentValW(bool val)
{
	currentValW = val;
}
void Tile::set_CurrentValOut(bool val)
{
	currentValOut = val;
}

bool Tile::get_CurrentValN(void)
{
	return currentValN;

}
bool Tile::get_CurrentValS(void)
{
	return currentValS;
}
bool Tile::get_CurrentValW(void)
{
	return currentValW;
}
bool Tile::get_CurrentValOut(void)
{
	return currentValOut;
}

//----------------------------------------------Processes-------------------------------------------------------------------

bool Tile::processAND(bool val1, bool val2)
{
	bool out = (val1 && val2);
	return out;
}
bool Tile::processOR(bool val1, bool val2)
{
	bool out = (val1 || val2);
	return out;
}
bool Tile::processXOR(bool val1, bool val2)
{
	bool out = ((val1 && (!val2)) || ((!val1) && val2));
	return out;
}
bool Tile::processNEG(bool val)
{
	bool out = !(val);
	return out;
}



