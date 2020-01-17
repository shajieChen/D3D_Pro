#pragma once


namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(int line, const char* file, HRESULT hr) noexcept; 
		virtual const char* what() const noexcept override; 

		virtual const char* GetType() const noexcept; 
		int Getline() const noexcept; 
		const std::string& GetFile() const noexcept; 
		std::string GetOriginString() const noexcept;  

		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;  
		std::string GetErrorString() const noexcept;  

	private : 
		int line; 
		std::string file; 
		HRESULT hr; 

	protected :
		mutable std::string whatBuffer; 
	};  
};
namespace DX
{ 
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			//throw com_exception(__LINE__, __FILE__, hr);
		}
	}
}