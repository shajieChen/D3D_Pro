#pragma once
#include "CDXObjects.h"
class CIndexBuffer
{
public :
	CIndexBuffer(DXObjects dxObject , WORD* indices, unsigned int count); 
	~CIndexBuffer(); 

	void Bind() const; 
	void UnBind() const; 
	inline unsigned int GetCount() const { return m_count;}
private :
	DXObjects m_dxo;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;  
	unsigned int m_count; 
};

class CVertexBuffer
{
public :
	CVertexBuffer(DXObjects dxObjects, void* vertices, unsigned int count,
				unsigned int structureByteStride);
	~CVertexBuffer(); 

	void Bind() const; 
	void UnBind() const; 
private: 
	DXObjects m_dxo; 
	unsigned int m_stride; 
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer; 
};
 