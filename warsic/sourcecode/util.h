/////////////////////////////////////////////////////////////////
#ifndef _UTIL_H_
#define _UTIL_H_
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2d.h>
#include <djconsole.h>
#include <djmatrix2D.h>
#include <dj2dutil.h>
#include <djlink.h>
/////////////////////////////////////////////////////////////////
// Game Includes

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

// Returns -1 if no match found
djint32 StringToEnum(const char *szEnumStr, const char **lpszEnumStrsm, djint32 nCount);

///

djbool FileExists(const char *szFileName);

///

float GetUnitStep(float fTime, const DJVector2 &vVel);

///

// Determines if two boxes overlap each other, i.e. collision.
djint32 AreOverlapped(const DJRECT &box1, const DJRECT &box2, djint32 nDir);

DJPOINT* ToPoint(DJPOINT *pPos, const DJVector2 &vPos);

DJRECT* MakeBox(DJRECT *pRect, const DJVector2 &vBottom,
				djint32 nWidth, djint32 nHeight);

DJRECT* MakeBoxCenter(DJRECT *pRect, const DJVector2 &vCenter,
					  djint32 nWidth, djint32 nHeight);

DJRECT ToBox(const DJQuaternion &q);
///

void LoadAnimation(DJ2DAnimation *&pAnimation, const char *lpszName, bool bLenient = false);
void LoadAnimations(DJ2DAnimation **pAnimations, const char **lpszNames, djint32 nCount, bool bLenient = false);
void ReleaseAnimation(DJ2DAnimation *&pAnimations);
void ReleaseAnimations(DJ2DAnimation **pAnimations, djint32 nCount);

void PlayAnimation(DJ2DSprite *pSprite, DJ2DAnimation *pAnimation);

///

// Gets a int random
djint32 GetRandom(djint32 min, djint32 max);

///

// A simple utility to keep track of which area of the map is actives.
// Only objects positioned inside this active area should be considered
// for collision detection/updating.
class ActiveMapAreaTracker
{
protected:
	float			m_fLookAhead;
	float			m_fLookBehind;

	float			m_xMax;
	float			m_xMin;
public:
	void Init(float fLookAhead, float fLookBehind);
	void Term();

	void UpdatePosition(const DJVector2 &vPos);
	bool IsInActiveArea(const DJVector2 &vPos);
};
extern ActiveMapAreaTracker theActiveMapAreaTracker;

///

struct RectVertex
{
	float x,y,z;
	DJColor c;
	float u,v;
};

///

static DJVertexBuffer::DJVERTEX_DESC RectVertex_Descs[3] =
{
	DJVertexBuffer::DJVERTEX_DESC(0, DJVertexBuffer::typeFloat3, "pos_attr", INDEX_POS_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(12, DJVertexBuffer::typeFloat4, "col_attr", INDEX_COL_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(28, DJVertexBuffer::typeFloat2, "uv_attr", INDEX_UV_ATTR, 0),
};

///

static const djuint32 RectVertex_DescCount = 3;

///

void FillQuadVertices(RectVertex *pVertices,
					  const DJVector2 &vTopLeft,
					  const DJVector2 &vSize,
					  const DJColor &color,
					  djfloat fZ = 0.0f,
					  const DJMatrix &tm = DJMatrix::Identity(), 
					  const DJVector2 &vTopLeftUV = DJVector2(0.0f),
					  const DJVector2 &vBottomRightUV = DJVector2(1.0f));

///
//
//struct Circle
//{
//	DJPOINT pointCenter;
//	djuint  uRadius;
//};
//
/////
//
//struct CircleVertex
//{
//	float x,y,z;
//	DJColor c;
//	float u,v;
//};
//
/////
//
//static DJVertexBuffer::DJVERTEX_DESC CircleVertex_Descs[3] =
//{
//	DJVertexBuffer::DJVERTEX_DESC(0, DJVertexBuffer::typeFloat3, "pos_attr", INDEX_POS_ATTR, 0),
//	DJVertexBuffer::DJVERTEX_DESC(12, DJVertexBuffer::typeFloat4, "col_attr", INDEX_COL_ATTR, 0),
//	DJVertexBuffer::DJVERTEX_DESC(28, DJVertexBuffer::typeFloat2, "uv_attr", INDEX_UV_ATTR, 0),
//};
//
/////
//
//static const djuint32 CircleVertex_DescCount = 3;

//////////

//Circle* MakeCircle(Circle *pCircle, const DJVector2 &vCenter, djuint uRadius);

//djbool AreOverlappedBoxAndCircle(const DJRECT &box, const Circle &circle);

//djbool CalculateDelta(djfloat a, djfloat b, djfloat c);

//djbool AreOverlappedCircle(const Circle &circle1, const Circle &circle2);

//djfloat CalculateAngle( const DJVector2 &vV1, const DJVector2 &vV2 );

//////////
//Matrix2D
//DJMatrix2D GetAffineInverse(const DJMatrix2D &tm);

///////
class Animation
{
public:
	DJString		m_sName;
	DJ2DAnimation*	m_pAnimation;
	//
	Animation()		{ m_pAnimation = NULL; }
	~Animation()	{ DJ_SAFE_RELEASE(m_pAnimation);}
};

///

class AnimationList
{
protected:
	DJArray<Animation*>		m_Animations;
public:
	AnimationList();
	~AnimationList();
	//
	void Init(const char *szFileName);
	//
	void Term();
	//
	DJ2DAnimation* GetAnimation(const char* pName);
	DJ2DAnimation* GetAnimation(djint32	nIndex);
	//
	djint32 GetFistIndex(const char* pName);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif //_UTIL_H_