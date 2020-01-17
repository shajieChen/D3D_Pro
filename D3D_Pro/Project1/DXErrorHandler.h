#pragma once
#include <Exception>
#include <stdio.h>
#include <D3d11.h>

namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override {
			static char s_str[64] = {}; 
			sprintf_s(s_str, "Failed with HRESULT of %08X",
				static_cast<unsigned int>(result)); 
			return s_str;  
		}

	private : 
		HRESULT result; 
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr); 
		}
	}



};