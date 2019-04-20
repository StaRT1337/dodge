workspace "level_builder"
	location "./build"
	characterset "MBCS"


	if os.isfile("src/level_builder/stdafx.hpp") == false then 
		io.writefile("src/level_builder/stdafx.hpp", "")
	end

	if os.isfile("src/level_builder/stdafx.cpp") == false then 
		io.writefile("src/level_builder/stdafx.cpp", "")
	end

	if os.isfile("src/level_builder/main.cpp") == false then 
		io.writefile("src/level_builder/main.cpp", "")
	end

	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	buildlog "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "Off"

	buildoptions { "/utf-8", "/Zm200", "/std:c++latest" }
	configurations { "Debug", "Release" }
	platforms { "x86" }

	systemversion "10.0.17763.0"

	project "level_builder"
		targetname "level_builder"
		kind "WindowedApp"

		language "C++"
		cppdialect "C++17"

		pchheader "stdafx.hpp"
		pchsource "src/level_builder/stdafx.cpp"

		files {
			"./src/level_builder/**.h",
			"./src/level_builder/**.hpp",
			"./src/level_builder/**.cpp",
			"./src/level_builder/**.rc",
		}

		resincludedirs {
			"$(ProjectDir)src",
		}

		vpaths {
			["*"] = { "./src/level_builder/**" },
		}
		
		warnings "off"

		if symbols ~= nil then
			symbols "On"
		else
			flags { "Symbols" }
		end

		filter "platforms:*32"
			architecture "x86"

		configuration "Release*"
			defines { "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
			undefines { "UNICODE" }
			optimize "On"

		configuration "Debug*"
			defines { "WIN32_LEAN_AND_MEAN", "NOMINMAX", "DEBUG" }
			undefines { "UNICODE" }
			optimize "Debug"
			symbols "On"
