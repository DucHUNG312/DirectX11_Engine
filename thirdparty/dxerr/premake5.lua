project "dxerr"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"**.inl",
		"**.h",
		"**.hpp",
		"**.c",
		"**.cc",
		"**.cpp",
	}

	includedirs
	{
	}

	links
	{
		
	}

	filter "system:windows"
		systemversion "latest"
		defines "DXE_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "DXE_DEBUG_BUILD"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DXE_RELEASE_BUILD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DXE_DIST_BUILD"
		runtime "Release"
		optimize "on"
