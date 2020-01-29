#include "b_Triangle.h"

namespace example
{
	BasicTriangle::BasicTriangle(DXObjects dxObjects): m_dxo(dxObjects)
	{

		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		m_shader = std::make_unique<Shader>(m_dxo, ied , ARRAYSIZE(ied), L"legacyVS.cso" , L"legacyPS.cso");

		struct Vertex {
			float x;
			float y;
		};

		Vertex vertices[] = {
			{  0.0f,  0.5f },
			{  0.5f, -0.5f },
			{ -0.5f, -0.5f }
		};
		 
		m_vertexBuffer = std::make_unique<CVertexBuffer>(m_dxo, vertices, ARRAYSIZE(vertices), sizeof(Vertex));
		//设置 对应变量
		BindRes();
	}
	BasicTriangle::~BasicTriangle()
	{
		m_shader->UnBind();
	}
	void BasicTriangle::Update()
	{ 
		m_shader->Bind(); 

		m_vertexBuffer->Bind(); 

		m_dxo.context->Draw(3u , 0u);

	}
	void BasicTriangle::UpdateGUI()
	{

	}
	void BasicTriangle::BindRes()
	{   
	}
}