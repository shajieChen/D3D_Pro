#include "b_Cube.h"
namespace example
{
	struct  ConstantBuffer
	{
		XMFLOAT4X4 matVP;
		XMFLOAT4X4 matGeo; 
	};
	example::BasicCube::BasicCube(DXObjects dxObjects) : m_dxo(dxObjects)
	{
		//创建InputLayout 输入布局 
			//创建顶点输入描述; 
			D3D11_INPUT_ELEMENT_DESC ied[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			//创建Shader 
			m_shader = std::make_unique<Shader>(m_dxo, ied , (unsigned int)ARRAYSIZE(ied),L"basicCubeVS.cso", L"basicCubePS.cso"); 
			m_shader->AddVSConstantBuffer(sizeof(ConstantBuffer));
			/*创建和设置ConstantBuffer*/
			struct Vertex
			{
				XMFLOAT3 Position; 
				XMFLOAT4 Color; 
			};
			//顶点数据 //创建顶点
			Vertex vertices[] = 
			{
				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower left
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Lower right	
				{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top left
				{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Front Top right

				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower left
				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Lower right	
				{ XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// Back Top left
				{ XMFLOAT3(1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }	// Back Top right
			};
			m_vertexBuffer = std::make_unique<CVertexBuffer>(m_dxo, vertices, (unsigned int)ARRAYSIZE(vertices), sizeof(Vertex)); 
		//创建indexBuffer索引缓冲
			//创建索引缓冲
			WORD indices[] = 
			{ 
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};
			m_indexBuffer = std::make_unique<CIndexBuffer>(m_dxo, indices , (unsigned int)ARRAYSIZE(indices)); 
	}

	example::BasicCube::~BasicCube()
	{
	}

	void example::BasicCube::Update()
	{ 
		//设置timer 
		m_timer.Tick([&]() {});  
	 
		//绑定Shader  
		m_shader->Bind(); 

		//更新Constant Buffer
		/*配置ViewMatrix*/
		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01f) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01f) *
			XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);  

		//设置matVP矩阵， 并且需要知道HLSL中是Col列矩阵 DirectxMath中是Row行矩阵 所以需要调转 
		ConstantBuffer cb ;
		XMStoreFloat4x4(&cb.matGeo, DirectX::XMMatrixIdentity());
		XMStoreFloat4x4(&cb.matVP,  view);

		m_shader->UpdateVSConstantBuffer(0, &cb); 

		//绑定VertexBuffer
		m_vertexBuffer->Bind(); 
		//绑定索引缓冲
		m_indexBuffer->Bind(); 

	
		/**/
		m_dxo.context->DrawIndexed(m_indexBuffer->GetCount(), 0u , 0); 
	}

	void example::BasicCube::UpdateGUI()
	{
	} 
}