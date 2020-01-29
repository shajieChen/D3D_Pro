
#include "pch.h"
#include "DXException.h" 
#include "App.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{ 
	try {
		App app(hInstance);
		 
		MSG msg = { 0 };
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			app.Update(0.0f);
		}
		return static_cast<int>(msg.wParam);
	}
	catch (const DXException & e) {
		MessageBox(nullptr, (LPCWSTR)e.what(), (LPCWSTR)e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e) {
		MessageBox(nullptr, (LPCWSTR)e.what(), L"Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, L"No details available", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}