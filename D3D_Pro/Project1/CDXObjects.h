﻿#pragma once
#include "pch.h"
/*此处应该添加引用次数 */
struct  DXObjects
{
	Microsoft::WRL::ComPtr<ID3D11Device> device; 
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context; 
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target; 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView; 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState; 
};