
/////////////////////////////////////////////////////////////////
//
// mainmenu.h
// Main menu class
// 
/////////////////////////////////////////////////////////////////

#ifndef _MAINMENU_H_
#define _MAINMENU_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djuinode.h>
#include <djapp.h>
#include <dj2d.h>

/////////////////////////////////////////////////////////////////
// Game Includes

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJMainMenuPageUINode : public DJPageUINode
{
	IMPLEMENT_INTERFACE(MainMenuPageUINode, PageUINode)
protected:
	
public:
	/// Constructor
	DJMainMenuPageUINode();
	/// Destructor
	virtual ~DJMainMenuPageUINode();

	/// Paint (virtual function implementation)
	virtual void OnPaint(const DJ2DRenderContext &rc);
	/// Show node (virtual function implementation)	
	virtual void OnShowNode(djbool bShow, djbool bParentChanged);

	/////////////////////////////////////////////////////////////////
	// Instancing / Copying

	/// Create Instance (virtual)
	virtual DJUINode* CreateInstance()				{ return DJ_NEW(DJMainMenuPageUINode); }
	/// Instanciate Node (virtual)
	virtual void InstanciateNode(DJUINode *pNode);
	/// Copy Node (virtual)
	virtual void CopyNode(DJUINode *pNode);
	/// Build node virtual call
	virtual djint32 Build();

	/// Virtual overload to create node from tagfile (remember to call superclass versions to load everything!)
	virtual djbool LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance);

	/// Touch begin callback
	virtual djint32 OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch move callback
	virtual djint32 OnTouchMove( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y );

	/// On UI event callback
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);

	/// On Tick
	virtual djbool OnTick();
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif //_MAINMENU_H_