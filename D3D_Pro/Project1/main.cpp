#include <Windows.h>
#include <iOStream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break; 
		case WM_LBUTTONDOWN:
			MessageBox(0, "helloworld", "HelloWind",MB_OK );
			return 0; 
		case WM_KEYDOWN :
			if (wParam == VK_ESCAPE)
				DestroyWindow(hwnd);
			return 0; 
		default:
			break; 
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int CALLBACK WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	const auto pClassName = "D3D_Pro"; 
	//window 
	WNDCLASSEX wc = { 0 }; 
	wc.cbSize = sizeof(wc); 
	wc.style = CS_OWNDC; 
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = hInstance;
	wc.hIcon = nullptr; 
	wc.hbrBackground = nullptr; 
	wc.lpszMenuName = nullptr; 
	wc.lpszClassName = pClassName; 
	wc.hIconSm = nullptr; 
	RegisterClassEx(&wc); 

	//WindowInstance 
	HWND hwnd = CreateWindowEx(
			0,pClassName , "D3D_Pro", 
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT , CW_USEDEFAULT, 640 , 480 , nullptr ,nullptr ,
			hInstance, nullptr
	);

	if (hwnd == 0)
	{
		MessageBox(0, "CreateWindow FALLED" , 0 , 0 );
		return false; 
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//callBack 
	MSG msg; 
	BOOL gResult; 
	while ((gResult = GetMessage(&msg, nullptr , 0, 0)) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	switch (gResult)
	{
		case -1: return -1; 
		default: return msg.wParam; 
	}
}