#include "phc.h"
#include "InitApp.h"
#include "DXErrorHandler.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		MessageBox(0, "wellcome", "welcome", MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(0, "helloworld", "HelloWind", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
} 
InitApp::InitApp(HINSTANCE& hInstance) : m_className("D3D_Pro"), 
							m_wc({ 0 }), m_hwnd(nullptr) , 
							m_renderer(nullptr), m_rc(nullptr)
{
#pragma region ´°¿ÚwindowÃèÊö 
	m_wc.cbSize = sizeof(m_wc);
	m_wc.style = CS_OWNDC;
	m_wc.lpfnWndProc = WndProc;
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = hInstance;
	m_wc.hIcon = nullptr;
	m_wc.hCursor = nullptr; 
	m_wc.hbrBackground = nullptr;
	m_wc.lpszMenuName = nullptr;
	m_wc.lpszClassName = m_className;
	m_wc.hIconSm = nullptr;
	RegisterClassEx(&m_wc);

	//WindowInstance 
	HWND hwnd = CreateWindowEx(
		0, m_className, "D3D_Pro",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr,
		hInstance, nullptr
	);

	if (hwnd == 0)
	{
		MessageBox(0, "CreateWindow FALLED", 0, 0);
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd); 
#pragma endregion 


	m_rc = new CRenderCommand(m_hwnd);
	m_renderer = new CRenderer(*m_rc); 
}
InitApp::~InitApp()
{
	delete m_rc; 
	delete m_renderer; 
}

void InitApp::Update(float dt)
{ 
	m_rc->Clear();
	m_renderer->BeginScene();
	m_renderer->EndScene();
	m_rc->Swap();
}
void InitApp::Update()
{
}