#include "CBuffer.h"
#include "DXException.h"
/*Vertex Buffer */
	CVertexBuffer::CVertexBuffer(DXObjects dxObjects, void* vertices, unsigned int count, 
					unsigned int structureByteStride): m_dxo(dxObjects) , m_stride(structureByteStride)
	{
		/*顶点缓冲描述*/
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
		bd.Usage = D3D11_USAGE_DEFAULT; 
		bd.CPUAccessFlags = 0u; 
		bd.MiscFlags = 0u; 
		bd.ByteWidth = structureByteStride * count; 
		bd.StructureByteStride = structureByteStride;

		/*指定需要放入缓冲去的数据*/
		D3D11_SUBRESOURCE_DATA sd = { 0 }; 
		sd.pSysMem = vertices; 
		/*创建顶点缓冲*/
		DX::ThrowIfFailed(CALL_INFO,
			m_dxo.device->CreateBuffer(&bd,  &sd, &m_vertexBuffer)
		);
	}

	CVertexBuffer::~CVertexBuffer()
	{
	}

	void CVertexBuffer::Bind() const
	{
		UINT stride = m_stride; 
		UINT offset = 0; 
		m_dxo.context->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	}

	void CVertexBuffer::UnBind() const 
	{
	}

/*Index Buffer*/ 
	CIndexBuffer::CIndexBuffer(DXObjects dxObject , WORD* indices , unsigned int count): m_dxo(dxObject), m_count(count)
	{
		//buffer创建缓冲区描述 
		D3D11_BUFFER_DESC bd = {}; 
		bd.Usage = D3D11_USAGE_DEFAULT; 
		bd.ByteWidth = sizeof(WORD) * count; 
		bd.StructureByteStride = sizeof(WORD); 
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER; 
		bd.CPUAccessFlags = 0; 
		//创建缓冲区 数据需要传输到缓冲区 
		D3D11_SUBRESOURCE_DATA sd = {};  
		sd.pSysMem = indices; 
		//创建缓冲区 
		DX::ThrowIfFailed(CALL_INFO , 
			m_dxo.device->CreateBuffer(&bd, &sd , &m_indexBuffer) 
		); 



	}

	CIndexBuffer::~CIndexBuffer()
	{
	
	}

	void CIndexBuffer::Bind() const
	{
		m_dxo.context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	}

	void CIndexBuffer::UnBind() const
	{
		m_dxo.context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT , 0 );
	} 