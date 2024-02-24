#pragma once

#include "DXE.h"

namespace dxe::utils
{
	LPCWSTR StringToLPCWSTR(const std::string& s) 
	{
		// Convert std::string to std::wstring
		std::wstring ws(s.begin(), s.end());

		// Get pointer to the internal buffer of std::wstring
		LPCWSTR result = ws.c_str();

		return result;
	}
}
