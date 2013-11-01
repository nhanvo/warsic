/////////////////////////////////////////////////////////////////
//
// tutorial3app.h
// Application class
//
/////////////////////////////////////////////////////////////////
//
// Copyright (C) 2009 Daniel Jeppsson
// All Rights Reserved. These instructions, statements, computer
// programs, and/or related material (collectively, the "Source")
// contain unpublished information propietary to Daniel Jeppsson
// which is protected by US federal copyright law and by 
// international treaties. This Source may NOT be disclosed to 
// third parties, or be copied or duplicated, in whole or in 
// part, without the written consent of Daniel Jeppsson.
//
/////////////////////////////////////////////////////////////////
// 
// Author: Daniel Jeppsson
// 
/////////////////////////////////////////////////////////////////

#include <djapp.h>
#include <dj2d.h>
#include <djuinode.h>

#ifndef _WARSICAPP_H_
#define _WARSICAPP_H_

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Layer enum
enum
{
	// Layer for background image
	LAYER_BACKGROUND	= 0,
	// Layer for our sprites
	LAYER_PLAYER,
	// Layer for overlay and UI
	LAYER_OVERLAY,
	LAYER_MENU,
	
	// Number of layers
	LAYER_COUNT,
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Sound enum
enum
{
	// ZAP sound effect
	SOUND_ZAP = 0,
	// Tick sound effect
	SOUND_TICK,

	// Number of sound effects
	SOUND_COUNT
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
enum
{
	MENU_MAIN,	
	MENU_COUNT,
	MENU_INVALID
};

enum
{
	GS_PRELOAD = 0,
	GS_PRELOGO,
	GS_LOADGAME, 	
	GS_MENU,
	GS_LOAD_LEVEL,
	GS_UNLOAD_LEVEL,
	GS_INGAME,
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern void FillRect(const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4);
extern void PaintTexRect(DJTexture* pTexture, const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4, djbool bFlipHoriz = DJFALSE, djbool bAlphaAdd = DJFALSE);
extern void PaintScalable(DJTexture* pTexture, float* pBorders, const DJVector3 &vPosIn, const DJVector2 &vSize, djbool bSkipCenter, djbool bFlipHoriz, djuint32 uModCol);
extern void PaintEffectRect(DJTexture* pTexture, DJMaterial* pEffectMat, const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4, djbool bFlipHoriz = DJFALSE);
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJWarsicApplication : public DJApplication, public DJUIEventListener
{
	IMPLEMENT_INTERFACE(WarsicApplication, Application)

protected: 
	// menu stack
	DJArray<djuint32>	m_menuStack;

	// menu nodes
	DJUINode*			m_pMenus[MENU_COUNT];

	// Current game state
	djint32				m_nGameState;

	// Game state changed
	djbool				m_bGameStateChanged;

	// Time in current game state
	float				m_fStateTimer;

	// Is game loaded?
	djbool				m_bGameIsLoaded;
public:

	/////////////////////////////////////////////////////////////////
	// CONSTRUCTION/DESTRUCTION

	// Constructor
	DJWarsicApplication( );
	// Destructor
	virtual ~DJWarsicApplication( );

	// Application initialization
	virtual djresult OnInit( );

	// Load application
	virtual djbool OnLoad( );

	// Application termination
	virtual void OnTerm( );

	/////////////////////////////////////////////////////////////////
	// OPERATIONS

	// Called once per frame from main loop
	virtual void OnUpdate( );

	// Called once per frame to render the application
	virtual djbool OnPaint( );

	/////////////////////////////////////////////////////////////////
	// EVENT HANDLERS

	// Touch begin callback
	virtual djint32 OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y );
	// Touch move callback
	virtual djint32 OnTouchMove( djint32 nDevice, djint32 nID, float x, float y );
	// Touch end callback
	virtual djint32 OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y );
	// Touch cancel callback
	virtual djint32 OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y );
	// Accelerator event callback
	virtual djint32 OnAccelerate( float x, float y, float z );
	// Button Down
	virtual djint32 OnButtonDown(djint32 nKey);
	// Button Up
	virtual djint32 OnButtonUp(djint32 nKey);

	// Handle incoming system event
	virtual void OnMessage( djuint32 nMessage, djuint32 nParam1 = 0, djuint32 nParam2 = 0 );

	// event listener
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);

	// Is menu on menu stack?
	djbool IsMenuOnStack(djuint32 uMenu);

	// Goto menu
	void GotoMenu(djuint32 uMenu, djbool bReplaceStack = DJFALSE, djbool bReplaceLast = DJFALSE);
	// Get current menu ID
	djuint32 GetCurrentMenu()						{ if (m_menuStack.GetCurrentIndex()==0) return MENU_INVALID; return m_menuStack[m_menuStack.GetCurrentIndex()-1]; }
	// Get last menu ID
	djuint32 GetLastMenu()							{ if (m_menuStack.GetCurrentIndex()<=1) return MENU_INVALID; return m_menuStack[m_menuStack.GetCurrentIndex()-2]; }
	// Pop menu from stack and goto that menu
	djuint32 PopMenu();
	// Goto menu
	void GoBackToMenu(djuint32 uMenu); 

	// Load UI menu
	void LoadMenu(djuint32 uMenuID, const char* strMenuName);

	// Current game state
	djint32 GetGameState() const {return m_nGameState;}

	// Change game state
	djbool GotoGameState(djint nState);

	// Update load game state
	void UpdateLoadGame(); 
	// Update prelogo state
	void UpdatePreLogo();
	// Update load level state
	void UpdateLoadLevel();
	// Update unload level state
	void UpdateUnloadLevel();
	// Update ingame state
	void UpdateIngame();

	// Paint load game
	void PaintLoadGame(); 
	// Paint prelogo state
	void PaintPreLogo();
	// Paint load level
	void PaintLoadLevel();
		// Paint ingame state
	void PaintIngame();
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _WARSICAPP_H_
