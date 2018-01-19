#pragma once
class Tile
{
private:
	bool state;
	int x;
	int y;
	char type[20];
	int north;
	int east;
	int south;
	int west;
	bool currentValN;
	bool currentValS;
	bool currentValW;
	bool currentValOut;
public:
	Tile(void);
/*
Which sides become active when a certain type is selected.
For example if an AND gate is chosen then only the East 
face becomes true because that is the only possible way incoming signals can be sent,
whereas as a wire can be sent and in 2(or even 3) directions.
*/
	void sortDirection(void);
	void set_directionNorth(int);
	void set_directionEast(int);
	void set_directionSouth(int);
	void set_directionWest(int);

	int get_directionNorth(void);
	int get_directionEast(void);
	int get_directionSouth(void);
	int get_directionWest(void);
			   
/*
The state determines whether the tile is 
active for the transmission of a signal
*/
	void setState(bool);
	bool getState(void);
/*
The type of the tile can be either an AND, OR,XNOR, 
NEG(negating gate), directional wires, input 
and output for delivering and obtaining/displaying 
a signal. The null tile is the default of the tiles 
and thus, can be used as an eraser for placed tiles.
*/
	void setType(char *);
	char * getType(void);
/*
The position of the tile is related to the coordinate
system of the picture itself and not SDL. So the coordinate is
in rows and columns.
*/
	void setX(int);
	void setY(int);

	int getX(void);
	int getY(void);

/*
2 Values are stored  for the use of gates so that the output
can be determined only when 2 outputs are read through 
the inputs sides.
VAl1 and VAl2 are inputs
ValOut is output
*/
	void set_CurrentValN(bool);
	void set_CurrentValS(bool);
	void set_CurrentValW(bool);

	void set_CurrentValOut(bool);

	bool get_CurrentValN(void);
	bool get_CurrentValS(void);
	bool get_CurrentValW(void);

	bool get_CurrentValOut(void);

/*
processes for the gates and returns the
outputVal for the tile
*/
	bool processAND(bool, bool);
	bool processOR(bool, bool);
	bool processXOR(bool, bool);
	bool processNEG(bool);
};