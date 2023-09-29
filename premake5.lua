workspace "PROJECTS"
	configurations {"Debug", "Release"}
	platforms {"Win64", "Static", "Shared"}
	toolset ("clang")
project "SDF"
	location "./"
	kind "ConsoleApp"
	language "C++"
	targetdir "./" --bin/%{cfg.buildcfg}"
	targetname "sdf"
	files {"**.hpp", "**.h", "**.c", "**.cpp", "**.cc", "**.idl"}
	includedirs{"include",
	"./vendor",
	"./vendor/imgui/include",
	"D:/DEV/boost_1_82_0",
	"vendor/GL",
	"./vendor/gleq",
	"./vendor/stb"}
	removefiles{}
	links{"glew32s",
	"opengl32",
	"comdlg32",
	"gdi32",
	"glfw3"}
	linkoptions{}
	libdirs{"lib"}
	defines{"GLEW_STATIC"}
	stl ("libc++")
	filter "files:**.cpp"
		buildoptions {"-std=c++17"}
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		buildoptions{"-g3", "-fdata-sections", "-ffunction-sections"}
	filter "configurations:Release"
		defines {}
		optimize "On"
		linkoptions {"-fno-exceptions", "-fno-rtti", "-fPIC"}
		buildoptions {"-fdata-sections", "-ffunction-sections", "-m64", "-Wall", "-Wextra", "-pthread"}
	filter { "platforms:Win64" }
    	system "Windows"
    	architecture "x86_64"
	filter { "platforms:Static" }
    	kind "StaticLib"
	filter { "platforms:DLL" }
    	kind "SharedLib"
    	defines { "DLL_EXPORTS" }
