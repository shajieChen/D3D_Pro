#pragma once
namespace example
{
	class IExample
	{
	public:
		virtual ~IExample() {};
		//update 用于更新图元 然后发送到renderCommand Queue中
		virtual void Update() = 0; 
		//封装更新GUI 的接口
		virtual void UpdateGUI() = 0;
	}; 
}