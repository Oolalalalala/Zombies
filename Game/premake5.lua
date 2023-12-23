project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/vendor",
        "%{wks.location}/Dependencies/assimp/include",
        "%{wks.location}/Dependencies/SDL2/include",
        "%{wks.location}/Dependencies/SDL2_mixer/include",
        "%{wks.location}/Dependencies/freetype/include",
        "%{wks.location}/Dependencies/GLEW/include"
    }

    links
    {
        "Engine",
        "SDL2.lib",
        "SDL2main.lib",
        "SDL2_mixer.lib",
        "freetype.lib",
        "glew32s.lib",
        "opengl32.lib",
        "assimp-vc143-mt.lib"
    }


    filter "platforms:Win64"
        libdirs
        {
            "%{wks.location}/Dependencies/SDL2/lib/x64",
            "%{wks.location}/Dependencies/SDL2_mixer/lib/x64",
            "%{wks.location}/Dependencies/freetype/lib/Release/x64",
            "%{wks.location}/Dependencies/GLEW/lib/Release/x64",
            "%{wks.location}/Dependencies/assimp/lib"
        }


    
    filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "DISTRIBUTION"
		runtime "Release"
		optimize "on"