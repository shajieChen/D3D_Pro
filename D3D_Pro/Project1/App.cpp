
#include "pch.h"
#include "App.h"
#include "b_Triangle.h"
#include "b_Cube.h" 
IMGUI_IMPL_API HRESULT ImGUI_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(ImGUI_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true; 
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


App::App(HINSTANCE& hInstance) : m_className("D3D_Pro"), m_hwnd(nullptr)
{
	initWindow(hInstance);
	initDirectX11();
	initImGui(); 


	m_renderer = std::make_unique<Renderer>(m_dxo);
	m_rc = std::make_unique<RenderCommand>(m_dxo); 

	m_example = std::make_unique<example::BasicCube>(m_dxo);
}  
 
App::~App()
{  
	ImGui_ImplDX11_Shutdown(); 
	ImGui_ImplWin32_Shutdown(); 
	ImGui::DestroyContext();
} 

void App::Update(float dt) {
	m_rc->Clear(); 
	 
	//update GUI
	m_example->UpdateGUI(); 
	
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Main Debug Window"); 
		ImGui::Text("%.3 ms/Frame FPS" , 1000.0f / ImGui::GetIO().Framerate ,ImGui::GetIO().Framerate );
		ImGui::End(); 
	}
	//更新图元
	{
		m_renderer->BeginScene(); 
		m_example->Update(); 
		m_renderer->EndScene();
	}
	

/*渲染GUI - 开始*/
	ImGui::Render(); 
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
/*渲染GUI - 结束*/

	m_rc->Swap();
}

/*private helper method 
	- 初始化窗口
	- 初始化D3D11
	- 初始化ImGUI UI系统 
*/
 
void App::initWindow(HINSTANCE& hInstance)
{  
	//添加窗口描述
	WNDCLASSEX wc; 
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = (LPCWSTR)m_className;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE); 
	//创建窗口
	m_hwnd = CreateWindowEx(
		0, (LPCWSTR)m_className, L"D3D_Pro",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr
	);
	if (m_hwnd == nullptr) {
		throw DX_LAST_ERROR_EXCEPTION;
	}

	ShowWindow(m_hwnd, SW_SHOW);
	
}

void App::initDirectX11()
{ 
	//获取viewPort的大小 
	RECT rc; 
	GetClientRect(m_hwnd , &rc);
	const int width = rc.right - rc.left;  
	const int height = rc.bottom - rc.top; 
	//窗口描述
	DXGI_SWAP_CHAIN_DESC sd; 
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT createDeviceFlags = 0; 
#ifdef  NDefDEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; 
#endif //  NDefDEBUG

	//声明Feature Level 
	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_1, 
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numFeatureLeves = ARRAYSIZE(featureLevel);

	//三步走 ： device -> context/ SwapChain -> BackBuffer -> rendertargetView ()
	DX::ThrowIfFailed(CALL_INFO , 
		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE , nullptr, 
			createDeviceFlags, featureLevel, numFeatureLeves,  
			D3D11_SDK_VERSION, &sd, &m_dxo.swapChain ,
			&m_dxo.device , nullptr, &m_dxo.context
		)
	);
	//从renderTarget view 中获取backbuffer 
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer; 
	DX::ThrowIfFailed(CALL_INFO , 
		m_dxo.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D) , &backBuffer) 
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_dxo.target)
	); 

	//创建深度模板纹理
	//ID3D11Texture2D* depthStencilTexture = nullptr; 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;  
		//创建缓冲描述
	D3D11_TEXTURE2D_DESC descDepth; 
	descDepth.Width = width;  
	descDepth.Height = height;  
	descDepth.MipLevels = 1; 
	descDepth.ArraySize = 1;  
	descDepth.Format = DXGI_FORMAT_D32_FLOAT; 
	descDepth.SampleDesc.Count = 1;  
	descDepth.SampleDesc.Quality = 0; 
	descDepth.Usage = D3D11_USAGE_DEFAULT;  
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;  
	descDepth.CPUAccessFlags = 0; 
	descDepth.MiscFlags = 0; 
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateTexture2D(&descDepth, nullptr, &depthStencil)
	);
		//创建深度模板状态
	D3D11_DEPTH_STENCIL_DESC dsDesc; 
	dsDesc.DepthEnable = false;  
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  
	dsDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;  

	dsDesc.StencilEnable = true; 
	dsDesc.StencilReadMask = 0xFF; 
	dsDesc.StencilWriteMask = 0xFF; 
		//根据模板测试与深度测试的结果 ，应对各种模板运算 
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  //当像素片段在模板测试中失败 如何根性模板缓冲区
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; //描述当像素片段通过模板测试 但是在深度测试的时候失败 如何更新模板
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // 描述当通过模板测试和深度测试的时候 如何更新模板 
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 描述模板测试使用的比较函数 

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;  //当像素片段在模板测试中失败 如何根性模板缓冲区
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; //描述当像素片段通过模板测试 但是在深度测试的时候失败 如何更新模板
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // 描述当通过模板测试和深度测试的时候 如何更新模板 
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 描述模板测试使用的比较函数 
	//创建DepthStencilState 
	m_dxo.device->CreateDepthStencilState(&dsDesc, &m_dxo.depthStencilState); 
	 


		//描述 DepthStencilView 
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {}; 
	descDSV.Format = descDepth.Format; 
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  
	descDSV.Texture2D.MipSlice = 0; 
	//创建DepthStencilView 
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateDepthStencilView(depthStencil.Get(), &descDSV, &m_dxo.depthStencilView)
	);
	
	//设置viewPort
	D3D11_VIEWPORT vp; 
	vp.Width = (float)width; 
	vp.Height = (float)height;  
	vp.MinDepth = 0.0f; 
	vp.MaxDepth = 1.0f; 
	vp.TopLeftX = 0; 
	vp.TopLeftY = 0; 
	m_dxo.context->RSSetViewports(1, &vp);
	/*设置图元*/
	m_dxo.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/*设置Stencil*/
	m_dxo.context->OMSetDepthStencilState(m_dxo.depthStencilState.Get() , 1);
	/*设置renderTarget */
	m_dxo.context->OMSetRenderTargets(1u , m_dxo.target.GetAddressOf() , m_dxo.depthStencilView.Get());


	//设置背面提出    
	D3D11_RASTERIZER_DESC rasterizerDesc;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSCullClockWise; 
	/* ComPtr<ID3D11RasterizerState> RSCullClockWise;*/
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthClipEnable = true;
	DX::ThrowIfFailed(CALL_INFO , 
		m_dxo.device->CreateRasterizerState(&rasterizerDesc, &RSCullClockWise)
	);
	m_dxo.context->RSSetState(RSCullClockWise.Get()); 
}

void App::initImGui()
{
	IMGUI_CHECKVERSION(); 
	ImGui::CreateContext(); 
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_dxo.device.Get() , m_dxo.context.Get());
	ImGui::StyleColorsLight();
}

IMGUI_IMPL_API HRESULT ImGUI_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return IMGUI_IMPL_API HRESULT();
}
