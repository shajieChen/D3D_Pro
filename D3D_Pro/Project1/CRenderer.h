#pragma once
#include "CRenderCommand.h"
#include "CDXObjects.h"
class Renderer
{
public:
	Renderer(DXObjects dxObjects);
	~Renderer(); 

	void BeginScene(); 
	void Submit(); 
	void EndScene(); 
private: 
	DXObjects m_dxo; 
};

