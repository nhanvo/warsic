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
#include <djeffectresourcemanager.h>
#include <djlocalizer.h>
#include <djcamera.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "warsicapp.h"
#include "mainmenu.h"
#include "gamedef.h"

/////////////////////////////////////////////////////////////////
// Name of the game module
const char *GetGameModuleName() { return "Warsic"; }
// Name of the game application
const char *GetGameObjectName() { return "WarsicApplication"; }

/////////////////////////////////////////////////////////////////
IMPORT_STATIC_MODULE(Warsic)
IMPORT_STATIC_MODULE(OpenGLRender)
#if defined (_DJAIRPLAY)
IMPORT_STATIC_MODULE(AirSound)
#elif defined(_DJANDROID)
IMPORT_STATIC_MODULE(OpenALSound)
#else
IMPORT_STATIC_MODULE(OpenALSound)
#endif
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void DJSystem_AddStaticModules()
{
	ADD_STATIC_MODULE(OpenGLRender);
#if defined (_DJAIRPLAY)
	ADD_STATIC_MODULE(AirSound);
#else
	ADD_STATIC_MODULE(OpenALSound);
#endif
	ADD_STATIC_MODULE(Warsic);
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_ENUMERATE_INTERFACE()
ENUMERATE_INTERFACE(WarsicApplication)
ENUMERATE_INTERFACE(MainMenuPageUINode)
END_ENUMERATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_CREATE_INTERFACE()
CREATE_INTERFACE(WarsicApplication)
CREATE_INTERFACE(MainMenuPageUINode)
END_CREATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_REGISTER_SYMBOL(Warsic)
REGISTER_INTERFACE(WarsicApplication)
REGISTER_INTERFACE(MainMenuPageUINode)
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
REGISTER_ENUMERATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
END_REGISTER_SYMBOL()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Globals

extern djbool g_bUseSDUI;
extern float g_fGlobalFontQualityScale;

// Screen width (set in OnInit function)
djuint32				g_nScreenWidth			= 0;
// Screen height (set in OnInit function)
djuint32				g_nScreenHeight			= 0;
// View port 
DJViewport				g_UIViewport;
// font system
DJFont*					g_pFont				= NULL;
DJMaterial*				g_pRectMaterial		= NULL;
DJMaterial*				g_pTexRectMaterial	= NULL;
DJCamera*				g_pCamera			= NULL;

float g_fGameTimeScale = 1.0f;

/////////////////////////////////////////////////////////////////
// List of sounds to load
static const char *l_szSoundFiles[SOUND_COUNT] =
{
	"sound/zap1.wav",
	"sound/tick.wav",
};
///////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////
struct RectVertex
{
	float x,y,z;
	djuint32 dwCol;
};
static DJVertexBuffer::DJVERTEX_DESC RectVertex_Descs[] =
{
	DJVertexBuffer::DJVERTEX_DESC(0, DJVertexBuffer::typeFloat3, "pos_attr", INDEX_POS_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(12, DJVertexBuffer::typeByte4, "col_attr", INDEX_COL_ATTR, DJVertexBuffer::vflagNormalize),
};
#define RectVertex_DescCount 2
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
struct TexRectVertex
{
	float x,y,z;
	float u,v;
	djuint32 dwCol;
};
static DJVertexBuffer::DJVERTEX_DESC TexRectVertex_Descs[] =
{
	DJVertexBuffer::DJVERTEX_DESC(0, DJVertexBuffer::typeFloat3, "pos_attr", INDEX_POS_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(12, DJVertexBuffer::typeFloat2, "uv_attr", INDEX_UV_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(20, DJVertexBuffer::typeByte4, "col_attr", INDEX_COL_ATTR, DJVertexBuffer::vflagNormalize),
};
#define TexRectVertex_DescCount 3
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void FillRect(const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4)
{
	if (g_pRectMaterial == NULL)
	{
		DJWarning("rect material not loaded! Reloading..");
		g_pRectMaterial = (DJMaterial*)theResourceManager.GetResource("rect", DJResource::TYPE_MATERIAL);
	}

	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetMaterial(g_pRectMaterial);
	if (g_pRectMaterial->GetCurrentRenderMode() == NULL)
		return;
	pTheRenderDevice->UpdateUniforms(g_pRectMaterial->GetCurrentRenderMode()->GetLocalUniformList());
	pTheRenderDevice->EnableAlphaBlend(DJTRUE);

	RectVertex vtx[4];
	vtx[0].x = vPos[0];
	vtx[0].y = vPos[1];
	vtx[0].z = vPos[2];
	vtx[0].dwCol = dwCol1;
	vtx[1].x = vtx[0].x;
	vtx[1].y = vtx[0].y + vSize[1];
	vtx[1].z = vtx[0].z;
	vtx[1].dwCol = dwCol2;
	vtx[2].x = vtx[0].x + vSize[0];
	vtx[2].y = vtx[0].y + vSize[1];
	vtx[2].z = vtx[0].z;
	vtx[2].dwCol = dwCol3;
	vtx[3].x = vtx[0].x + vSize[0];
	vtx[3].y = vtx[0].y;
	vtx[3].z = vtx[0].z;
	vtx[3].dwCol = dwCol4;
	pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleFan, 2, vtx, RectVertex_DescCount, RectVertex_Descs, sizeof(RectVertex));
}

///

void FillRect(const DJVector3 &vPos0, const DJVector3 &vPos1, const DJVector3 &vPos2, const DJVector3 &vPos3, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4)
{
	if (g_pRectMaterial == NULL)
	{
		DJWarning("rect material not loaded! Reloading..");
		g_pRectMaterial = (DJMaterial*)theResourceManager.GetResource("rect", DJResource::TYPE_MATERIAL);
	}

	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetMaterial(g_pRectMaterial);
	if (g_pRectMaterial->GetCurrentRenderMode() == NULL)
		return;
	pTheRenderDevice->UpdateUniforms(g_pRectMaterial->GetCurrentRenderMode()->GetLocalUniformList());
	pTheRenderDevice->EnableAlphaBlend(DJTRUE);

	RectVertex vtx[4];
	vtx[0].x = vPos0[0];
	vtx[0].y = vPos0[1];
	vtx[0].z = vPos0[2];
	vtx[0].dwCol = dwCol1;
	vtx[1].x = vPos1[0];
	vtx[1].y = vPos1[1];
	vtx[1].z = vPos1[2];
	vtx[1].dwCol = dwCol2;
	vtx[2].x = vPos2[0];
	vtx[2].y = vPos2[1];
	vtx[2].z = vPos2[2];
	vtx[2].dwCol = dwCol3;
	vtx[3].x = vPos3[0];
	vtx[3].y = vPos3[1];
	vtx[3].z = vPos3[2];
	vtx[3].dwCol = dwCol4;
	pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleFan, 2, vtx, RectVertex_DescCount, RectVertex_Descs, sizeof(RectVertex));
}

///

void PaintTexRect(DJTexture* pTexture, const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4, djbool bFlipHoriz, djbool bAlphaAdd)
{
	if (g_pTexRectMaterial == NULL)
		g_pTexRectMaterial = (DJMaterial*)theResourceManager.GetResource("std_ui", DJResource::TYPE_MATERIAL);
	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetMaterial(g_pTexRectMaterial);
	if (g_pTexRectMaterial->GetCurrentRenderMode() == NULL)
		return;
	pTheRenderDevice->UpdateUniforms(g_pTexRectMaterial->GetCurrentRenderMode()->GetLocalUniformList());
	pTheRenderDevice->EnableAlphaBlend(DJTRUE);
	pTheRenderDevice->SetTexture(pTexture);

	if (bAlphaAdd)
	{
		pTheRenderDevice->SetAlphaBlendMode(DJBLEND_SRCALPHA, DJBLEND_ONE);
	}

	TexRectVertex vtx[4];
	vtx[0].x = vPos[0];
	vtx[0].y = vPos[1];
	vtx[0].z = vPos[2];
	vtx[0].dwCol = dwCol1;
	vtx[1].x = vtx[0].x;
	vtx[1].y = vtx[0].y + vSize[1];
	vtx[1].z = vtx[0].z;
	vtx[1].dwCol = dwCol2;
	vtx[2].x = vtx[0].x + vSize[0];
	vtx[2].y = vtx[0].y + vSize[1];
	vtx[2].z = vtx[0].z;
	vtx[2].dwCol = dwCol3;
	vtx[3].x = vtx[0].x + vSize[0];
	vtx[3].y = vtx[0].y;
	vtx[3].z = vtx[0].z;
	vtx[3].dwCol = dwCol4;
	if (bFlipHoriz)
	{
		vtx[0].u = 1;
		vtx[0].v = 0;
		vtx[1].u = 1;
		vtx[1].v = 1;
		vtx[2].u = 0;
		vtx[2].v = 1;
		vtx[3].u = 0;
		vtx[3].v = 0;
	}
	else
	{
		vtx[0].u = 0;
		vtx[0].v = 0;
		vtx[1].u = 0;
		vtx[1].v = 1;
		vtx[2].u = 1;
		vtx[2].v = 1;
		vtx[3].u = 1;
		vtx[3].v = 0;
	}
	pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleFan, 2, vtx, TexRectVertex_DescCount, TexRectVertex_Descs, sizeof(TexRectVertex));
}

///

void PaintScalable(DJTexture* pTexture, float* pBorders, const DJVector3 &vPosIn, const DJVector2 &vSize, djbool bSkipCenter, djbool bFlipHoriz, djuint32 uModCol)
{
	DJVector3 vPos = vPosIn;
	if (bFlipHoriz)
	{
		vPos[0] += vSize[0];
	}

	if (g_pTexRectMaterial == NULL)
		g_pTexRectMaterial = (DJMaterial*)theResourceManager.GetResource("std_ui", DJResource::TYPE_MATERIAL);
	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetMaterial(g_pTexRectMaterial);
	if (g_pTexRectMaterial->GetCurrentRenderMode() == NULL)
		return;
	pTheRenderDevice->UpdateUniforms(g_pTexRectMaterial->GetCurrentRenderMode()->GetLocalUniformList());
	pTheRenderDevice->EnableAlphaBlend(DJTRUE);
	pTheRenderDevice->SetTexture(pTexture);

	float fB[BORDER_COUNT];
	fB[BORDER_TOP] = pBorders[BORDER_TOP];
	fB[BORDER_BOTTOM] = pBorders[BORDER_BOTTOM];
	fB[BORDER_LEFT] = pBorders[BORDER_LEFT];
	fB[BORDER_RIGHT] = pBorders[BORDER_RIGHT];
	if (vSize[0] < pBorders[BORDER_LEFT] + pBorders[BORDER_RIGHT])
	{
		fB[BORDER_LEFT] = fB[BORDER_RIGHT] = vSize[0]*0.5f;
		//vSize[0] = pBorders[BORDER_LEFT] + pBorders[BORDER_RIGHT];
	}
	if (vSize[1] < pBorders[BORDER_TOP] + pBorders[BORDER_BOTTOM])
	{
		fB[BORDER_TOP] = fB[BORDER_BOTTOM] = vSize[1]*0.5f;
		//vSize[1] = pBorders[BORDER_TOP] + pBorders[BORDER_BOTTOM];
	}

	DJVector3 pos[16];
	DJVector2 uvs[16];
	TexRectVertex vtx[16];
	DJVector2 vTexSize((float)pTexture->GetWidth(),(float)pTexture->GetHeight());

	if (bFlipHoriz)
	{
		pos[0] = vPos;
		pos[1] = vPos - DJVector2(fB[BORDER_LEFT], 0);
		pos[2] = vPos - DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[3] = vPos - DJVector2(vSize[0],0);

		pos[4] = vPos + DJVector2(0,fB[BORDER_TOP]);
		pos[5] = vPos + DJVector2(0,fB[BORDER_TOP]) - DJVector2(fB[BORDER_LEFT], 0);
		pos[6] = vPos + DJVector2(0,fB[BORDER_TOP]) - DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[7] = vPos + DJVector2(0,fB[BORDER_TOP]) - DJVector2(vSize[0],0);

		pos[8] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]);
		pos[9] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) - DJVector2(fB[BORDER_LEFT], 0);
		pos[10] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) - DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[11] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) - DJVector2(vSize[0],0);

		pos[12] = vPos + DJVector2(0,vSize[1]);
		pos[13] = vPos + DJVector2(0,vSize[1]) - DJVector2(fB[BORDER_LEFT], 0);
		pos[14] = vPos + DJVector2(0,vSize[1]) - DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[15] = vPos + DJVector2(0,vSize[1]) - DJVector2(vSize[0],0);
	}
	else
	{
		pos[0] = vPos;
		pos[1] = vPos + DJVector2(fB[BORDER_LEFT], 0);
		pos[2] = vPos + DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[3] = vPos + DJVector2(vSize[0],0);

		pos[4] = vPos + DJVector2(0,fB[BORDER_TOP]);
		pos[5] = vPos + DJVector2(0,fB[BORDER_TOP]) + DJVector2(fB[BORDER_LEFT], 0);
		pos[6] = vPos + DJVector2(0,fB[BORDER_TOP]) + DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[7] = vPos + DJVector2(0,fB[BORDER_TOP]) + DJVector2(vSize[0],0);

		pos[8] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]);
		pos[9] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) + DJVector2(fB[BORDER_LEFT], 0);
		pos[10] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) + DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[11] = vPos + DJVector2(0,vSize[1]-fB[BORDER_BOTTOM]) + DJVector2(vSize[0],0);

		pos[12] = vPos + DJVector2(0,vSize[1]);
		pos[13] = vPos + DJVector2(0,vSize[1]) + DJVector2(fB[BORDER_LEFT], 0);
		pos[14] = vPos + DJVector2(0,vSize[1]) + DJVector2(vSize[0]-fB[BORDER_RIGHT], 0);
		pos[15] = vPos + DJVector2(0,vSize[1]) + DJVector2(vSize[0],0);
	}

	uvs[0] = DJVector2(0,0);
	uvs[1] = DJVector2(0,0) + DJVector2(1,0)*pBorders[BORDER_LEFT]/vTexSize[0];
	uvs[2] = DJVector2(1,0) - DJVector2(1,0)*pBorders[BORDER_RIGHT]/vTexSize[0];
	uvs[3] = DJVector2(1,0);

	uvs[4] = DJVector2(0,0) + DJVector2(0,1)*pBorders[BORDER_TOP]/vTexSize[1];
	uvs[5] = DJVector2(0,0) + DJVector2(0,1)*pBorders[BORDER_TOP]/vTexSize[1] + DJVector2(1,0)*pBorders[BORDER_LEFT]/vTexSize[0];
	uvs[6] = DJVector2(1,0) + DJVector2(0,1)*pBorders[BORDER_TOP]/vTexSize[1] - DJVector2(1,0)*pBorders[BORDER_RIGHT]/vTexSize[0];
	uvs[7] = DJVector2(1,0) + DJVector2(0,1)*pBorders[BORDER_TOP]/vTexSize[1];

	uvs[8] = DJVector2(0,1) - DJVector2(0,1)*pBorders[BORDER_BOTTOM]/vTexSize[1];
	uvs[9] = DJVector2(0,1) - DJVector2(0,1)*pBorders[BORDER_BOTTOM]/vTexSize[1] + DJVector2(1,0)*pBorders[BORDER_LEFT]/vTexSize[0];
	uvs[10] = DJVector2(1,1) - DJVector2(0,1)*pBorders[BORDER_BOTTOM]/vTexSize[1] - DJVector2(1,0)*pBorders[BORDER_RIGHT]/vTexSize[0];
	uvs[11] = DJVector2(1,1) - DJVector2(0,1)*pBorders[BORDER_BOTTOM]/vTexSize[1];

	uvs[12] = DJVector2(0,1);
	uvs[13] = DJVector2(0,1) + DJVector2(1,0)*pBorders[BORDER_LEFT]/vTexSize[0];
	uvs[14] = DJVector2(1,1) - DJVector2(1,0)*pBorders[BORDER_RIGHT]/vTexSize[0];
	uvs[15] = DJVector2(1,1);

	for (int q=0; q<16; q++)
	{
		vtx[q].x = pos[q][0];
		vtx[q].y = pos[q][1];
		vtx[q].z = pos[q][2];
		vtx[q].u = uvs[q][0];
		vtx[q].v = uvs[q][1];
		vtx[q].dwCol = uModCol;
	}

	if (!bSkipCenter)
	{
		djuint16 idx[] = 
		{
			0,4,5, 0,5,1, 1,5,6, 1,6,2, 2,6,7, 2,7,3, 
			4,8,9, 4,9,5, 5,9,10, 5,10,6, 6,10,11, 6,11,7, 
			8,12,13, 8,13,9, 9,13,14, 9,14,10, 10,14,15, 10,15,11
		};
		pTheRenderDevice->RenderIndexedPrimitiveRaw(DJRenderDevice::ptTriangleList, 18, 16, vtx, TexRectVertex_DescCount, TexRectVertex_Descs, sizeof(TexRectVertex), idx);
	}
	else
	{
		djuint16 idx[] = 
		{
			0,4,5, 0,5,1, 1,5,6, 1,6,2, 2,6,7, 2,7,3, 
			4,8,9, 4,9,5, 6,10,11, 6,11,7, 
			8,12,13, 8,13,9, 9,13,14, 9,14,10, 10,14,15, 10,15,11
		};
		pTheRenderDevice->RenderIndexedPrimitiveRaw(DJRenderDevice::ptTriangleList, 16, 16, vtx, TexRectVertex_DescCount, TexRectVertex_Descs, sizeof(TexRectVertex), idx);
	}
}

