#pragma once

#include "Common.h"

namespace dxe
{
	class DXEException : public std::exception
	{
	public:
		DXEException(i32 line, const c8* file) noexcept
			: line(line), file(file)
		{
		}

		virtual const c8* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl << GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		virtual const c8* GetType() const noexcept
		{
			return "DXEDXEException";
		}

		i32 GetLine() const noexcept
		{
			return line;
		}

		const std::string& GetFile() const noexcept
		{
			return file;
		}

		std::string GetOriginString() const noexcept
		{
			std::ostringstream oss;
			oss << "[File] " << file << std::endl << "[Line] " << line;
			return oss.str();
		}
	private:
		i32 line;
		std::string file;
	protected:
		mutable std::string whatBuffer;
	};

	class WndException : public DXEException
	{
	public:
		WndException(i32 line, const c8* file, HRESULT hr) noexcept
			: DXEException(line, file), hr(hr)
		{
		}

		virtual const c8* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl << "[Error Code] " << GetErrorCode() << std::endl << "[Description] " << GetErrorString() << std::endl << GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		virtual const c8* GetType() const noexcept
		{
			return "Window Exception";
		}

		static std::string TranslateErrorCode(HRESULT hr) noexcept
		{
			c8* pMsgBuf = nullptr;
			DWORD nMsgLen = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
			);

			if (nMsgLen == 0)
			{
				return "Unidentified error code";
			}

			std::string errorString = pMsgBuf;
			LocalFree(pMsgBuf);
			return errorString;
		}

		HRESULT GetErrorCode() const noexcept
		{
			return hr;
		}

		std::string GetErrorString() const noexcept
		{
			return TranslateErrorCode(hr);
		}
	private:
		HRESULT hr;
	};
}

