-- https://github.com/premake/premake-core/issues/160#issuecomment-129494474

solution "Solution"
	configurations { "Debug", "Release" }

	filter "system:windows"
		platforms { "Windows" }

	filter "system:macosx"
		platforms { "MacOS" }

	filter "system:linux"
		platforms { "Linux" }

	filter { "configurations:Debug" }
		symbols "On"
		defines { "_DEBUG" }
		targetdir ( "Build/Bin/%{cfg.platform}/Debug" )
		objdir ( "Build/Obj" )

	filter { "configurations:Release" }
		optimize "On"
		defines { "NDEBUG" }
		targetdir ( "Build/Bin/%{cfg.platform}/Release" )
		objdir ( "Build/Obj" )

	filter { "platforms:Windows" }
		system "windows"
		excludes { "**/MacOS/**", "**/Linux/**" }
		targetextension (".exe")
		libdirs { "Library/Windows/glfw", "C:/VulkanSDK/1.2.135.0/Lib32" }
		links { "OpenGL32", "glfw3", "vulkan-1" }
		linkoptions { }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/Windows/glfw/GLFW", "Library/Common/native", "C:/VulkanSDK/1.2.135.0/Include" }
		prebuildcommands { "{MKDIR} %{cfg.buildtarget.directory}" }

	filter { "platforms:MacOS" }
		system "macosx"
		excludes { "**/Windows/**", "**/Linux/**" }
		targetextension (".app")
		libdirs { "Library/MacOS/glfw" }
		links { }
		linkoptions { "-framework Cocoa", "-framework Metal", "-framework MetalKit", "-framework IOKit", "-framework CoreVideo", "-framework OpenGL", "-framework QuartzCore", "-lglfw3" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/MacOS/glfw/GLFW", "Library/Common/native" }
		prebuildcommands { "{MKDIR} %{cfg.buildtarget.directory}" }

	filter { "platforms:Linux" }
		system "linux"
		excludes { "**/Windows/**", "**/MacOS/**" }
		targetextension ("")
		libdirs { "Library/Linux/glfw" }
		links { }
		linkoptions { "-lglfw3", "-lGLU", "-lGL", "-lGLEW", "-lX11", "-lXrandr", "-lXi", "-lXxf86vm", "-lXcursor", "-lXinerama", "-lpthread", "-lm", "-ldl", "-lvulkan", "-lxcb" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.vert", "**.frag", "**.lua" }
		includedirs { "External/**", "Source/**", "Library/Linux/glfw/GLFW", "Library/Common/native" }
		prebuildcommands { "{MKDIR} %{cfg.buildtarget.directory}" }

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
		excludes { "Library/**", "Sample/**", "Verify/**", "Source/Graphics/**" }
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
		kind "ConsoleApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Verify/Math/**" }

	project "VerifyUtil"
		kind "ConsoleApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Verify/Util/**" }