///

void PaintEffectRect(DJTexture* pTexture, DJMaterial* pEffectMat, const DJVector3 &vPos, const DJVector2 &vSize, djuint32 dwCol1, djuint32 dwCol2, djuint32 dwCol3, djuint32 dwCol4, djbool bFlipHoriz)
{
	pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
	pTheRenderDevice->SetMaterial(pEffectMat);
	if (pEffectMat->GetCurrentRenderMode() == NULL)
		return;
	pTheRenderDevice->UpdateUniforms(pEffectMat->GetCurrentRenderMode()->GetLocalUniformList());
	//pTheRenderDevice->EnableAlphaBlend(DJTRUE);
	//pTheRenderDevice->SetAlphaBlendMode(DJBLEND_ONE, DJBE
	if (pTexture)
		pTheRenderDevice->SetTexture(pTexture);

	TexRectVertex vtx[4];
	vtx[0].x = vPos[0];
	vtx[0].y = vPos[1];
	vtx[0].z = vPos[2];
	vtx[0].dwCol = dwCol1;
	vtx[1].x = vtx[0].x;
	vtx[1].y = vtx[0].y + vSize[1];
	vtx[1].z = vtx[0].z;
	vtx[1].dwCol = dwCol2;
	vtx[2].x = vtx[0].x + vSize[0];
	vtx[2].y = vtx[0].y + vSize[1];
	vtx[2].z = vtx[0].z;
	vtx[2].dwCol = dwCol3;
	vtx[3].x = vtx[0].x + vSize[0];
	vtx[3].y = vtx[0].y;
	vtx[3].z = vtx[0].z;
	vtx[3].dwCol = dwCol4;
	if (bFlipHoriz)
	{
		vtx[0].u = 1;
		vtx[0].v = 0;
		vtx[1].u = 1;
		vtx[1].v = 1;
		vtx[2].u = 0;
		vtx[2].v = 1;
		vtx[3].u = 0;
		vtx[3].v = 0;
	}
	else
	{
		vtx[0].u = 0;
		vtx[0].v = 0;
		vtx[1].u = 0;
		vtx[1].v = 1;
		vtx[2].u = 1;
		vtx[2].v = 1;
		vtx[3].u = 1;
		vtx[3].v = 0;
	}
	pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleFan, 2, vtx, TexRectVertex_DescCount, TexRectVertex_Descs, sizeof(TexRectVertex));
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Application class
DJWarsicApplication::DJWarsicApplication()
{
	DJTrace("%s()", __FUNCTION__);	
	djMemSet(m_pMenus, 0, sizeof(m_pMenus));
	m_nGameState	= GS_PRELOAD; 
	m_bGameIsLoaded	= DJFALSE;
	m_fStateTimer = 0.0f;
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

	// Randomize random seed by using system time
	djRandomSetSeed( (djuint32)(djGetSystemTimeFloat() * 65536.0f) );

	// Get initial size of screen
	pTheRenderDevice->GetScreenSize(g_nScreenWidth, g_nScreenHeight);
#if defined(FORCE_WIDESCREEN)
	if ((float)g_nScreenWidth/(float)g_nScreenHeight > 16.0f/9.0f) // Too wide
	{
		g_nScreenWidth = (djint32)(float)g_nScreenHeight*16.0f/9.0f;
	}
	else
	{
		g_nScreenHeight = (djint32)(float)g_nScreenWidth*9.0f/16.0f;
	}
#elif defined(FORCE_PLAYBOOK)
	if (g_nScreenWidth>=1024 && g_nScreenHeight>=600)
	{
		//g_nScreenWidth = 1024;
		g_nScreenHeight = (djint32)(600.0f*(float)g_nScreenWidth/(float)1024);
	}
#endif 
	djuint32 rsw,rsh;
    pTheRenderDevice->GetScreenSize(rsw,rsh);
	pTheRenderDevice->SetViewport(DJViewport(rsw/2-g_nScreenWidth/2,rsh/2-g_nScreenHeight/2,g_nScreenWidth,g_nScreenHeight));

	if(g_nScreenWidth == g_nScreenHeight)
	{
		g_UIViewport.SetViewport(0,0,720,720);
	}
	else if(g_nScreenWidth <= 480)
	{
		g_bUseSDUI = DJTRUE;
		g_UIViewport.SetViewport(0,0,800,(djint32)(520.0f*((float)(g_nScreenHeight)/(float)(g_nScreenWidth))/((float)UI_DESIGN_HEIGHT/(float)UI_DESIGN_WIDTH)));
	}
	else if(g_nScreenWidth < 800)
	{
		g_UIViewport.SetViewport(0,0,800,(djint32)(520.0f*((float)(g_nScreenHeight)/(float)(g_nScreenWidth))/((float)UI_DESIGN_HEIGHT/(float)UI_DESIGN_WIDTH)));
	}
	else
	{
		float fh = (((float)(g_nScreenHeight)/(float)(g_nScreenWidth))/((float)UI_DESIGN_HEIGHT/(float)UI_DESIGN_WIDTH));
		float fw = 1.0f;
		if (fh < 1.3356521739130434782608695652174f)
		{
			fw = 1.3356521739130434782608695652174f/fh;
			fh = 1.3356521739130434782608695652174f;
		}
		g_UIViewport.SetViewport(0,0,(djint32)(UI_DESIGN_WIDTH*fw),(djint32)(UI_DESIGN_HEIGHT*fh));

		if(g_nScreenWidth == 960 && g_nScreenHeight==544)
		{
			g_UIViewport.SetViewport(0,0,(djint32)(700.0f*960.0f/544.0f),700);
		}
		else if (g_nScreenWidth==1024 && g_nScreenHeight==600)
		{
			g_UIViewport.SetViewport(0,0,1194,700);
		}
		else if (g_nScreenWidth==854 && g_nScreenHeight==480)
		{
			g_UIViewport.SetViewport(0,0,1244,700);	  
		}
		else if (g_nScreenWidth==800 && g_nScreenHeight==480)
		{
			g_UIViewport.SetViewport(0,0,1167,700);
		}
		else if (g_nScreenWidth==1136 && g_nScreenHeight==640)
		{
			g_UIViewport.SetViewport(0,0,1242,700);
		}
		else if (g_nScreenWidth==1280 && g_nScreenHeight==768)
		{
			g_UIViewport.SetViewport(0,0,1166,700);
		}
		else if (g_nScreenWidth==1280 && g_nScreenHeight==720)
		{
			g_UIViewport.SetViewport(0,0,1244,700);
		}
		else if (g_nScreenWidth==960 && g_nScreenHeight==640)
		{
			g_UIViewport.SetViewport(0,0,(djint32)(700.0f*960.0f/640.0f),700);			
		}
	}

#if defined(_DJQNX) || defined (FORCE_PLAYBOOK) || defined (FORCE_WIDESCREEN)
	DJTexture* pTex;
	if (g_nScreenWidth == g_nScreenHeight)
		pTex = (DJTexture*)theResourceManager.GetResource("backgrounds/funkoi_splash_1_1.png", DJResource::TYPE_TEXTURE);
	else
		pTex = (DJTexture*)theResourceManager.GetResource("backgrounds/funkoi_splash_16_9.png", DJResource::TYPE_TEXTURE);
#else
	DJTexture* pTex;
	float fAspect = (float)g_nScreenWidth/(float)g_nScreenHeight;
	if (fAspect>1.6f)
		pTex = (DJTexture*)theResourceManager.GetResource("backgrounds/funkoi_splash_16_9.png", DJResource::TYPE_TEXTURE);
	else if (fAspect<1.1f)
		pTex = (DJTexture*)theResourceManager.GetResource("backgrounds/funkoi_splash_1_1.png", DJResource::TYPE_TEXTURE);
	else
		pTex = (DJTexture*)theResourceManager.GetResource("backgrounds/funkoi_splash.png", DJResource::TYPE_TEXTURE);
#endif

	if (pTex)
	{
		pTheRenderDevice->BeginFrame();
        pTheRenderDevice->EnableZBuffer(DJRenderDevice::flagEnableZComp | DJRenderDevice::flagEnableZWrite);
        pTheRenderDevice->SetClearColor(DJColor(0,0,0,1));
        pTheRenderDevice->Clear(DJRenderDevice::flagClearAll);
        pTheRenderDevice->EnableZBuffer(0);
        pTheRenderDevice->SetCullMode(0);
        pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
        pTheRenderDevice->SetViewTransform(DJMatrix::Identity());
        pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)g_nScreenWidth,(float)g_nScreenHeight,0.0f,100.0f);
		PaintTexRect(pTex, DJVector3(0,0,0), DJVector2((float)g_nScreenWidth, (float)g_nScreenHeight), 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		pTheRenderDevice->EndFrame();
		pTheRenderDevice->SwapBuffer();
		pTheRenderDevice->BeginFrame();
		pTheRenderDevice->Clear(DJRenderDevice::flagClearAll);
		PaintTexRect(pTex, DJVector3(0,0,0), DJVector2((float)g_nScreenWidth, (float)g_nScreenHeight), 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		pTheRenderDevice->EndFrame();
		pTheRenderDevice->SwapBuffer();
		DJ_SAFE_RELEASE(pTex);
	}

	// Set sound device to use threaded updates (so music can play and fade-in/out during loading)
	// Most games should use threaded sound updates.
	pTheSoundDevice->EnableThreadedUpdates(DJTRUE);

	// Create UI system
	pTheUI = DJ_NEW(DJUI);
	pTheUI->Init();
	pTheUI->SetMultitouchEnabled(DJFALSE);
	pTheUI->SetUIWidth(g_UIViewport.GetWidth());
	pTheUI->SetUIHeight(g_UIViewport.GetHeight());
	pTheUI->RegisterEventListener(this);

	//// Setup global 3D camera
	//g_pCamera = DJ_NEW(DJCamera);
	//if (g_pCamera == NULL)
	//{
	//	DJError("Failed to create camera (out of memory)!");
	//	return DJFALSE;
	//}
	//g_pCamera->SetNearDistance(10.0f);
	//g_pCamera->SetFarDistance(2000.0f);
	//g_pCamera->SetFOV(DJRADIANS(65.0f));
	//g_pCamera->SetAspect((float)g_nScreenWidth/(float)g_nScreenHeight);

	// Load ui text
	theLocalizer.AddTagFile("text/eng/menu.text");

	// Set global freetype font quality (to make sharper fonts for high resolution displays)
	// This needs to be set before we load any DJFTFont
    g_fGlobalFontQualityScale = (float)g_nScreenWidth/1024.0f;
	g_fGlobalFontQualityScale = DJ_MAX(g_fGlobalFontQualityScale, 0.5f);

	g_pFont = (DJFont*)theResourceManager.GetResource( "font", DJResource::TYPE_FONT );	

	// Initialize sprite engine
	theSpriteEngine.Init(LAYER_COUNT);

	// Initialize effect resource manager
	theEffectResourceManager.Init();

	return DJ_SUCCESS;
}

