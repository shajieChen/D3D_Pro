#include "CShader.h"

#include "pch.h"
Shader::Shader(DXObjects dxObjects, D3D11_INPUT_ELEMENT_DESC* ied ,
				unsigned int iedElementCount , 
			   LPCWSTR vsSzFilePath, LPCWSTR psSzFilePath) 
			   : m_dxo(dxObjects)
{
	 //创建内存块Blob 
	Microsoft::WRL::ComPtr<ID3DBlob> blob; 
		
	//创建Vertex shader 
	DX::ThrowIfFailed(CALL_INFO,
		D3DReadFileToBlob(vsSzFilePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateVertexShader(blob->GetBufferPointer() ,blob->GetBufferSize() , 
			nullptr, &m_vs
		)
	);

	//创建Inputlayout 
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateInputLayout(
			ied, iedElementCount,
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&m_inputLayout
		)
	);

	//创建pixel shader
	DX::ThrowIfFailed(CALL_INFO, 
		D3DReadFileToBlob(psSzFilePath, &blob)
	);
	DX::ThrowIfFailed(CALL_INFO , 
		m_dxo.device->CreatePixelShader(blob->GetBufferPointer(), 
				blob->GetBufferSize() , 
				nullptr, &m_ps)
	);

}

Shader::~Shader()
{

}

void Shader::Bind() const
{
	m_dxo.context->IASetInputLayout(m_inputLayout.Get()); 
	m_dxo.context->VSSetShader(m_vs.Get(), nullptr, 0u); 
	m_dxo.context->PSSetShader(m_ps.Get(), nullptr, 0u);
}

void Shader::UnBind() const
{
	m_dxo.context->IASetInputLayout(nullptr); 
	m_dxo.context->VSSetShader(nullptr, nullptr, 0u);
	m_dxo.context->PSSetShader(nullptr, nullptr, 0u);
}

unsigned int Shader::AddVSConstantBuffer(unsigned int byteWidth)
{
	/*创建ConstantBuffer*/
	ID3D11Buffer* constantBuffer = nullptr; 
		/*描述ConstantBuffer*/
	D3D11_BUFFER_DESC bd = {}; 
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = byteWidth;  
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; 
	bd.CPUAccessFlags = 0; 
		/*描述需要上传到缓冲区的数据*/
		//nullptr
		/*创建Buffer*/
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd, nullptr, &constantBuffer)
	);
		/*添加到VertexBuffers列表中*/ 
	m_vsConstatnBuffers.push_back(constantBuffer);
	unsigned int slot =(unsigned int)m_vsConstatnBuffers.size() - 1;  
		/*临时设置Constant buffer*/
	m_dxo.context->VSSetConstantBuffers(slot, 1, &constantBuffer); 
	return slot;
}

unsigned int Shader::AddPSConstantBuffer(unsigned int byteWidth)
{
	/*创建Constant Buffer*/
	ID3D11Buffer* constantBuffer = nullptr; 
		/*描述ConstantBuffer*/
	D3D11_BUFFER_DESC bd = {}; 
		/*描述需要上传到缓冲区的数据*/
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = byteWidth;  
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; 
	bd.CPUAccessFlags = 0; 
		/*创建Buffer*/
	DX::ThrowIfFailed(CALL_INFO,
		m_dxo.device->CreateBuffer(&bd , nullptr, &constantBuffer)
	);
	/*添加到VertexBuffer列表中*/
	m_psConstatnBuffers.push_back(constantBuffer);
	/*临时设置ConstantBuffer*/
	unsigned int slot = (unsigned int)m_psConstatnBuffers.size() - 1; 
	/*返回Slot*/
	m_dxo.context->PSSetConstantBuffers(slot, 1, &constantBuffer);
	return slot;
}

void Shader::UpdateVSConstantBuffer(unsigned int slot, void* data)
{
	try
	{
		m_dxo.context->UpdateSubresource(m_vsConstatnBuffers.at(slot).Get() , 0 , nullptr, data , 0, 0 );
	}
	catch (const std::exception&)
	{
		__debugbreak(); 
		throw std::out_of_range("[Shader] updateVSConstantBuffer out of range slot!");
	}
}

void Shader::UpdatePSConstantBuffer(unsigned int slot, void* data)
{
	try
	{
		m_dxo.context->UpdateSubresource(m_psConstatnBuffers.at(slot).Get(), 0, nullptr, data, 0, 0); 
	}
	catch (const std::exception&)
	{
		__debugbreak(); 
		throw std::out_of_range("[Shader] updatePSConstantBuffer out of range Slot!"); 
	}
}
  