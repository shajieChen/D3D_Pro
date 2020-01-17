#include <Windows.h>
#include <iOStream>
#include <D3d11.h> 
int Run(); 

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

	//SwapChain
	DXGI_SWAP_CHAIN_DESC sd = {}; 
	sd.BufferDesc.Width = 0; 
	sd.BufferDesc.Height = 0; 
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	sd.BufferDesc.RefreshRate.Numerator = 0; 
	sd.BufferDesc.RefreshRate.Denominator = 0; 
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 
	sd.SampleDesc.Count = 1; 
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	sd.BufferCount = 1; 
	sd.OutputWindow = hwnd;  
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;  
	sd.Flags = 0; 
	//
	ID3D11Device* pDevice = nullptr; 
	IDXGISwapChain* pSwap = nullptr; 
	ID3D11DeviceContext* pContext = nullptr; 
	ID3D11RenderTargetView* pTarget = nullptr; 

	//device 
	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr , 0 ,
		nullptr, 0, D3D11_SDK_VERSION , &sd ,&pSwap, 
		&pDevice, nullptr , &pContext
	);
	//back buffer
	ID3D11Resource* pBackBuffer = nullptr; 
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)); 
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr , &pTarget);
	pBackBuffer->Release();



	//callBack 
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		//clear Buffer 
		const float color[] = { 1.0f , 0.0f, 0.0f, 1.0f };
		pContext->ClearRenderTargetView(pTarget , color);

		DispatchMessage(&msg);

		//present
		pSwap->Present(1u,0u);
	}
	switch (gResult)
	{
	case -1: return -1;
	default: return msg.wParam;
	}
}
int Run()
{
	return 1; 
 }