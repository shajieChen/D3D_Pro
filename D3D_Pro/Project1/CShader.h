#pragma once
#include "CDXObjects.h"
class Shader
{
	public:
		Shader(DXObjects dxObjects, D3D11_INPUT_ELEMENT_DESC* ied, unsigned int iedElementCount , LPCWSTR vsSzFilePath, LPCWSTR psSzFilePath); 
		~Shader(); 

		void Bind() const; 
		void UnBind() const; 

		/**
		*@brief 添加固定缓冲区到顶点Buffer中 
		*输入为 buffer的大小 
		*返回值为slot插槽位置 
		*/
		unsigned int AddVSConstantBuffer(unsigned int byteWidth); 
		unsigned int AddPSConstantBuffer(unsigned int byteWidth); 
		 
		void UpdateVSConstantBuffer(unsigned int slot, void* data); 
		void UpdatePSConstantBuffer(unsigned int slot, void* data);

private:
	DXObjects m_dxo; 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; 
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs; 
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;  
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_vsConstatnBuffers; 
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_psConstatnBuffers; 
};

