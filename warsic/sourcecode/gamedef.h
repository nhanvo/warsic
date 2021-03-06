/////////////////////////////////////////////////////////////////
//
// gamedef.h
// Game definitions
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
#ifndef _GAMEDEF_H_
#define _GAMEDEF_H_

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

// UI width
#define UI_DESIGN_WIDTH				1024
// UI height
#define UI_DESIGN_HEIGHT			575

enum
{
	COLIDE_VALID = -1,
	COLIDE_TOP,
	COLIDE_RIGHT,
	COLIDE_BOTTOM,
	COLIDE_LEFT,
};

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _GAMEDEF_H_