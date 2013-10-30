/////////////////////////////////////////////////////////////////
//
// mainmenu.cpp
// Main menu class
//
/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djlocalizer.h>
#include <djfont.h>
#include <djgamesounds.h>
#include <djfacebook.h>
#include <djservice.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "warsicapp.h"
#include "mainmenu.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
#ifdef _DJQNX
extern const char* g_szQNXAppID;
#endif // _DJQNX

djresult InviteToBBM()
{
#ifdef _DJQNX
	DJString s("Play Kiwi Wonder for Blackberry and try to beat my highscore!\n\nDownload the game on App World: https://appworld.blackberry.com/webstore/content/%s/", g_szQNXAppID);
	return djBBMShareText((const char*)s);
#else
	return DJ_NOT_IMPLEMENTED;
#endif // _DJQNX
}

djresult ShareBBMScore(djint64 nScore)
{
#ifdef _DJQNX
	DJString s = "I got ";
	djFormatScoreString(s, nScore,  (const char*)DJString(" meters and beat my highscore in Kiwi Wonder for Blackberry 10!\n\nDownload the game and try to beat me: https://appworld.blackberry.com/webstore/content/%s/", g_szQNXAppID));
	return djBBMShareText((const char*)s);
	//djBBMShareImage("https://fbcdn-profile-a.akamaihd.net/hprofile-ak-frc3/c6.0.169.169/s160x160/557251_342433079184099_1833074237_a.png");
	//djFacebookShowProfile("323730264386882", "page");
#else
	return DJ_NOT_IMPLEMENTED;
#endif // _DJQNX
}

djresult ShareFacebookScore(djint64 nScore)
{
#if defined(_DJIPHONE)
	DJString s = "I got ";
	djFormatScoreString(s, nScore,  (const char*)DJString(" meters and beat my highscore in Kiwi Wonder for iOS!\n\nDownload the game and try to beat me: https://itunes.apple.com/app/alpha-zero/id%s", g_szIOSAppID));
	return djBBMShareText((const char*)s);
	//djBBMShareImage("https://fbcdn-profile-a.akamaihd.net/hprofile-ak-frc3/c6.0.169.169/s160x160/557251_342433079184099_1833074237_a.png");
	//djFacebookShowProfile("323730264386882", "page");
#elif defined(_DJQNX)
	DJString s = "I got ";
	djFormatScoreString(s, nScore,  (const char*)DJString(" meters and beat my highscore in Kiwi Wonder for Blackberry 10!\n\nDownload the game and try to beat me: https://appworld.blackberry.com/webstore/content/%s/", g_szQNXAppID));
	return djBBMShareText((const char*)s);
	//djBBMShareImage("https://fbcdn-profile-a.akamaihd.net/hprofile-ak-frc3/c6.0.169.169/s160x160/557251_342433079184099_1833074237_a.png");
	//djFacebookShowProfile("323730264386882", "page");
#else
	return DJ_NOT_IMPLEMENTED;
#endif // _DJQNX
}

///////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////
DJMainMenuPageUINode::DJMainMenuPageUINode()
{
	SetClickable(1); 
}

///

DJMainMenuPageUINode::~DJMainMenuPageUINode()
{
}

///

void DJMainMenuPageUINode::InstanciateNode(DJUINode *pNode)
{
	DJPageUINode::InstanciateNode(pNode);
}

///

void DJMainMenuPageUINode::CopyNode(DJUINode *pNode)
{
	DJPageUINode::CopyNode(pNode);
}

djint32 DJMainMenuPageUINode::Build()
{
	djint32 nRet = DJPageUINode::Build();
	return nRet;
}

///

void DJMainMenuPageUINode::OnPaint(const DJ2DRenderContext &rc)
{
	if(!IsNodeVisible())
		return;

	DJ2DRenderContext rcc = rc;

	DJVector2 vPos = GetPosition();
	if(IsNodeEnabled())
	{
	}
	else
	{
		
	} 	

	DJPageUINode::OnPaint(rc);
}

///

djbool DJMainMenuPageUINode::OnTick()
{
	djbool bRet = DJUINode::OnTick();  

	return bRet;
}

///

void DJMainMenuPageUINode::OnShowNode(djbool bShow, djbool bParentChanged)
{
	DJPageUINode::OnShowNode(bShow, bParentChanged);
	if (bShow)
	{
	}
	else
	{  		
	}  
}

///

djbool DJMainMenuPageUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJPageUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);
	return bRet;
}

///

djint32 DJMainMenuPageUINode::OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJPageUINode::OnTouchBegin(nDevice, nID, x, y);
	return nRet;
}
///

djint32 DJMainMenuPageUINode::OnTouchMove( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJPageUINode::OnTouchMove(nDevice, nID, x, y);
	return nRet;
}

///

djint32 DJMainMenuPageUINode::OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJPageUINode::OnTouchEnd(nDevice, nID, x, y);

	return nRet;
}

///

djint32 DJMainMenuPageUINode::OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJPageUINode::OnTouchCancel(nDevice, nID, x, y);
	return nRet;
}

///

djbool DJMainMenuPageUINode::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	if (ev.m_uEventID == pTheUI->EVENTID_ON_CLICKED)
	{
	   if (ev.m_uStateID == pTheUI->GetStateID("MENUMAIN_PLAY"))
		{
			((DJWarsicApplication*)pTheApp)->GotoGameState(GS_INGAME);
			return DJTRUE;
		}
	}
	return DJFALSE;
}  
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
