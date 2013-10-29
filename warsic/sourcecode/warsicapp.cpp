/////////////////////////////////////////////////////////////////
//
// warsicapp.cpp
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

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djmodulemanager.h>
#include <djostime.h>
#include <dj2d.h>
#include <dj2dutil.h>
#include <djgamesounds.h>
#include <djfont.h>
#include <djosdatastorage.h>
#include <djdevicemanager.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "warsicapp.h"

/////////////////////////////////////////////////////////////////
// Name of the game module
const char *GetGameModuleName() { return "Warsic"; }
// Name of the game application
const char *GetGameObjectName() { return "WarsicApplication"; }
// Declare standard module imports
STANDARD_MODULE_IMPORTS(Warsic);
// Declare standard interfaces
STANDARD_INTERFACE_DECLARATION(Warsic, WarsicApplication);
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Globals
// Screen width (set in OnInit function)
djuint32				g_nScreenWidth			= 0;
// Screen height (set in OnInit function)
djuint32				g_nScreenHeight			= 0;

/////////////////////////////////////////////////////////////////
// List of sounds to load
static const char *l_szSoundFiles[SOUND_COUNT] =
{
	"sound/zap1.wav",
	"sound/tick.wav",
};
///////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////
// Application class
DJWarsicApplication::DJWarsicApplication()
{
	DJTrace("%s()", __FUNCTION__);

	m_pFlareSheet		= NULL;
	m_pBackgroundSheet	= NULL;
	m_pFont				= NULL;
	m_pSprite			= NULL;
}

///

DJWarsicApplication::~DJWarsicApplication()
{
	DJTrace("%s()", __FUNCTION__);
}

///

djresult DJWarsicApplication::OnInit()
{
	DJTrace("%s()", __FUNCTION__);

#ifndef _DJWINDOWS
	// Set updates to run at solid 60 fps no matter what the rendering frame rate is (this might result in several updates for each paint called)
	SetUpdateToFixedTime(60);
#endif
	// Enable threaded sound updates
	pTheSoundDevice->EnableThreadedUpdates(DJTRUE);

	// Get size of screen and put in global variables
	pTheRenderDevice->GetScreenSize(g_nScreenWidth, g_nScreenHeight);

	// Randomize random seed by using system time
	djRandomSetSeed( (djuint32)(djGetSystemTimeFloat() * 65536.0f) );
	
	// Initialize sprite engine with the layers we want
	if (!theSpriteEngine.Init(LAYER_COUNT))
	{
		DJError("Failed to initialize the sprite engine!");
		return DJ_FAILURE;
	}

	// Load font
	m_pFont = (DJFont*)theResourceManager.GetResource( "font", DJResource::TYPE_FONT );
	if (m_pFont == NULL)
	{
		DJWarning("Failed to load the font!");
	}

	// Create a flare sprite sheet
	m_pFlareSheet = DJ_NEW(DJ2DGraphicsSheet);
	if (!m_pFlareSheet->Init("sprites/flare.tga"))
	{
		DJError("Failed to load flare sheet!");
		return DJ_FAILURE;
	}
	DJVector2 vHotspot = DJVector2(128/2, 128/2);
	m_pFlareSheet->AutoGenFrames(0, 0, 128, 128, 1, 1, &vHotspot);
	theSpriteEngine.AddGraphicsSheet(m_pFlareSheet);

	// Create background image sheet
	m_pBackgroundSheet = DJ_NEW(DJ2DGraphicsSheet);
	if (!m_pBackgroundSheet->Init("backgrounds/space001.tga"))
	{
		DJError("Failed to load background sheet!");
		return DJ_FAILURE;
	}
	m_pBackgroundSheet->AutoGenFrames(0, 0, 1024, 512, 1, 1, NULL);
	theSpriteEngine.AddGraphicsSheet(m_pBackgroundSheet);
		
	// Set the background layer to draw the background image
	DJ2DLayer *pLayer = theSpriteEngine.GetLayer(LAYER_BACKGROUND);
	DJ2DGraphicsFrame *pFrame = m_pBackgroundSheet->GetFrame(0);
	pLayer->SetBackground(pFrame);
	pLayer->SetScale((float)g_nScreenWidth/1024.0f, (float)g_nScreenHeight/512.0f);
	pLayer->SetPosition(0,0);

	// Create a sprite node
	m_pSprite = DJ_NEW(DJ2DSprite);
	// Make the sprite magnify with a blur instead of pixelated
	m_pSprite->SetFilter(DJ2DSprite::filterLinear);
	// Make the sprite blend with the background by adding the color values
	m_pSprite->SetBlendOp(DJ2DSprite::blendAdd);
	// Set the sprite to the center of the screen
	m_pSprite->SetPosition((float)g_nScreenWidth/2, (float)g_nScreenHeight/2);
	// Set the graphics of the sprite to be the flare graphics we loaded above
	m_pSprite->SetCurrentFrame(m_pFlareSheet->GetFrame(0));

	// Add sprite to our sprite layer
	pLayer = theSpriteEngine.GetLayer(LAYER_SPRITES);
	pLayer->AddNode(m_pSprite);

	// Load sound effects using the DJEngine gamesound helper functions (internally uses DJSoundDevice)
	if (!djGameSoundsInit(l_szSoundFiles, SOUND_COUNT))
	{
		DJError("Failed to load sound effects!");
		return DJ_FAILURE;
	}

	return DJ_SUCCESS;
}

