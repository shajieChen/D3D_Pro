#pragma once
#include "Example.h"
#include "CDXObjects.h"
#include "CShader.h"
#include "StepTimer.h"
#include "CBuffer.h"
namespace example
{ 
	class BasicCube : public IExample
	{
	public:
		BasicCube(DXObjects dxObjects);
		~BasicCube();

		virtual void Update() override;
		virtual void UpdateGUI() override; 
	private:
		DX::StepTimer m_timer; 
		DXObjects m_dxo;
		std::unique_ptr<Shader> m_shader; 
		std::unique_ptr<CIndexBuffer> m_indexBuffer; 
		std::unique_ptr<CVertexBuffer> m_vertexBuffer; 
	};


}