///


// Load application
djbool DJWarsicApplication::OnLoad()
{
	if (m_bGameIsLoaded)
	{
		return DJTRUE;
	}
	m_bGameIsLoaded = DJTRUE;
	// Load menu for menu system
	LoadMenu(MENU_MAIN, "menumain/menumain");
	return DJTRUE;
}

///

void DJWarsicApplication::OnTerm()
{
	DJTrace("%s()", __FUNCTION__);

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

	// Update UI
	pTheUI->OnUpdate();
	m_fStateTimer += m_fDeltaFrameTime;
	switch (m_nGameState)
	{
		case GS_PRELOAD:
			GotoGameState(GS_PRELOGO);
		break;
		case GS_PRELOGO:
			UpdatePreLogo();
		break;
		case GS_LOADGAME:
			UpdateLoadGame();
		break;
		case GS_MENU: 
		break;
	};
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

	pTheUI->OnPaint(rc);

	// Render all layers (and sprites)
	theSpriteEngine.OnPaint(rc);

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

///

djbool DJWarsicApplication::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	return DJFALSE;
}

///

// Check if requested menu is already on stack
djbool DJWarsicApplication::IsMenuOnStack(djuint32 uMenu)
{
	for (int q=0; q<m_menuStack.GetCurrentIndex(); q++)
	{
		if (m_menuStack[q] == uMenu)
		{
			return DJTRUE;
		}
	}
	return DJFALSE;
}

