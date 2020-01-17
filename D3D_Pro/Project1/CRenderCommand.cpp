
#include "phc.h" 
#include "CRenderCommand.h"
#include "DXErrorHandler.h"
CRenderCommand::CRenderCommand(HWND& hWnd) : m_sd({ 0 } )
{
	//SwapChain Desc
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
	sd.OutputWindow = hWnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

#pragma region ÏÔ¿¨/ äÖÈ¾Ä¿±êÉèÖÃ 
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
#pragma endregion 
	DX::ThrowIfFailed(
		D3D11CreateDeviceAndSwapChain( 	//device 
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwap,
			&pDevice, nullptr, &pContext
		)
	);
	//back buffer
	ID3D11Resource* pBackBuffer = nullptr;//back buffer
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);
	pBackBuffer->Release();
}


void CRenderCommand::Clear()
{
	const float color[] = { 1.0f , 0.0f, 0.0f, 1.0f };

//		pContext->ClearRenderTargetView(pTarget, color);
			 
}

void CRenderCommand::Swap()
{
	//pSwap->Present(1u, 0u);
}