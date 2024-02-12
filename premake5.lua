include "Scripts/PremakeUtils/PremakeCommon.lua"
include "Scripts/PremakeUtils/PremakeDefines.lua"
include "Scripts/PremakeUtils/PremakeDependencies.lua"

workspace "dxe"
	architecture "x86_64"
	startproject "dxe"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
	filter "language:C++ or language:C"
		architecture "x86_64"
	filter ""

include "dxe"