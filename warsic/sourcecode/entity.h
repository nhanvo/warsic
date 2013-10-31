/////////////////////////////////////////////////////////////////

#ifndef _ENTITY_H_
#define _ENTITY_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2d.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "util.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
class Entity
{
protected:
	DJVector2 m_vPos;
	DJVector2 m_vBoxPos;
	DJVector2 m_vVel;
	DJVector2 m_vAccel;
public:
	Entity()			{}
	virtual ~Entity()	{}

public:
	
	// Forces the entity to move in a direction
	virtual void Move(const DJVector2 &vDelta);
	
	// Something the entity is standing on is falling
	virtual void Fall(const DJVector2 &vDelta);

	virtual void SetPosition(const DJVector2 &vPos) { m_vPos = vPos; }
	virtual const DJVector2 &GetPosition() const { return m_vPos; }
	virtual void SetBoxPosition(const DJVector2 &vPos)	{ m_vBoxPos = vPos; }

protected:
	friend class EntityShadow;
};

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif // _ENTITY_H_