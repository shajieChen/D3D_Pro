#include "phc.h"
#include "InitApp.h"
#include "DXErrorHandler.h"
using namespace DX;
#pragma comment (lib, "D3D11.lib") 

int CALLBACK WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	try
	{ 
		InitApp app(hInstance);  
		//callBack 
		MSG msg = { 0 };
		while ((msg.message != WM_QUIT))
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 	
			
			app.Update(); //run Logic  
		}
		return static_cast<int>(msg.wParam);
	}
	catch (const com_exception&e) 
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details Aviable ", "Unkonwn Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return false; 
} 