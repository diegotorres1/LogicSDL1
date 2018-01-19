/*
_TILE CIRCUITS_

name of the program is Tile Circuits
-the aim is practice with logic design in circuitry
-Get tiles from TOOLS, place them on the MAIN GRID, press START to intiate circuit,STOP to reset circuit
	press SAVE to save layout, and LOAD to load last saved layout
-For future reference this program contains use of structs, 
	implementing pictures, text, examples of passing pointers,and classes
*/
#include <SDL.h>        
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SDL_ttf.h>
#include <cstring>
#include <string>//removed the .h !!
#include <cstdlib> 
#include <time.h>
#include "Tile.h"
//rows = 18 and col = 19, remember indexes
using namespace std;//c++
//global variables
extern bool showGateSelector = false;
extern bool showInstructions = false;
//------------------------------------------------------DATA STRUCTURES-------------------------------------------------------------

typedef struct rowscols
{
	int row;
	int col;
}rowscols;
rowscols RowsAndCols = {0,0};
typedef struct TileCoord
{
	int xUL; int yUL;
	int xLR; int yLR;
}TileCoord;

typedef struct Rect
{
	SDL_Rect GateSelector_rect;
}Rect;
typedef struct Mouse
{
	int x;
	int y;
	bool state;
}Mouse;
Mouse myMouse = { 0,0,false };
Rect rect = { { 0, 300,800,300 } }; //x,y,w,h

//------------------------------------------------------ALL FUNCTIONS INCLUDING MAIN------------------------------------------------------