///

// Goto specified menu, replace whole stack if requested, replace current menu if requested or push on top of stack
void DJWarsicApplication::GotoMenu(djuint32 uMenu, djbool bReplaceStack, djbool bReplaceLast)
{
	djuint32 uCurrentMenu = GetCurrentMenu();

	if (bReplaceStack)
	{
		m_menuStack.SetCurrentIndex(0);
	}
	else if (bReplaceLast)
	{
		m_menuStack.Pop();
	}

	m_menuStack.Push(uMenu);
	//m_uLastMenu = m_uCurrentMenu;
	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	//m_uCurrentMenu = uMenu;
	if (uMenu != MENU_INVALID)
	{
		if (m_pMenus[uMenu]->IsNodeVisible())
		{
			m_pMenus[uMenu]->ShowNode(DJFALSE);
		}
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}
}

///

// Pop topmost menu from stack
djuint32 DJWarsicApplication::PopMenu()
{ 
	djuint32 uCurrentMenu = GetCurrentMenu();
	djuint32 uMenu	= GetLastMenu();
	djuint32 uRet = m_menuStack.Pop();

	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	if (uMenu != MENU_INVALID)
	{
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}

	return uRet;
}

///

// Go back on stack to requested menu
void DJWarsicApplication::GoBackToMenu(djuint32 uMenu)
{
	djuint32 uCurrentMenu = GetCurrentMenu();

	djbool bFound = DJFALSE;
	for (int q=m_menuStack.GetCurrentIndex()-1; q>=0; q--)
	{
		if (m_menuStack[q] == uMenu)
		{
			m_menuStack.SetCurrentIndex(q+1);
			bFound = DJTRUE;
			break;
		}
	}
	if (!bFound)
	{
		m_menuStack.SetCurrentIndex(0);
		m_menuStack.Push(uMenu);
	}

	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	if (uMenu != MENU_INVALID)
	{
		if (m_pMenus[uMenu]->IsNodeVisible())
		{
			m_pMenus[uMenu]->ShowNode(DJFALSE);
		}
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}
}

