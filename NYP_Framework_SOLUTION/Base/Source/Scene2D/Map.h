#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init(	const int theScreen_Height, const int theScreen_Width, 
				const int theNumOfTiles_Height, const int theNumOfTiles_Width, 
				const int theMap_Height, const int theMap_Width,
				const int theTileSize_Height = 25, const int theTileSize_Width = 25);
	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);			// Get the number of tiles for height of the screen
	int GetNumOfTiles_Width(void);			// Get the number of tiles for width of the screen
	int GetTileSize_Height(void);			// Get height of tile
	int GetTileSize_Width(void);			// Get width of tile

	int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
	int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

	int getScreenHeight(void);				// Get height of screen
	int getScreenWidth(void);				// Get width of screen

	vector<vector<int> > theScreenMap;

private:
	int theScreen_Height;			// The map's height
	int theScreen_Width;			// The map's width
	int theNumOfTiles_Height;		// Number of tiles in the screen's height
	int theNumOfTiles_Width;		// Number of tiles in the screen's width
	int theTileSize_Height;
	int theTileSize_Width;

	int theMap_Height;				// The map's height
	int theMap_Width;				// The map's width
	int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
	int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

	bool LoadFile(const string mapName);
};
