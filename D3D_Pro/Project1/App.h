#pragma once
#include "CDXObjects.h"
#include "CRenderer.h"
#include "CRenderCommand.h"
#include "Example.h" 
class App
{
public:
	App(HINSTANCE& hInstance);
	~App(); 
	void Update(float dt); 
	HWND getHwnd() const { return m_hwnd; }
private: 
	void initWindow(HINSTANCE& hInstace); 
	void initDirectX11(); 
	void initImGui();  
private:
	HWND m_hwnd; 
	const char* m_className;


	DXObjects m_dxo; 
	std::unique_ptr<Renderer> m_renderer; 
	std::unique_ptr<RenderCommand> m_rc;

	std::unique_ptr<example::IExample> m_example; 
};