///

void DJWarsicApplication::LoadMenu(djuint32 uMenuID, const char* strMenuName)
{
	if(uMenuID == MENU_INVALID)
		return;
	m_pMenus[uMenuID] = (DJUINode*)theResourceManager.GetResource(strMenuName, DJResource::TYPE_UINODE);
	if (m_pMenus[uMenuID])
	{
		m_pMenus[uMenuID]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[uMenuID]);
		m_pMenus[uMenuID]->AddToParent(pTheUI->GetRootNode());
	}
}

///

// Go to another game state
djbool DJWarsicApplication::GotoGameState(djint nState)
{
	djbool bRet = (nState != m_nGameState);
	m_bGameStateChanged = bRet;

	// Deactivate old state
	switch (m_nGameState)
	{
		case GS_PRELOGO:
		case GS_LOADGAME:
		case GS_UNLOAD_LEVEL:
		case GS_MENU:
			if (m_pMenus[GetCurrentMenu()])
			{
				if(GetCurrentMenu() != MENU_INVALID)
					m_pMenus[GetCurrentMenu()]->EnableNode(DJFALSE);				
			}
		break;	

		case GS_LOAD_LEVEL:
		if (m_pMenus[GetCurrentMenu()])
		{
			if(GetCurrentMenu() != MENU_INVALID)
				m_pMenus[GetCurrentMenu()]->EnableNode(DJFALSE);				
		}
		break;
	}

	if (pTheUI)
	{
		pTheUI->SetUIWidth(g_UIViewport.GetWidth());
		pTheUI->SetUIHeight(g_UIViewport.GetHeight());
	}
	
	// Activate new state
	switch (nState)
	{
		case GS_PRELOGO:
		{
			/// ::TODO
		}
		break;
		case GS_LOAD_LEVEL:
		case GS_UNLOAD_LEVEL:
		case GS_LOADGAME:
		{
			/// ::TODO
		}
		break;
		case GS_MENU:
		{
			m_pMenus[GetCurrentMenu()]->ShowNode(DJTRUE);
			m_pMenus[GetCurrentMenu()]->EnableNode(DJTRUE);
		}
		break;	
	
	}

	m_fStateTimer = 0.0f;
	m_nGameState = nState;	 
	return bRet;
}

