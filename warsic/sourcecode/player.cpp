/////////////////////////////////////////////////////////////////
//
// player.cpp
// Player class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djgamesounds.h>
#include <djeffect.h>
#include <djfont.h>
#include <djlocalizer.h>
#include <djservice.h>
#include <djanalytics.h>
#include <djquaternion.h>
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Game Includes
#include "player.h"
#include "warsicapp.h"
#include "camera.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
extern float					g_fScreenScaler;
extern DJVector2				g_vGlobalScale;	
extern Camera*					g_pCamera;
extern djuint32					g_nScreenWidth;
extern djuint32					g_nScreenHeight;

Player::Player()
{
	m_pKiwiSkel	= NULL;
}

///
Player::~Player()
{
	theSpriteEngine.RemoveActiveNode(m_pKiwiSkel);
	theSpriteEngine.RemoveNode(m_pKiwiSkel);
	DJ_SAFE_DELETE(m_pKiwiSkel);
}

///

djbool Player::Init()
{ 	
	m_pKiwiSkel = DJ_NEW(DJ2DSkeletonNode);
	m_pKiwiSkel->SetPosition((float)g_nScreenWidth/2, (float)g_nScreenHeight*6/10);
	m_pKiwiSkel->SetScale((float)g_nScreenHeight/720.0f, (float)g_nScreenHeight/720.0f);
	m_pKiwiSkel->Create("sprites/dragon");
	m_pKiwiSkel->SetAnimation("flying", DJTRUE);
	theSpriteEngine.AddActiveNode(m_pKiwiSkel);
	theSpriteEngine.AddNode(m_pKiwiSkel, LAYER_PLAYER);	

	return DJTRUE;
}

///

djbool Player::Update()
{

	return DJTRUE;
}

///	  

Player* g_pPlayer = NULL;
/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
