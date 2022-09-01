project "Generator"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource ( "src/pch.cpp" )

	files 
	{
		"src/**.h",
		"src/**.c",
		"src/**.cpp",
	}

	includedirs
	{
		"src/",
		"%{IncludeDirectories.Generator}"
	}
		
	filter {}

	postbuildcommands
	{
		--("{COPYDIR} \"./src/**.h\" \"./include/Generator\""),
	}

	filter "system:Windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GENERATOR_WINDOWS_BUILD",
			"GENERATOR_STATIC_BUILD"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GENERATOR_LINUX_BUILD",
			"GENERATOR_STATIC_BUILD"
		}
		
		links
		{
			"pthread",
			"dl"
		}

	filter "configurations:Debug"
		defines { "GENERATOR_DEBUG", "GENERATOR_PROFILING" }
		symbols "On"

	filter "configurations:Release"
		defines { "GENERATOR_RELEASE", "GENERATOR_PROFILING" }
		optimize "On"

	filter "configurations:Dist"
		defines "GENERATOR_DIST"
		optimize "On"