/////////////////////////////////////////////////////////////////
// Update
void DJWarsicApplication::UpdateLoadGame()
{
	g_fGameTimeScale = (djClampFloat(1.5f-m_fStateTimer,0.05f,1));	
	
	if (m_fStateTimer > 1.5f && !m_bGameIsLoaded)
	{
		if (!OnLoad())
		{
			DJError("Failed to load application!!!");
		}
	}
	if (m_bGameIsLoaded && m_fStateTimer > 3.0f)
	{
		GotoMenu(MENU_MAIN);
		GotoGameState(GS_MENU);
	}
}

///

void DJWarsicApplication::UpdatePreLogo()
{
	GotoGameState(GS_LOADGAME);
}

///

void DJWarsicApplication::UpdateLoadLevel()
{
	/// ::TODO
}

///

void DJWarsicApplication::UpdateIngame()
{
	/// ::TODO
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Paint
void DJWarsicApplication::PaintLoadGame()
{
	float fAnim;
	if (m_bGameIsLoaded && m_fStateTimer > 2.0f)
	{
		fAnim = m_fStateTimer-2.0f;
		fAnim = djSin(DJ_PIHf * DJ_MIN(fAnim*2.0f, 1.0f));
	}
	else
	{
		fAnim = 1.0f-djSin(DJ_PIHf * DJ_MIN(m_fStateTimer*2.0f, 1.0f));
	}
	g_pFont->DrawString( theLocalizer.GetText("MENU_PLEASEWAIT"), DJVector3( 500.0f*fAnim + (float)g_UIViewport.GetWidth()-54, (float)g_UIViewport.GetHeight()-54, 0.0f ), DJVector2( 40, 40 ), DJColor(1,1,1,1.0f-fAnim).ABGR8888(), DJFont::ALIGN_RIGHT | DJFont::ALIGN_BOTTOM );
}

///

void DJWarsicApplication::PaintPreLogo()
{
	/// ::TODO
}

///

void DJWarsicApplication::PaintLoadLevel()
{
	/// ::TODO
}

///

void DJWarsicApplication::PaintIngame()
{
	/// ::TODO
}
/////////////////////////////////////////////////////////////////

// Application class
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

