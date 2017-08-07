solution "Sandbox"
	configurations { "Debug", "Release" }

	filter { "configurations:Debug" }
		symbols "On"
		defines { "_DEBUG" }
		targetdir ( "Build/Bin/Debug" )
		objdir ( "Build/Obj/Debug" )

	filter { "configurations:Release" }
		optimize "On"
		defines { "NDEBUG" }
		targetdir ( "Build/Bin/Release" )
		objdir ( "Build/Obj/Release" )

	filter { "kind:WindowedApp" }
		links { "StaticNative" }

	filter { "system:windows" }
		libdirs { "Library/Windows/glfw", "C:/VulkanSDK/1.2.135.0/Lib32" }
		links { "OpenGL32", "glfw3", "vulkan-1" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag" }
		includedirs { "External/**", "Source/**", "Library/Windows/glfw/GLFW", "Library/Common/native", "C:/VulkanSDK/1.2.135.0/Include" }
		excludes { "**/MacOS/**", "**/Linux/**" }

	filter { "system:macosx" }
		libdirs { "Library/MacOS/glfw" }
		linkoptions { "-framework Cocoa", "-framework Metal", "-framework MetalKit", "-framework IOKit", "-framework CoreVideo", "-framework OpenGL", "-framework QuartzCore", "-lglfw3" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag" }
		includedirs { "External/**", "Source/**", "Library/MacOS/glfw/GLFW", "Library/Common/native" }
		excludes { "**/Windows/**", "**/Linux/**" }

	filter { "system:linux" }
		libdirs { "Library/Linux/glfw" }
		linkoptions { "-lglfw3", "-lGLU", "-lGL", "-lGLEW", "-lX11", "-lXrandr", "-lXi", "-lXxf86vm", "-lXcursor", "-lXinerama", "-lpthread", "-lm", "-ldl", "-lvulkan", "-lxcb" }
		files { "**.h", "**.hh", "**.hpp", "**.c", "**.cc", "**.cpp", "**.m", "**.mm", "**.vert", "**.frag" }
		includedirs { "External/**", "Source/**", "Library/Linux/glfw/GLFW", "Library/Common/native" }
		excludes { "**/Windows/**", "**/MacOS/**" }

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
		files { "Sample/CommonGl/**" }
		defines { "COMPILE_COMMON" }

	project "SampleNativeDx"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/NativeDx/**" }
		defines { "COMPILE_NATIVE" }

	project "SampleNativeGl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/NativeGl/**" }
		defines { "COMPILE_NATIVE" }

	project "SampleNativeMl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/NativeMl/**" }
		defines { "COMPILE_NATIVE" }

	project "SampleNativeVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/NativeVk/**" }
		defines { "COMPILE_NATIVE" }

	project "SamplePuzzleMl"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/PuzzleMl/**" }
		defines { "COMPILE_NATIVE" }

	project "SamplePuzzleVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**" }
		files { "Sample/PuzzleVk/**" }
		defines { "COMPILE_NATIVE" }

	project "SampleRiddleVk"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**", "Source/Graphics/**" }
		files { "Sample/RiddleVk/**" }
		defines { "COMPILE_NATIVE" }

	project "SampleRiddleZa"
		kind "WindowedApp"
		language "C++"
		buildoptions { "-std=c++17" }
		excludes { "Library/**", "Sample/**", "Verify/**", "Source/Graphics/**" }
		files { "Sample/RiddleZa/**" }
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
