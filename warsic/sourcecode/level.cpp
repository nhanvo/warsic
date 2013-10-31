/////////////////////////////////////////////////////////////////
//
// level.cpp
// Level class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include "djgamesounds.h"
#include <djfont.h>
#include <djlocalizer.h>
#include <djosthread.h>
#include <djanalytics.h>
#include <djservice.h>
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include "level.h"
#include "player.h"

/////////////////////////////////////////////////////////////////
extern Player* g_pPlayer;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
Level::Level()
{

}

///

Level::~Level()
{
	Term();
}

///

djbool Level::Init()
{
	/*if (!thePlayerHandler.Init())
	{
		DJError("Failed to initialize player handler!");
		return DJFALSE;
	}*/

	g_pPlayer = DJ_NEW(Player);
	if(g_pPlayer == NULL)
	{
		DJError("Failed to create Player (out of memory)!");
		return DJFALSE;
	}
	g_pPlayer->Init();
	return DJTRUE;
}

///

void Level::Update()
{
	g_pPlayer->Update();
}

///

void Level::Term()
{

}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
LevelManager::LevelManager()
{
	m_pCurrentLevel = NULL;
}

///

LevelManager::~LevelManager()
{
	Term();
}

///

djbool LevelManager::Init()
{
	return DJTRUE;
}

///

djbool LevelManager::Update()
{
	if(m_pCurrentLevel == NULL)
		return DJTRUE;

	m_pCurrentLevel->Update();
	return DJFALSE;
}

///

void LevelManager::Term()
{
	DJ_SAFE_DELETE(m_pCurrentLevel);
}

///

djbool LevelManager::LoadLevel()
{
	m_pCurrentLevel = DJ_NEW(Level);
	if (m_pCurrentLevel == NULL ||
		!m_pCurrentLevel->Init())
	{
		DJError("Failed to load level!");
		DJ_SAFE_DELETE(m_pCurrentLevel);
		return DJFALSE;
	}
	return DJTRUE;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////