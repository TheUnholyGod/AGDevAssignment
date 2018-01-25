#include "LuaInterface.h"
#include <iostream>
using namespace std;

// Allocating and initializing CLuaInterface's static data member.  
// The pointer is allocated but not the object's constructor.
CLuaInterface *CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
	: theLuaState(NULL)
{
}


CLuaInterface::~CLuaInterface()
{
}


// Initialisation of the Lua Interface Class
bool CLuaInterface::Init()
{
	bool result = false;

	// 1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// 3. Load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");

		result = true;
	}

	return result;
}

// Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	// 4. Read the variables
	// lua_getglobal(lua_State*, const char*)
	lua_getglobal(theLuaState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	// Display on screen
	cout << title << endl;
	cout << "---------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;

	lua_getglobal(theLuaState, "keyFORWARD");

	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);
	keyFORWARD = cstr[0];
	//std::string str(cstr, len);
}

// Get an integer value through the Lua Interface Class
int CLuaInterface::getIntValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return lua_tointeger(theLuaState, -1);
}
// Get a float value through the Lua Interface Class
float CLuaInterface::getFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

// Save an integer value through the Lua Interface Class
void CLuaInterface::saveIntValue(const char* varName, 
								const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0); cout << "....................";
}
// Save a float value through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char* varName, 
								const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
}

// Drop the Lua Interface Class
void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		// Close lua state
		lua_close(theLuaState);
	}
}
