-- https://github.com/premake/premake-core/issues/160#issuecomment-129494474

solution "Solution"
	configurations { "Debug", "Release" }

	filter "system:windows"
		-- pick only 1 below
		platforms { "Windows" }
		-- platforms { "Emscripten" }

	filter "system:macosx"
		-- pick only 1 below
		platforms { "MacOS" }
		-- platforms { "Emscripten" }

	filter "system:linux"
		-- pick only 1 below
		platforms { "Linux" }
		-- platforms { "Emscripten" }

		filter { "configurations:Debug" }
		symbols "On"
		defines { "_DEBUG" }
		targetdir ( "Build/Bin/%{cfg.platform}/%{cfg.buildcfg}" )
		objdir ( "Build/Obj" )

	filter { "configurations:Release" }
		optimize "On"
		defines { "NDEBUG" }
		targetdir ( "Build/Bin/%{cfg.platform}/%{cfg.buildcfg}" )
		objdir ( "Build/Obj" )

	filter { "platforms:Windows" }
		libdirs { "Library/%{cfg.platform}/glfw", "C:/VulkanSDK/1.2.135.0/Lib32" }
		links { "OpenGL32", "vulkan-1" }
		linkoptions { }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/%{cfg.platform}/glfw/GLFW", "Library/Common/native", "C:/VulkanSDK/1.2.135.0/Include" }
		excludes { "**/MacOS/**", "**/Linux/**" }

	filter { "platforms:Windows", "kind:ConsoleApp" }
		targetextension (".exe")

	filter { "platforms:Windows", "kind:WindowedApp" }
		targetextension (".exe")

	filter { "platforms:Windows", "kind:SharedLib" }
		targetextension (".dll")

	filter { "platforms:Windows", "kind:StaticLib" }
		targetextension (".lib")

	filter { "platforms:Windows", "action:gmake", "kind:WindowedApp" }
		prebuildcommands {
		}
		postbuildcommands {
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{COPYDIR} Assets/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders",
			"{COPYDIR} Shaders/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders"
		}

	filter { "platforms:MacOS" }
		libdirs { "Library/%{cfg.platform}/glfw" }
		links { }
		linkoptions { "-Wl,-rpath,'@loader_path/" .. "." .. "'", "-framework Cocoa", "-framework Metal", "-framework MetalKit", "-framework Foundation", "-framework QuartzCore" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/%{cfg.platform}/glfw/GLFW", "Library/Common/native" }
		excludes { "**/Windows/**", "**/Linux/**" }

	filter { "platforms:MacOS", "kind:ConsoleApp" }
		targetextension ("")

	filter { "platforms:MacOS", "kind:WindowedApp" }
		targetextension (".app")

	filter { "platforms:MacOS", "kind:SharedLib" }
		targetextension (".dylib")

	filter { "platforms:MacOS", "kind:StaticLib" }
		targetextension (".a")

	filter { "platforms:MacOS", "action:xcode4", "kind:WindowedApp" }
		prebuildcommands {
		}
		postbuildcommands {
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{COPYDIR} Assets/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders",
			"{COPYDIR} Shaders/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders"
		}
		xcodebuildsettings {
			["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
			["PRODUCT_BUNDLE_IDENTIFIER"] = 'com.yourdomain.yourapp',
			["CODE_SIGN_STYLE"] = "Automatic",
			["ENABLE_HARDENED_RUNTIME"] = "YES",
			["LD_RUNPATH_SEARCH_PATHS"] = "$(inherited) @executable_path/../Frameworks",
			["GENERATE_INFOPLIST_FILE"] = "YES",
			["OTHER_CODE_SIGN_FLAGS"] = "--deep"
		}
		externalincludedirs {
			"Library/**"
		}

	filter { "platforms:MacOS", "action:gmake", "kind:WindowedApp" }
		prebuildcommands {
		}
		postbuildcommands {
			"{COPYDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name} %{cfg.buildtarget.directory}/%{prj.name}",
			"{RMDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/%{cfg.platform}",
			"{COPYDIR} %{cfg.buildtarget.directory}/%{prj.name} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/%{cfg.platform}",
			"{RMDIR} %{cfg.buildtarget.directory}/%{prj.name}",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{COPYDIR} Assets/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders",
			"{COPYDIR} Shaders/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Shaders"
		}

	filter { "platforms:Linux" }
		libdirs { "Library/%{cfg.platform}/glfw" }
		links { }
		linkoptions { "-Wl,-rpath,'$$ORIGIN/" .. "." .. "'", "-lGLU", "-lGLEW", "-lXrandr", "-lXi", "-lXxf86vm", "-lXcursor", "-lXinerama", "-lm", "-lvulkan", "-lxcb", "-lglfw3", "-ldl", "-lGL", "-lX11", "-lpthread" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/%{cfg.platform}/glfw/GLFW", "Library/Common/native" }
		excludes { "**/Windows/**", "**/MacOS/**" }

	filter { "platforms:Linux", "kind:ConsoleApp" }
		targetextension ("")

	filter { "platforms:Linux", "kind:WindowedApp" }
		targetextension ("")

	filter { "platforms:Linux", "kind:SharedLib" }
		targetextension (".so")

	filter { "platforms:Linux", "kind:StaticLib" }
		targetextension (".a")

	filter { "platforms:Linux", "action:gmake", "kind:WindowedApp" }
		prebuildcommands {
		}
		postbuildcommands {
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/%{cfg.platform}",
			"{COPYDIR} %{cfg.buildtarget.directory}/%{prj.name} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/%{cfg.platform}",
			"{RMDIR} %{cfg.buildtarget.directory}/%{prj.name}",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/Resources/Assets",
			"{COPYDIR} Assets/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/Resources/Assets",
			"{MKDIR} %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/Resources/Shaders",
			"{COPYDIR} Shaders/* %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}.app/Contents/Resources/Shaders",
		}

	project "StaticNative"
		kind "StaticLib"
		files { "Library/Common/native" }
		includedirs { "Library/Common/native" }
		excludes { "External/**", "Sample/**", "Shaders/**", "Source/**", "Verify/**", "Library/**/GLFW/**" }

	project "SampleCommonGl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/AcrossZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_COMMON", "COMPILE_API_GL" }

	project "SampleNativeDx"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/AcrossZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE", "COMPILE_API_DX" }

	project "SampleNativeGl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/AcrossZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE", "COMPILE_API_GL" }

	project "SampleNativeMl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/AcrossZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE", "COMPILE_API_ML" }

	project "SampleNativeVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/AcrossZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE", "COMPILE_API_VK" }

	project "SamplePuzzleMl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/PuzzleMl/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE" }

	project "SamplePuzzleVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/PuzzleVk/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE" }

	project "SampleRiddleVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "External/Common/stb/**", "External/Common/syoyo/**", "Library/**", "Sample/**", "Verify/**", "Source/Graphics/**", "Source/Image/**" }
		files { "Sample/RiddleVk/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE" }

	project "SampleRiddleZa"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**", "Source/Graphics/**" }
		files { "Sample/RiddleZa/**" }
		links { "StaticNative" }
		defines { "COMPILE_NATIVE" }

	project "VerifyMath"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Verify/Math/**" }

	project "VerifyUtil"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Verify/Util/**" }
