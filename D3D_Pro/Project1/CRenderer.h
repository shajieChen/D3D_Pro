#pragma once
#include "CRenderCommand.h"

class CRenderer
{
public:
	CRenderer(CRenderCommand& rc); 
	~CRenderer(); 

	void BeginScene(); 
	void EndScene(); 
	void Submit();
private : 
	CRenderCommand& m_rc;
};

