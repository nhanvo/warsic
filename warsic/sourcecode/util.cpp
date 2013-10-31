/////////////////////////////////////////////////////////////////
//
// util.cpp
// Util helper class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2dutil.h>
#include <djfont.h>
#include <djosfile.h>
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Game Includes
#include "util.h"
#include "gamedef.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
djint32 StringToEnum(const char *szEnumStr, const char **lpszEnumStrs, djint32 nCount)
{
	DJString str(szEnumStr);
	for (djint32 i = 0; i < nCount; i++)
	{
		if (str == lpszEnumStrs[i])
			return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

djbool FileExists(const char *szFileName)
{
	DJFile file;
	if (file.Open(szFileName, DJFile::in))
	{
		file.Close();
		return DJTRUE;
	}
	return DJFALSE;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

djfloat GetUnitStep(djfloat fTime, const DJVector2 &vVel)
{
	float fMaxComponent = DJMAX(
			djFloatAbs(vVel.e[0]), djFloatAbs(vVel.e[1]));
	float fStep;
	if (fMaxComponent > DJ_EPSf)
	{
		fStep = 1.0f / fMaxComponent;
		if (fTime < fStep)
			fStep = fTime;
	}
	else
	{
		fStep = fTime;
	}
	if (fStep < DJ_EPSf)
		fStep = fTime;
	return fStep;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

djint32 AreOverlapped(const DJRECT &box1, const DJRECT &box2, djint32 nDir)
{
	if ((box1.nW == 0) || (box1.nH == 0) || (box2.nW == 0) || (box2.nH == 0))
		return DJFALSE;
	djint32 nDeltaR, nDeltaB, nDeltaL, nDeltaT; 
	nDeltaR = box1.nX + box1.nW - 1 - box2.nX;
	if(nDeltaR < 0) return COLIDE_VALID;
	//if ((box1.nX + box1.nW - 1) < box2.nX) return DJFALSE;
	nDeltaB = box1.nY + box1.nH - 1 - box2.nY;
	if(nDeltaB < 0) return COLIDE_VALID;
	//if ((box1.nY + box1.nH - 1) < box2.nY) return DJFALSE;
	nDeltaL = (box2.nX + box2.nW - 1) - box1.nX;
	if(nDeltaL <= 0) return COLIDE_VALID;
	//if (box1.nX > (box2.nX + box2.nW - 1)) return DJFALSE;
	nDeltaT = (box2.nY + box2.nH - 1) - box1.nY;
	if(nDeltaT <= 0) return COLIDE_VALID;
	//if (box1.nY > (box2.nY + box2.nH - 1)) return DJFALSE;

	nDeltaR = DJMIN(nDeltaR, nDeltaL);
	nDeltaB = DJMIN(nDeltaT, nDeltaB);
	// Only get collition on top, otherwise dont care
	
	if(nDir >= 0)
		nDeltaT = box1.nH / 2;
	else
		nDeltaT = box1.nH / 8;
	if((box2.nY > box1.nY) && (nDeltaB <= nDeltaT))
		return COLIDE_TOP;
	else if(nDeltaR >= nDeltaB)
		return COLIDE_BOTTOM;
	else if(box1.nX < box2.nX)
		return COLIDE_LEFT;
	else
		return COLIDE_RIGHT;
	//if(nDeltaR*1.1f >= nDeltaB)
	//{
	//	if(box1.nY > box2.nY)
	//		return COLIDE_BOTTOM;
	//	else
	//		return COLIDE_TOP;
	//}
	//else
	//{
	//	if(box1.nX > box2.nX)
	//	return COLIDE_LEFT;
	//	else
	//		return COLIDE_RIGHT;
	//}

	//return DJTRUE;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJPOINT* ToPoint(DJPOINT *pPos, const DJVector2 &vPos)
{
	pPos->nX = djTruncToInt(vPos.e[0]);
	pPos->nY = djTruncToInt(vPos.e[1]);
	return pPos;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJRECT* MakeBox(DJRECT *pRect, const DJVector2 &vBottom,
				djint32 nWidth, djint32 nHeight)
{
	pRect->nX = djTruncToInt(vBottom.e[0] - (nWidth + 1) / 2);
	pRect->nY = djTruncToInt(vBottom.e[1] - nHeight + 1);
	pRect->nW = nWidth;
	pRect->nH = nHeight;
	return pRect;
}

///

DJRECT ToBox(const DJQuaternion &q)
{
	return DJRECT(djTruncToInt(q.x()), djTruncToInt(q.y()), djTruncToInt(q.z()), djTruncToInt(q.scalar()));
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJRECT* MakeBoxCenter(DJRECT *pRect, const DJVector2 &vCenter,
					  djint32 nWidth, djint32 nHeight)
{
	pRect->nX = djTruncToInt(vCenter.e[0] - (nWidth + 1) / 2);
	pRect->nY = djTruncToInt(vCenter.e[1] - (nHeight + 1) / 2);
	pRect->nW = nWidth;
	pRect->nH = nHeight;
	return pRect;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

void LoadAnimation(DJ2DAnimation *&pAnimation, const char *lpszName, bool bLenient)
{
	if (bLenient && (pAnimation != NULL)) return;
	pAnimation = (DJ2DAnimation*)theResourceManager.GetResource(lpszName, DJResource::TYPE_ANIMATION2D);
	DJAssert(pAnimation != NULL);
	DJAssert(pAnimation->GetNumberOfFrames() > 0);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void LoadAnimations(DJ2DAnimation **pAnimations,
					const char **lpszNames,
					djint32 nCount,
					bool bLenient)
{
	for (djint32 i = 0; i < nCount; i++)
		LoadAnimation(pAnimations[i], lpszNames[i], bLenient);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void PlayAnimation(DJ2DSprite *pSprite, DJ2DAnimation *pAnimation)
{
	if (pSprite->GetAnimation() != pAnimation)
		pSprite->PlayAnimation(pAnimation);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

djint32 GetRandom(djint32 min, djint32 max)
{
	DJ_ASSERT(min <= max);
	return min + djRandomGetDword() % (max - min + 1);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
ActiveMapAreaTracker theActiveMapAreaTracker;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void ActiveMapAreaTracker::Init(float fLookAhead, float fLookBehind)
{
	m_xMax = 0.0f;
	m_xMin = 0.0f;
	m_fLookAhead = fLookAhead;
	m_fLookBehind = fLookBehind;
}

///

void ActiveMapAreaTracker::Term()
{
}

///

void ActiveMapAreaTracker::UpdatePosition(const DJVector2 &vPos)
{
	m_xMin = vPos[0] - m_fLookBehind;
	m_xMax = vPos[0] + m_fLookAhead;
}

///

bool ActiveMapAreaTracker::IsInActiveArea(const DJVector2 &vPos)
{
	return (vPos[0] <= m_xMax) && (vPos[0] >= m_xMin);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
float QUAD_DX[6] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
float QUAD_DY[6] = { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

void FillQuadVertices(RectVertex *pVertices,
					  const DJVector2 &vTopLeft,
					  const DJVector2 &vSize,
					  const DJColor &color,
					  float fZ,
					  const DJMatrix &tm,
					  const DJVector2 &vTopLeftUV,
					  const DJVector2 &vBottomRightUV)
{
	for (djint32 i = 0; i < 6; i++)
	{
		DJVector3 v(vTopLeft[0] + QUAD_DX[i] * vSize[0],
					vTopLeft[1] + QUAD_DY[i] * vSize[1],
					fZ);
		v = tm * v;
					
		pVertices[i].x = v[0];
		pVertices[i].y = v[1];
		pVertices[i].z = v[2];
		pVertices[i].c = color;
		pVertices[i].u = vTopLeftUV[0] + QUAD_DX[i] * (vBottomRightUV[0] - vTopLeftUV[0]);
		pVertices[i].v = vTopLeftUV[1] + QUAD_DY[i] * (vBottomRightUV[1] - vTopLeftUV[1]);
	}
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

//Circle* MakeCircle(Circle *pCircle, const DJVector2 &vCenter, djuint uRadius)
//{
//	ToPoint( &pCircle->pointCenter, vCenter);
//	pCircle->uRadius = uRadius;
//	return pCircle;
//}
//
/////
//
//djbool AreOverlappedBoxAndCircle(const DJRECT &box, const Circle &circle/*, DJVector2 &vPosColie*/)
//{
//	djfloat a__ = 1.0f;
//	djfloat b__ = - 2 * (djfloat)(circle.pointCenter.nX);
//	djfloat c__ = djPow(((float)box.nY + (float)box.nH),2) - 2 * (djfloat) circle.pointCenter.nY * (box.nY + box.nH) + ( djPow((djfloat)circle.pointCenter.nX,2) + djPow((djfloat)circle.pointCenter.nY,2) - djPow((djfloat)circle.uRadius,2) );
//
//	DJVector2 vPosColie;
//	djfloat a = 1.0f;
//	djfloat b = - 2 * (djfloat)circle.pointCenter.nX;
//	djfloat c = djPow((float)box.nY,2) - 2 * (djfloat) circle.pointCenter.nY * box.nY + ( djPow((djfloat)circle.pointCenter.nX,2) + djPow((djfloat)circle.pointCenter.nY,2) - djPow((djfloat)circle.uRadius,2) );
//	if (CalculateDelta(a,b,c) || CalculateDelta(a__,b__,c__))
//	{
//		if ( CalculateDelta(a,b,c) && !CalculateDelta(a__,b__,c__) )
//		{
//					vPosColie[1] = (float)box.nY;
//		} else if (CalculateDelta(a__,b__,c__))
//		{
//			vPosColie[1] = (float)(box.nY + box.nH);
//		} else 
//			vPosColie[1] = (float)(box.nY+ box.nH/2);
//
//		djfloat a_ = 1.0f;
//		djfloat b_ = - 2 * (djfloat)circle.pointCenter.nY;
//		djfloat c_ = djPow((float)(box.nX + box.nW - 1),2) - 2 * (djfloat) circle.pointCenter.nX * (box.nX + box.nW - 1) + ( djPow((djfloat)circle.pointCenter.nX,2) + djPow((djfloat)circle.pointCenter.nY,2) - djPow((djfloat)circle.uRadius,2) );
//		if (CalculateDelta(a_,b_,c_))
//		{
//			vPosColie[0] = (float)(box.nX + box.nW - 1);
//			return DJTRUE;
//		}
//	}
//	return DJFALSE;
//}
//
/////
//
//djbool CalculateDelta(djfloat a, djfloat b, djfloat c)
//{
//	if ( (djPow(b,2) - 4*a*c) >= 0 )
//	{
//		return DJTRUE;
//	}
//	return DJFALSE;
//}
//
/////
//
//djbool AreOverlappedCircle(const Circle &circle1, const Circle &circle2)
//{
//	DJVector2 vDist1 = DJVector2((djfloat)circle1.pointCenter.nX - (djfloat)circle2.pointCenter.nX, (djfloat)circle1.pointCenter.nY - (djfloat)circle2.pointCenter.nY);
//	djfloat fDist1 = vDist1.GetNorm();
//	djfloat fDist2 = (djfloat)circle1.uRadius + (djfloat)circle2.uRadius;
//	if ( fDist1 <= fDist2 )
//	{
//		return DJTRUE;
//	}
//	return DJFALSE;
//}
//
/////
//
//djfloat CalculateAngle( const DJVector2 &vV1, const DJVector2 &vV2 )
//{
//	DJVector2 vTemp = DJVector2(vV1[0] + 50, vV1[1] );
//	djfloat fDis_a = (djPow(vV1.e[0] - vV2.e[0], 2) + djPow( vV1.e[1] - vV2.e[1], 2 ));
//	djfloat fDis_b = (djPow(vV1.e[0] - vTemp.e[0], 2) + djPow( vV1.e[1] - vTemp.e[1], 2 ));
//	djfloat fDis_c = (djPow(vTemp.e[0] - vV2.e[0], 2) + djPow( vTemp.e[1] - vV2.e[1], 2 ));
//
//	djfloat fCos = ((fDis_a + fDis_b) - fDis_c ) / ( 2 * djSquareRoot(fDis_a) * djSquareRoot(fDis_b) );
//	djfloat fAngle = djArcCos( fCos );
//	if ( vV1[1] > vV2[1] ) 
//	{
//		fAngle = DJRADIANS( 360 ) - fAngle;
//	}
//	return fAngle;
//}
//
/////
//
//DJMatrix2D GetAffineInverse(const DJMatrix2D &tm)
//{
//	DJVector2 vtmp0, vtmp1, vtemp2;
//	vtmp0 = tm.GetBaseX();
//	vtmp1 = tm.GetBaseY();
//	vtemp2 = tm.GetTranslation();
//	float fDetermainant = 1.0f / (vtmp0[0] * vtmp1[1] - vtmp0[1] * vtmp1[0]);
//
//	return DJMatrix2D(fDetermainant * vtmp1[1], - fDetermainant * vtmp0[1], -fDetermainant * vtmp1[0], fDetermainant * vtmp0[0], 
//		fDetermainant * (vtmp1[0] * vtemp2[1] - vtmp1[1] * vtemp2[0]), fDetermainant * (vtmp0[1] * vtemp2[0] - vtmp0[0] * vtemp2[1]));
//}

///

AnimationList::AnimationList()
{
}

///

AnimationList::~AnimationList()
{
	Term();
}

///

void AnimationList::Init(const char *szFileName)
{
	DJTagFile tagfile;
	if(tagfile.Load(szFileName) != DJ_SUCCESS)
	{
		DJError("Failed to find animation list file!");
		return;
	}

	DJString sStr;
	DJString sStr2;
	DJTagLine* pLine = tagfile.GetTag("ANIMATION");
	while (pLine)
	{
		if(!pLine->GetArgString(0, sStr) || !pLine->GetArgString(1, sStr2))
		{
			DJWarning("Failed to parse animation");
		}

		Animation* pAnim = new Animation;
		pAnim->m_sName = sStr;
		pAnim->m_pAnimation = (DJ2DAnimation*)theResourceManager.GetResource(sStr2, DJResource::TYPE_ANIMATION2D);
		if(pAnim->m_pAnimation == NULL)
		{
			DJWarning("Failed to parse %s animation", (const char*)pAnim->m_sName);
		}

		m_Animations.Add(pAnim);

		pLine = pLine->m_parent->GetNextTag(pLine);
	}
}

///

void AnimationList::Term()
{
	m_Animations.Clear(DJTRUE);
}

///

DJ2DAnimation* AnimationList::GetAnimation(const char *pName)
{
	for(djint32 i = 0; i < m_Animations.GetCurrentIndex(); i++)
	{
		if(m_Animations[i]->m_sName == pName)
		{
			return m_Animations[i]->m_pAnimation;
		}
	}
	return NULL;
}

///

DJ2DAnimation* AnimationList::GetAnimation(djint32	nIndex)
{
	return m_Animations[nIndex]->m_pAnimation;
}

///

djint32 AnimationList::GetFistIndex(const char *pName)
{
	for(djint32 i = 0; i < m_Animations.GetCurrentIndex(); i++)
	{
		if(m_Animations[i]->m_sName == pName)
		{
			return i;
		}
	}
	return -1;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////