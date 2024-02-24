#pragma once

#include <dxerr.h>

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

	//////////////////////////////
	///   Window exceptions
	//////////////////////////////

	class WndException : public DXEException
	{
	public:
		using DXEException::DXEException;
		
		static std::string TranslateErrorCode(HRESULT hr) noexcept
		{
			c8* pMsgBuf = nullptr;
			const DWORD nMsgLen = FormatMessageA(
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
	};

	class WndHrException : public WndException
	{
	public:
		WndHrException(i32 line, const c8* file, HRESULT hr) noexcept
			: WndException(line, file), hr(hr)
		{
		}

		virtual const c8* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl
				<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
				<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
				<< "[Description] " << GetErrorDescription() << std::endl
				<< GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		virtual const c8* GetType() const noexcept
		{
			return "DXE Window Exception";
		}

		HRESULT GetErrorCode() const noexcept
		{
			return hr;
		}

		std::string GetErrorDescription() const noexcept
		{
			return WndException::TranslateErrorCode(hr);
		}
	private:
		HRESULT hr;
	};

	class NoGfxException : public WndException
	{
	public:
		using WndException::WndException;
		const c8* GetType() const noexcept override
		{
			return "Chili Window Exception [No Graphics]";
		}
	};

	//////////////////////////////
	///   Graphics exceptions
	//////////////////////////////

	class GfxException : public DXEException
	{
		using DXEException::DXEException;
	};

	class GfxHrException : public GfxException
	{
	public:
		GfxHrException(i32 line, const c8* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept
			: GfxException(line, file), hr(hr)
		{	
			// join all info messages with newlines into single string
			for (const auto& m : infoMsgs)
			{
				info += m;
				info.push_back('\n');
			}
			// remove final newline if exists
			if (!info.empty())
			{
				info.pop_back();
			}
		}

		const c8* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl
				<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
				<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
				<< "[Error String] " << GetErrorString() << std::endl
				<< "[Description] " << GetErrorDescription() << std::endl;

			if (!info.empty())
			{
				oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
			}

			oss << GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		const c8* GetType() const noexcept override
		{
			return "DXE Graphics Exception";
		}

		HRESULT GetErrorCode() const noexcept
		{
			return hr;
		}

		std::string GetErrorString() const noexcept
		{
			return DXGetErrorString(hr);
		}

		std::string GetErrorDescription() const noexcept
		{
			c8 buf[512];
			DXGetErrorDescription(hr, buf, sizeof(buf));
			return buf;
		}

		std::string GetErrorInfo() const noexcept
		{
			return info;
		}
	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public GfxHrException
	{
		using GfxHrException::GfxHrException;
	public:
		const c8* GetType() const noexcept override
		{
			return "DXE Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
		}
	private:
		std::string reason;
	};

	class InfoException : public GfxException
	{
	public:
		InfoException(i32 line, const c8* file, std::vector<std::string> infoMsgs) noexcept
			: GfxException(line, file)
		{
			// join all info messages with newlines into single string
			for (const auto& m : infoMsgs)
			{
				info += m;
				info.push_back('\n');
			}
			// remove final newline if exists
			if (!info.empty())
			{
				info.pop_back();
			}
		}

		const c8* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl
				<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
			oss << GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		const c8* GetType() const noexcept override
		{
			return "DXE Graphics Info Exception";
		}

		std::string GetErrorInfo() const noexcept
		{
			return info;
		}
	private:
		std::string info;
	};
}