///

void DJWarsicApplication::OnTerm()
{
	DJTrace("%s()", __FUNCTION__);

	// Release the font resource
	DJ_SAFE_RELEASE(m_pFont);

	
	// No need to delete this since the sprite engine takes care of it (we added it above)
	m_pSprite			= NULL;
	m_pFlareSheet		= NULL;
	m_pBackgroundSheet	= NULL;

	// Terminate sprite engine
	theSpriteEngine.Term();

	// Terminate DJEngine gamesound helper function
	djGameSoundsTerm();

}

///

void DJWarsicApplication::OnUpdate()
{
	// Update sprite engine
	theSpriteEngine.OnUpdate();
}

///

djbool DJWarsicApplication::OnPaint()
{
	// Setup rendering view / transformation
	pTheRenderDevice->SetViewTransform(DJMatrix::Identity());
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)1024.0f,(float)g_nScreenHeight/(float)g_nScreenWidth*1024.0f,0.0f,100.0f);

	// Set the clear color
	pTheRenderDevice->SetClearColor(DJColor(1,0,0,0));
	// Clear the screen
	pTheRenderDevice->Clear(DJRenderDevice::flagClearAll);
	// Disable the depth buffer (no need for it in 2D games usually)
	pTheRenderDevice->EnableZBuffer(0);
	
	// Setup the 2D render context
	DJ2DRenderContext rc;
	rc.m_uFlags = 0;
	rc.m_cModColor = DJColor(1,1,1,1);
	rc.m_cAddColor = DJColor(0,0,0,0);
	rc.m_mLayerTransform = DJMatrix2D::Identity();
	rc.m_pLayer = NULL;
	rc.m_mTransform = DJMatrix2D::Identity();

	// Render all layers (and sprites)
	theSpriteEngine.OnPaint(rc);

	// Draw some text
	if (m_pFont)
		m_pFont->DrawString("Tutorial3\nTouch screen to move sprite", DJVector3(10,10,0), DJVector2(16,16), 0xFFFFFFFF);
	return DJTRUE;
}

///

djint32 DJWarsicApplication::OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Begin: %d %.2f %.2f", nID, x, y);

	if (DJApplication::OnTouchBegin(nDevice, nID, x,y))
		return 1;

	djGameSoundPlay(SOUND_ZAP);	
	return 0;
}

///

djint32 DJWarsicApplication::OnTouchMove(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Move: %d %.2f %.2f", nID, x, y);

	if (DJApplication::OnTouchMove(nDevice, nID, x,y))
		return 1;
	return 0;
}

///

djint32 DJWarsicApplication::OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch End: %d %.2f %.2f", nID, x, y);

	if (DJApplication::OnTouchEnd(nDevice, nID, x,y))
		return 1;
	return 0;
}

///

djint32 DJWarsicApplication::OnTouchCancel(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Cancel: %d %.2f %.2f", nID, x, y);

	if (DJApplication::OnTouchCancel(nDevice, nID, x,y))
		return 1;
	return 0;
}

///

djint32 DJWarsicApplication::OnAccelerate(float x, float y, float z)
{
	if (DJApplication::OnAccelerate(x,y,z))
		return 1;

	return 0;
}

///

djint32 DJWarsicApplication::OnButtonDown(djint32 nKey)
{
	DJInfo("Button Down: %d", nKey);

	if (DJApplication::OnButtonDown(nKey))
		return 1;

	return 0;
}

///

djint32 DJWarsicApplication::OnButtonUp(djint32 nKey)
{
	DJInfo("Button Up: %d", nKey);

	if (DJApplication::OnButtonUp(nKey))
		return 1;

	return 0;
}

///

void DJWarsicApplication::OnMessage(djuint32 nMessage, djuint32 nParam1, djuint32 nParam2)
{
	DJTrace(__FUNCTION__"()");
	DJApplication::OnMessage(nMessage, nParam1, nParam2);
}
// Application class
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

