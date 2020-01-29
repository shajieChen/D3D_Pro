#pragma once

#include "Example.h"
#include "CDXObjects.h"
#include "CShader.h"
#include "CBuffer.h"

namespace example
{
	class BasicTriangle : public IExample
	{
	public : 
		BasicTriangle(DXObjects dxObjects);
		~BasicTriangle(); 

		virtual void Update() override; 
		virtual void UpdateGUI() override;
		void BindRes();

	private: 
		DXObjects m_dxo; 
		std::unique_ptr<Shader> m_shader;  
		std::unique_ptr<CVertexBuffer> m_vertexBuffer; 
	};
}