//------------------------------------------------------CROP SPRITE SHEET
SDL_Surface* crop_surface(SDL_Surface* GateSelectorimg, int x, int y, int width, int height)
{
	SDL_Surface* surface = SDL_CreateRGBSurface(GateSelectorimg->flags, width, height, GateSelectorimg->format->BitsPerPixel, GateSelectorimg->format->Rmask, GateSelectorimg->format->Gmask, GateSelectorimg->format->Bmask, GateSelectorimg->format->Amask);
	SDL_Rect rect = { x, y, width, height };
	SDL_BlitSurface(GateSelectorimg, &rect, surface, 0);
	return surface;
}
//------------------------------------------------------SAVE
void saveTileConfig(Tile (&tilesObject)[18][19])
{
	ofstream file("temp.txt");//edit
	if (file.is_open() && file.good())
	{
		cout << "Save OpenFile Successful" << endl;
		for (int colTiles1 = 0; colTiles1 <= 18; colTiles1++)
			for (int rowTiles1 = 0; rowTiles1 <= 17; rowTiles1++)
			{
				file << (tilesObject[rowTiles1][colTiles1]).getType() << "\n";
			}
		file.close();
	}
	else
		cout << "Save OpenFile Failed!!!" << endl;

}
//------------------------------------------------------LOAD
void loadTileConfig(Tile (&tilesObject)[18][19])
{
	string line;
	char store[342][10];//amount by length 2 dimensional array
	int counter = 0;
	ifstream file("temp.txt");//edit
	if (file.is_open() && file.good())
	{
		cout << "Load OpenFile Successful" << endl;
		while (getline(file, line))//get into char
		{
			strncpy(store[counter], line.c_str(), sizeof(store[counter]));
			counter++;
		}
		for (int colTiles1 = 0; colTiles1 <= 18; colTiles1++)
			for (int rowTiles1 = 0; rowTiles1 <= 17; rowTiles1++)
			{
				(tilesObject[rowTiles1][colTiles1]).setType(store[((rowTiles1 * 19) + colTiles1)]);
			}
		file.close();
	}
	else
		cout << "Load OpenFile Failed!!!" << endl;
}
//------------------------------------------------------PROCESS TILE INFO
bool processTileInformation(Tile (&tilesObject)[18][19],int row1,int col1,int row2,int col2)
{
	int counterAmount = 0;
	bool Val1 = 0; bool Val2 = 0;
	char typestoCompare[][10] = { "AND","OR","XOR","NEG","WIRE_T", "WIRE_LU","WIRE_LD","WIRE_RU",
		"WIRE_RD","WIRE_LR","WIRE_UD", "INPUT","OUTPUT","NULL" };
	for (int k = 0; k <= 2; k++) {//FOR LOOP TO CHECK GATE OR NOT
		if (strcmp(typestoCompare[k], (tilesObject[row2][col2]).getType()) == 0) {//TO CHECK IF GATE
			if ((tilesObject[row2 - 1][col2]).getState()) {//N
				counterAmount++;
				(tilesObject[row2][col2]).set_CurrentValN((tilesObject[row1][col1]).get_CurrentValOut());
			}
			if ((tilesObject[row2][col2 - 1]).getState()) {//W
				counterAmount++;
				(tilesObject[row2][col2]).set_CurrentValW((tilesObject[row1][col1]).get_CurrentValOut());
			}
			if ((tilesObject[row2 + 1][col2]).getState()) {//S
				counterAmount++;
				(tilesObject[row2][col2]).set_CurrentValS((tilesObject[row1][col1]).get_CurrentValOut());
			}


			if (counterAmount == 2) {               //Fundamentals for gates!!

				if ((tilesObject[row2 - 1][col2]).getState() && (tilesObject[row2][col2 - 1]).getState())//North and West
				{
					Val1 = (tilesObject[row2 - 1][col2]).get_CurrentValOut();
					Val2 = (tilesObject[row2][col2 - 1]).get_CurrentValOut();
				}
				if ((tilesObject[row2 - 1][col2]).getState() && (tilesObject[row2 + 1][col2]).getState())//North and South
				{
					Val1 = (tilesObject[row2 - 1][col2]).get_CurrentValOut();
					Val2 = (tilesObject[row2 + 1][col2]).get_CurrentValOut();
				}
				if ((tilesObject[row2][col2 - 1]).getState() && (tilesObject[row2 + 1][col2]).getState())//West and South
				{
					Val1 = (tilesObject[row2][col2 - 1]).get_CurrentValOut();
					Val2 = (tilesObject[row2 + 1][col2]).get_CurrentValOut();
				}
				/*int g = 0;
				for (int g = 0; g <= 13; g++) {
					if (strcmp(typestoCompare[g], (tilesObject[row2][col2]).getType()) == 0)
					{
						break;
					}
				}
				Supplementary For Loop
				*/

				switch (k)//Determines Which functions are chosen for the gate
				{
				case 0:
					(tilesObject[row2][col2]).set_CurrentValOut((tilesObject[row2][col2]).processAND(Val1, Val2));
					break;
				case 1:
					(tilesObject[row2][col2]).set_CurrentValOut((tilesObject[row2][col2]).processOR(Val1, Val2));
					break;
				case 2:
					(tilesObject[row2][col2]).set_CurrentValOut((tilesObject[row2][col2]).processXOR(Val1, Val2));
					break;
				}
				return true;
			}
			if(k == 2) {
				return false;
			}
		}
		else {//ELSE NOT A GATE,or is a NEG GATE
			if (k == 2) {
				(tilesObject[row2][col2]).set_CurrentValN((tilesObject[row1][col1]).get_CurrentValOut());
				if (strcmp(typestoCompare[3], (tilesObject[row2][col2]).getType()) == 0)
				{
					(tilesObject[row2][col2]).set_CurrentValOut((tilesObject[row2][col2]).processNEG((tilesObject[row2][col2]).get_CurrentValN()));
				}
				else
					(tilesObject[row2][col2]).set_CurrentValOut((tilesObject[row2][col2]).get_CurrentValN());
				return true;
			}
		}

	}

}
//------------------------------------------------------ACTIVATE NEARBY TILES
void activateTile(Tile (&tilesObject)[18][19],int row,int col)
{
	if(row>0)//Checks to make sure index is within bounds
		if (((tilesObject[row+1][col]).get_directionNorth() == 1 || (tilesObject[row+1][col]).get_directionNorth() == 3) && !((tilesObject[row][col]).get_directionSouth() == 1))
		{
			if (processTileInformation(tilesObject, row, col, row+1, col))
			{
				(tilesObject[row + 1][col]).setState(true);
			}
		}
	if(col>0)//Checks to make sure index is within bounds
		if (((tilesObject[row][col-1]).get_directionEast() == 1 || (tilesObject[row][col-1]).get_directionEast() == 3) && !((tilesObject[row][col]).get_directionWest() == 1))
		{
			if (processTileInformation(tilesObject, row, col, row, col -1 ))
			{
				(tilesObject[row][col -1 ]).setState(true);
			}
		}
	if(row<17)//Checks to make sure index is within bounds
		if (((tilesObject[row-1][col]).get_directionSouth() == 1 || (tilesObject[row-1][col]).get_directionSouth() == 3) && !((tilesObject[row][col]).get_directionNorth() == 1))
		{
			if (processTileInformation(tilesObject, row, col, row-1, col))
			{
				(tilesObject[row - 1][col]).setState(true);
			}

		}
	if(col<18)//Checks to make sure index is within bounds
		if (((tilesObject[row][col+1]).get_directionWest() == 1 || (tilesObject[row][col + 1]).get_directionWest() == 3) && !((tilesObject[row][col]).get_directionEast() == 1))
		{
			if (processTileInformation(tilesObject,row,col,row,col+1)) 
			{
				(tilesObject[row][col + 1]).setState(true);
			}
		}
}
//------------------------------------------------------START CIRCUIT 100 PASSES
void start(Tile (&tilesObject)[18][19])
{
	//Checking and setting states
	//1st sub-pass check Inputs
	//2nd sub-pass check states 
	//keep refreshing until all tiles are at state 1 thus activated
	char typestoCompare[][10] = { "AND","OR","XOR","NEG","WIRE_T", "WIRE_LU","WIRE_LD","WIRE_RU",
		"WIRE_RD","WIRE_LR","WIRE_UD", "INPUT","OUTPUT","NULL" };
	for (int colTiles1 = 0; colTiles1 <= 18; colTiles1++)
		for (int rowTiles1 = 0; rowTiles1 <= 17; rowTiles1++)
		{
			if (strcmp((tilesObject[rowTiles1][colTiles1]).getType(), typestoCompare[11]) == 0)
			{
				if ((tilesObject[rowTiles1][colTiles1]).getState() == false) {//Ensures that INPUT tiles are not reactivated

					(tilesObject[rowTiles1][colTiles1]).setState(true);
					(tilesObject[rowTiles1][colTiles1]).set_CurrentValN(true);
					(tilesObject[rowTiles1][colTiles1]).set_CurrentValS(true);
					(tilesObject[rowTiles1][colTiles1]).set_CurrentValW(true);
					(tilesObject[rowTiles1][colTiles1]).set_CurrentValOut(true);

					activateTile(tilesObject, rowTiles1, colTiles1);
				}

			}
		}
	for (int j = 0; j < 100; j++)
	{
		for (int colTiles2 = 0; colTiles2 <= 18; colTiles2++)
			for (int rowTiles2 = 0; rowTiles2 <= 17; rowTiles2++)
			{
				if ((tilesObject[rowTiles2][colTiles2]).getState() == true) {//Ensures that INPUT tiles are not reactivated
					activateTile(tilesObject, rowTiles2, colTiles2);
				}
			}
	}



}
//------------------------------------------------------STOP CLEARS CURRENT STATE AND VALS
void stop(Tile(&tilesObject)[18][19])
{
	for (int row = 0; row <= 17; row++)
		for (int col = 0; col <= 18; col++)
		{
			(tilesObject[row][col]).setState(false);
			(tilesObject[row][col]).set_CurrentValN(false);
			(tilesObject[row][col]).set_CurrentValS(false);
			(tilesObject[row][col]).set_CurrentValW(false);
			(tilesObject[row][col]).set_CurrentValOut(false);

		}
}
//------------------------------------------------------DISPLAY TILES
void displayTile(SDL_Renderer* renderer, TileCoord(&tileCoordMainWindow)[18][19], Tile(&tilesObject)[18][19],
	SDL_Texture* SelectorsTex[])
{
	//Continue after including other get postion functions
	//GO through eachtile object
	//Check type and then set counter to corresponding sprite Tex
	//set current location relative to the row and col
	//Repeats Updating and checks type
	int counter = 0;
	SDL_Rect rect;
	char typestoCompare[][10] = { "AND","OR","XOR","NEG","WIRE_T", "WIRE_LU","WIRE_LD","WIRE_RU", "WIRE_RD","WIRE_LR","WIRE_UD", "INPUT","OUTPUT","NULL" };
	for (int row = 0; row <= 17; row++)
		for (int col = 0; col <= 18; col++)
		{
			if (strcmp(typestoCompare[13],(tilesObject[row][col]).getType()) != 0)
			{
				if (strcmp(typestoCompare[0], (tilesObject[row][col]).getType()) == 0)
					counter = 0;
				if (strcmp(typestoCompare[1], (tilesObject[row][col]).getType()) == 0)
					counter = 1;
				if (strcmp(typestoCompare[2], (tilesObject[row][col]).getType()) == 0)
					counter = 2;
				if (strcmp(typestoCompare[3], (tilesObject[row][col]).getType()) == 0)
					counter = 3;
				if (strcmp(typestoCompare[4], (tilesObject[row][col]).getType()) == 0)
					counter = 4;
				if (strcmp(typestoCompare[5], (tilesObject[row][col]).getType()) == 0)
					counter = 5;
				if (strcmp(typestoCompare[6], (tilesObject[row][col]).getType()) == 0)
					counter = 6;
				if (strcmp(typestoCompare[7], (tilesObject[row][col]).getType()) == 0)
					counter =7;
				if (strcmp(typestoCompare[8], (tilesObject[row][col]).getType()) == 0)
					counter = 8;
				if (strcmp(typestoCompare[9], (tilesObject[row][col]).getType()) == 0)
					counter = 9;
				if (strcmp(typestoCompare[10], (tilesObject[row][col]).getType()) == 0)
					counter = 10;
				if (strcmp(typestoCompare[11], (tilesObject[row][col]).getType()) == 0)
					counter = 11;
				if (strcmp(typestoCompare[12], (tilesObject[row][col]).getType()) == 0)
					counter = 12;
			}
			else
				counter = 13;
			rect = { 1 + (col * 32),1 + (row * 32),30,30 };
			SDL_RenderCopy(renderer, SelectorsTex[counter], NULL, &rect);

		}
}
//------------------------------------------------------CREATE BASIC TILE OUTLINE TYPE AND LOCATION
void createTile(TileCoord(&tileCoordMainWindow)[18][19], char type[], int row, int col, Tile(&tilesObject)[18][19])
{
	//Called from buttonHandler to create tile once clicked on grid
	cout << "CreateTile::SentType" << type << endl;
	(tilesObject[row][col]).setType(type);
	(tilesObject[row][col]).setX(row);
	(tilesObject[row][col]).setY(col);
	cout << "CreateTile::GetType" << (tilesObject[row][col]).getType() << endl;


}
//------------------------------------------------------SELECTOR FOR CURRENT TYPE
char* selector(int tileNumber)
{
	//sets currentType when the tool menu is selected accoring to..
	//number in list in array 
	string typeOf;
	switch (tileNumber)
	{
	case 0:
		typeOf = "AND";
		break;
	case 1:
		typeOf = "OR";
		break;
	case 2:
		typeOf = "XOR";
		break;
	case 3:
		typeOf = "NEG";
		break;
	case 4:
		typeOf = "WIRE_T";
		break;
	case 5:
		typeOf = "WIRE_LU"; //LEFTup
		break;
	case 6:
		typeOf = "WIRE_LD"; //LEFTdown
		break;
	case 7:
		typeOf = "WIRE_RU"; //RIGHTup
		break;
	case 8:
		typeOf = "WIRE_RD";//RIGHTdown
		break;
	case 9:
		typeOf = "WIRE_LR";//LEFTright
		break;
	case 10:
		typeOf = "WIRE_UD";//UPdown
		break;
	case 11:
		typeOf = "INPUT";
		break;
	case 12:
		typeOf = "OUTPUT";
		break;
	case 13:
		typeOf = "NULL";
		break;
	}
	char * cstr = new char[typeOf.length() + 1];
	if (strlen(cstr) >= strlen(typeOf.c_str()))
		strcpy(cstr, typeOf.c_str());
	char * p = strtok(cstr, " ");
	return p;

}
//------------------------------------------------------BUTTON HANDLER
void buttonHandler(TileCoord  tileCoords[], TileCoord(&tileCoordMainWindow)[18][19], Tile(&tilesObject)[18][19])
{
	//Get Coordinates of Mouse through myMouse struct
	int x = myMouse.x;
	int y = myMouse.y;
	static SDL_Point mouse_position;
	cout << "x: " << x << endl;
	cout << "y: " << y << endl;
	//currentType for identifying selected tile in Tool Menu
	static char currentType[20];
	//shows whether or not Menu should appear depending on state change of mouse..
	//as defined NOT BY ACTUAL state, but from my definition of state
	//defined in mouseHandler function
	if (myMouse.state)
	{
		if(!showInstructions)						//Show instructions -determine whether
			if (x > 665 && x < 740 && y>175 && y < 215)
			{
				if (showGateSelector)
					showGateSelector = false;
				else
					showGateSelector = true;

			}
		if(!showGateSelector)						//Show Gate Selector -determine whether
			if (x > 750 && x < 795 && y>550 && y < 595) {
				showInstructions = true;

		}
	}
	//If showGateSelector is true
	if (showGateSelector)
	{
		//locate which tile is selected based on coords
		for (int i = 0; i <= 13; i++)
		{
			if (x > tileCoords[i].xUL &&x<tileCoords[i].xLR && y>tileCoords[i].yUL &&y < tileCoords[i].yLR)
			{
				strcpy(currentType, selector(i));
			}
		}
		if (x > 770 && x < 800 && y>300 && y < 362)
			showGateSelector = false;

	}
	//If show Instructions is True
	else if (showInstructions) {
		if(x>742 && x<800 && y>0 && y<33)
			showInstructions = false;
	}
	//If neither Show Instructions or showGateSelector is true
	else
	{
		//1st IF checks to make sure left click mouse button action is done
		if (SDL_GetMouseState(&(mouse_position.x), &(mouse_position.y)) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (int row = 0; row <= 17; row++)
				for (int col = 0; col <= 18; col++)
					if (x > (tileCoordMainWindow[row][col]).xUL &&x<tileCoordMainWindow[row][col].xLR
						&& y>tileCoordMainWindow[row][col].yUL &&y < tileCoordMainWindow[row][col].yLR)
					{
						cout << "row: " << row << "\ncol: " << col << endl;
						createTile(tileCoordMainWindow, currentType, row, col, tilesObject);
					}
			if (x > 630 && x < 780 && y>20 && y < 105) //START button check
			{
				cout << "Start" << endl;
				start(tilesObject);
			}
			if (x > 665 && x < 740 && y>110 && y < 155) //STOP ie CLEAR  button check
			{
				cout << "STOP" << endl;
				stop(tilesObject);
			}
			if (x>760 && x<785 && y>340 && y<365) //Save button check
			{
				stop(tilesObject);
				saveTileConfig(tilesObject);
			}
			
			if (x>760 && x<785 && y>375 && y<400) //Load button check
			{
				stop(tilesObject);
				loadTileConfig(tilesObject);
			}

		}
		//2nd IF checks to make sure right click mouse button action is done
		if (SDL_GetMouseState(&(mouse_position.x), &(mouse_position.y)) & SDL_BUTTON(SDL_BUTTON_RIGHT))//error has to do with the mouse struct
		{
			x = myMouse.x; y = myMouse.y;
			for (int row = 0; row <= 17; row++)
				for (int col = 0; col <= 18; col++)
					if (x > (tileCoordMainWindow[row][col]).xUL &&x<tileCoordMainWindow[row][col].xLR
						&& y>tileCoordMainWindow[row][col].yUL &&y < tileCoordMainWindow[row][col].yLR)
					{
						//update display to include info about tileObject when right clicked
						RowsAndCols.row = row;
						RowsAndCols.col = col;
					}
		}

	}
}
//------------------------------------------------------MOUSE HANDLER
SDL_Point mouseHandler(TileCoord  tileCoords[], TileCoord(&tileCoordMainWindow)[18][19], Tile(&tilesObject)[18][19])
{
	//current position is gathered of mouse during click or a general call
	static SDL_Point mouse_position;
	//static bool state;
	SDL_GetMouseState(&(mouse_position.x), &(mouse_position.y));
	/*if (SDL_GetMouseState(&(mouse_position.x), &(mouse_position.y)) & SDL_BUTTON(SDL_BUTTON_LEFT)) LEAVE IF CHANGE REQUIRED
	state = true;
	else
	state = false;
	if (state) {
	cout << "Mouse position: x: " <<  //    Print mouse position
	mouse_position.x << "   y:   " << mouse_position.y << endl;
	}
	myMouse.state = state;*/
	
	myMouse.x = mouse_position.x;
	myMouse.y = mouse_position.y;
	buttonHandler(tileCoords, tileCoordMainWindow, tilesObject);

	return mouse_position;
}
//------------------------------------------------------MAIN DISPLAY HANDLER
void display(SDL_Renderer * renderer, SDL_Texture * MainWindowTex, SDL_Texture * GateSelectorTex,
	Rect * rect, TileCoord(&tileCoordMainWindow)[18][19], Tile(&tilesObject)[18][19],
	SDL_Texture* SelectorsTex[], TTF_Font * font, SDL_Color * color, 
	SDL_Texture * InstructionsTex, SDL_Texture * InstructionsButtonTex)
{
	cout << "-------------display call--------------" << endl;
	int row = RowsAndCols.row;
	int col = RowsAndCols.col;
	//char array to be used in later img  intializations
	static char coordsX[10];
	static char coordsY[10];
	char valOut[10];
	char north[10];
	char east[10];
	char south[10];
	char west[10];
	char state[20];
	char on[] = "ON"; char off[] = "OFF";
	char direction[40];
	//sprintf is used to convert int  into char array
	sprintf_s(coordsX, "%ld", row + 1);
	sprintf_s(coordsY, "%ld", col + 1);
	sprintf_s(north, "%ld", (tilesObject[row][col]).get_directionNorth());
	sprintf_s(east, "%ld", (tilesObject[row][col]).get_directionEast());
	sprintf_s(south, "%ld", (tilesObject[row][col]).get_directionSouth());
	sprintf_s(west, "%ld", (tilesObject[row][col]).get_directionWest());
	sprintf_s(valOut, "%ld", (tilesObject[row][col]).get_CurrentValOut());


	//Convert state bool val to char array
	if ((tilesObject[row][col]).getState()) 
	{
		strcpy(state,on);
	}
	else
	{
		strcpy(state, off);
	}
	//Create rect type for texture placement
	SDL_Rect coordX_rect = {710,280,25,20};
	SDL_Rect coordY_rect = {745,280,25,20};
	SDL_Rect state_rect = { 720,440,25,20 };
	SDL_Rect type_rect = { 630,350,80,25 };
	SDL_Rect northDirection_rect = {690,500,15,17};//15,70
	SDL_Rect eastDirection_rect = { 690,518,15,17 };
	SDL_Rect southDirection_rect = { 690,536,15,17 };
	SDL_Rect westDirection_rect = { 690,554,15,17 };
	SDL_Rect valOut_rect = { 630, 385,25,20 };
	SDL_Rect instruct_rect = { 750,550,45,45 };
	SDL_Rect save_rect = { 760,340,25,25 };
	SDL_Rect load_rect = { 760,375,25,25 };

	//Load imgs
	SDL_Surface * CoordinateXimg = TTF_RenderText_Solid(font, coordsX, *(color));
	SDL_Surface * CoordinateYimg = TTF_RenderText_Solid(font,coordsY , *(color));
	SDL_Surface * Typeimg = TTF_RenderText_Solid(font, (tilesObject[row][col]).getType(), *(color));
	SDL_Surface * Stateimg = TTF_RenderText_Solid(font, state, *(color));
	SDL_Surface * ValOutimg = TTF_RenderText_Solid(font, valOut, *(color));
	SDL_Surface * Northimg = TTF_RenderText_Solid(font, north, *(color));
	SDL_Surface * Eastimg = TTF_RenderText_Solid(font, east, *(color));
	SDL_Surface * Southimg = TTF_RenderText_Solid(font, south, *(color));
	SDL_Surface * Westimg = TTF_RenderText_Solid(font, west, *(color));
	SDL_Surface * Saveimg = IMG_Load("Save.png");
	SDL_Surface * Loadimg = IMG_Load("Load.png");

	//LOAD MAIN WINDOW and SUBSEQUENT TILES TO BE SHOWN
	SDL_RenderCopy(renderer, MainWindowTex, NULL, NULL);
	displayTile(renderer, tileCoordMainWindow, tilesObject, SelectorsTex);

	//CREATE OVERLAY TEXTURES from imgs
	SDL_Texture * CoordinateXTex = SDL_CreateTextureFromSurface(renderer, CoordinateXimg);
	SDL_Texture * CoordinateYTex = SDL_CreateTextureFromSurface(renderer, CoordinateYimg);
	SDL_Texture * StateTex = SDL_CreateTextureFromSurface(renderer, Stateimg);
	SDL_Texture * TypeTex = SDL_CreateTextureFromSurface(renderer, Typeimg);
	SDL_Texture * NorthTex = SDL_CreateTextureFromSurface(renderer, Northimg);
	SDL_Texture * EastTex = SDL_CreateTextureFromSurface(renderer, Eastimg);
	SDL_Texture * SouthTex = SDL_CreateTextureFromSurface(renderer, Southimg);
	SDL_Texture * WestTex = SDL_CreateTextureFromSurface(renderer, Westimg);
	SDL_Texture * ValOutTex = SDL_CreateTextureFromSurface(renderer, ValOutimg);
	SDL_Texture * LoadTex = SDL_CreateTextureFromSurface(renderer, Loadimg);
	SDL_Texture * SaveTex = SDL_CreateTextureFromSurface(renderer, Saveimg);

	//COPY to Renderer
	SDL_RenderCopy(renderer, ValOutTex, NULL, &(valOut_rect));
	SDL_RenderCopy(renderer,CoordinateXTex, NULL, &(coordX_rect));
	SDL_RenderCopy(renderer, CoordinateYTex, NULL, &(coordY_rect));
	SDL_RenderCopy(renderer, StateTex, NULL, &(state_rect));
	SDL_RenderCopy(renderer, TypeTex, NULL, &(type_rect));
	SDL_RenderCopy(renderer, NorthTex, NULL, &(northDirection_rect));
	SDL_RenderCopy(renderer, EastTex, NULL, &(eastDirection_rect));
	SDL_RenderCopy(renderer, SouthTex, NULL, &(southDirection_rect));
	SDL_RenderCopy(renderer, WestTex, NULL, &(westDirection_rect));
	SDL_RenderCopy(renderer, InstructionsButtonTex, NULL, &(instruct_rect));
	SDL_RenderCopy(renderer, SaveTex, NULL, &(save_rect));
	SDL_RenderCopy(renderer, LoadTex, NULL, &(load_rect));

	//copy selector to renderer as overlay texture
	if (showGateSelector)
		SDL_RenderCopy(renderer, GateSelectorTex, NULL, &(rect->GateSelector_rect));
	if (showInstructions)
		SDL_RenderCopy(renderer, InstructionsTex, NULL, NULL);

	SDL_RenderPresent(renderer);

}
//------------------------------------------------------KEYBOARD HANDLER
bool keyboardFunc(SDL_Event  *eventTest, bool quit, TileCoord tileCoord[], TileCoord(&tileCoordMainWindow)[18][19]
	, Tile (&tilesObject)[18][19])
{
	SDL_WaitEvent(eventTest);//waits for key press
	switch ((*eventTest).type)
	{
	case SDL_QUIT:
		quit = true;
		return true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		mouseHandler(tileCoord, tileCoordMainWindow, tilesObject);
		myMouse.state = true;
		return false;
		break;
	case SDL_MOUSEBUTTONUP:
		mouseHandler(tileCoord, tileCoordMainWindow, tilesObject);
		myMouse.state = false;
		return false;
		break;
	default:
		return false;
	}
}
//------------------------------------------------------MAIN-----------------------------------------------------------------------
int main(int argc, char ** argv)
{
	/*
	Alot is handled here
	-a couple of INIT functions for librarys are handled
	-arrays for positions and objects are created
	-another INIT functions for a library is handled
	-create windows and renderer
	-imgs and textures
	-create sprite sheet textures
	-go into main while loop
		-displays and keyboard checks repeatedly
	*/
	bool quit = false;//quit for application to exit
	SDL_Event eventTest;//created here for future use in keyboardFunc

	//Inits
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	//IMPORTANT ARRAY DECLARATIONS
	Rect rect = { { 0, 300,800,300 } }; //x,y,w,h
	TileCoord tileCoord[15];
	TileCoord tileCoordMainWindow[18][19];
	static Tile tilesObjects[18][19];

	//tileObjects initialize objects and set to NULL
	for (int colTiles = 0; colTiles <= 18; colTiles++)
		for (int rowTiles = 0; rowTiles <= 17; rowTiles++)
		{
			(tilesObjects[rowTiles][colTiles]).setType("NULL");
			(tilesObjects[rowTiles][colTiles]).setState(false);
			(tilesObjects[rowTiles][colTiles]).setX(colTiles);
			(tilesObjects[rowTiles][colTiles]).setY(rowTiles);
			(tilesObjects[rowTiles][colTiles]).set_CurrentValN(false);
			(tilesObjects[rowTiles][colTiles]).set_CurrentValS(false);
			(tilesObjects[rowTiles][colTiles]).set_CurrentValW(false);
			(tilesObjects[rowTiles][colTiles]).set_CurrentValOut(false);
		}
	//gateSelector positions X and Y coords
	for (int i = 0; i <= 8; i++) {
		tileCoord[i].xUL = 193 + (i * 64);		tileCoord[i].yUL = 65 + 300;
		tileCoord[i].xLR = 193 + 62 + (i * 64);		tileCoord[i].yLR = 65 + 300 + 62;
	}
	for (int j = 0; j <= 4; j++) {
		tileCoord[9 + j].xUL = 193 + (j * 64);		tileCoord[9 + j].yUL = 193 + 300;
		tileCoord[9 + j].xLR = 193 + 62 + (j * 64);		tileCoord[9 + j].yLR = 193 + 300 + 62;
	}
	//tilePositions create X and Y coords
	for (int col = 0; col <= 18; col++)
		for (int row = 0; row <= 17; row++)
		{
			tileCoordMainWindow[row][col].xUL = 1 + (col * 32); tileCoordMainWindow[row][col].yUL = 1 + (row * 32);
			tileCoordMainWindow[row][col].xLR = 1 + 30 + (col * 32); tileCoordMainWindow[row][col].yLR = 1 + 30 + (row * 32);
		}
	//

	//INIT CHECK ttf
	if (TTF_Init() == -1)
		cout << "TTF_Init: " << TTF_GetError() << endl;
	else
		cout << "works" << endl;
	cout << "TTF_Init: " << TTF_GetError() << endl;
	TTF_Font* MainFont = TTF_OpenFont("Arial.ttf", 14);
	SDL_Color BlackColor = { 0, 0, 0 };
	//Create Window and CREATE RENDERER
	SDL_Window * window = SDL_CreateWindow("Logic",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	//Create basic imgs
	SDL_Surface * MainWindowimg = IMG_Load("MainWindow.png");
	SDL_Surface * GateSelectorimg = IMG_Load("GateSelector.png");
	SDL_Surface * Instructionsimg = IMG_Load("Instructions.png");
	SDL_Surface * InstructionsButtonsimg = IMG_Load("buttons_instructions.png");
	SDL_Surface * Selectorsimg[14];
	//Creates Tiles imgs
	for (int k = 0; k <= 13; k++) //so tile Coord are offset incorrectly???
	{
		Selectorsimg[k] = crop_surface(GateSelectorimg, tileCoord[k].xUL,
			tileCoord[k].yUL-300,
			tileCoord[k].xLR - tileCoord[k].xUL,
			(tileCoord[k].yLR-300) - (tileCoord[k].yUL-300));  //surface,x,y,wid,height returns surface

	}

	//MAKES TEXTURES from imgs
	SDL_Texture * MainWindowTex = SDL_CreateTextureFromSurface(renderer, MainWindowimg);
	SDL_Texture * GateSelectorTex = SDL_CreateTextureFromSurface(renderer, GateSelectorimg);
	SDL_Texture * InstructionsTex = SDL_CreateTextureFromSurface(renderer, Instructionsimg);
	SDL_Texture * InstructionsButtonTex = SDL_CreateTextureFromSurface(renderer, InstructionsButtonsimg);
	SDL_Texture * SelectorsTex[14];

	for (int g = 0; g <= 13; g++)//CREATES SPRITE SHEET CUTOUTS
	{
		SelectorsTex[g] = SDL_CreateTextureFromSurface(renderer, Selectorsimg[g]);
	}




	//MAIN WHILE LOOP
	while (!quit)
	{
		quit = keyboardFunc(&eventTest, quit, tileCoord, tileCoordMainWindow, tilesObjects);
		display(renderer, MainWindowTex, GateSelectorTex, &rect, tileCoordMainWindow, tilesObjects, 
			SelectorsTex,MainFont,&BlackColor,InstructionsTex,InstructionsButtonTex);
	}

	//QUITS
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	return 0;
}