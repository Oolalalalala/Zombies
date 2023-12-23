project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/**.hpp",
        "vendor/glm/**.inl",
        "vendor/spdlog/**.h",
        "vendor/entt/entt.hpp",
        "vendor/stb_image/stb_image.h",
        "vendor/stb_image/stb_image.cpp"
    }

    includedirs
    {
        "src",
        "vendor",
        "%{wks.location}/Dependencies/assimp/include",
        "%{wks.location}/Dependencies/SDL2/include",
        "%{wks.location}/Dependencies/SDL2_mixer/include",
        "%{wks.location}/Dependencies/freetype/include",
        "%{wks.location}/Dependencies/GLEW/include"
    }

    links
    {
        "SDL2.lib",
        "SDL2main.lib",
        "SDL2_mixer.lib",
        "freetype.lib",
        "glew32s.lib",
        "opengl32.lib",
        "assimp-vc143-mt.lib"
    }

    defines
    {
        "GLEW_STATIC"
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

    filter "files:vendor/**.cpp"
        flags { "NoPCH" }

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