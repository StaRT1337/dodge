workspace "dodge"
	location "./build"
	characterset "MBCS"


	if os.isfile("src/dodge/stdafx.hpp") == false then 
		io.writefile("src/dodge/stdafx.hpp", "")
	end

	if os.isfile("src/dodge/stdafx.cpp") == false then 
		io.writefile("src/dodge/stdafx.cpp", "")
	end

	if os.isfile("src/dodge/main.cpp") == false then 
		io.writefile("src/dodge/main.cpp", "")
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

	project "dodge"
		targetname "dodge"
		kind "WindowedApp"

		language "C++"
		cppdialect "C++17"

		pchheader "stdafx.hpp"
		pchsource "src/dodge/stdafx.cpp"

		files {
			"./src/dodge/**.h",
			"./src/dodge/**.hpp",
			"./src/dodge/**.cpp",
			"./src/dodge/**.rc",
		}

		resincludedirs {
			"$(ProjectDir)src",
		}

		vpaths {
			["*"] = { "./src/dodge/**" },
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
