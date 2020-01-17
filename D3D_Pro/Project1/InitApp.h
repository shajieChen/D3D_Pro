#pragma once
#include "CRenderer.h"
#include "CRenderCommand.h"

class InitApp
{


public: 
	InitApp(HINSTANCE& hInstance); 
	~InitApp(); 
	void Update();
	void Update(float dt); //每一帧调用
	HWND getHwnd() const { return m_hwnd; }
private:
	HWND m_hwnd; 
	WNDCLASSEX m_wc; 
	const char* m_className; 
	CRenderCommand* m_rc; 
	CRenderer* m_renderer; 
};

