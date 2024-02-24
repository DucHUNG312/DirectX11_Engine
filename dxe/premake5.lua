project "dxe"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	shaderobjectfileoutput ("shadersout/%%(Filename).cso")

	files
	{
		"shaders/**.hlsl",
		"include/**.h",
		"src/**.c",
		"include/**.hpp",
		"src/**.cpp",
		"include/**.hxx",
		"src/**.cxx",
		"**.rc"
	}

	includedirs
	{
		"include",
		"%{IncludeDir.dxerr}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.dxe}",
	}

	links
	{
		"dxerr"
	}

	filter { "files:**.p.hlsl" }
   		shadertype "Pixel"
	filter { "files:**.v.hlsl" }
		shadertype "Vertex"

	filter "system:windows"
		defines 
		{
			"DXE_PLATFORM_WINDOWS",
		}
		systemversion "latest"

	filter "configurations:Debug"
		defines "DXE_DEBUG_BUILD"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines 
		{
			"DXE_RELEASE_BUILD",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DXE_DIST_BUILD"
		runtime "Release"
		optimize "on"

	filter "files:**.asm"
		buildaction "Masm"