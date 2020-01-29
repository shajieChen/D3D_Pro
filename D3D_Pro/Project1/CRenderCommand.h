#pragma once 
#include "CDXObjects.h"


class RenderCommand {
public:
	RenderCommand(DXObjects dxObjects);
	void Clear() const;
	void Swap() const;

	/**/
	void DrawTriangle();
 
private	: 
	DXObjects m_dxo;


};