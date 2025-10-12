-- ---------------------------- d2e Project.
project "d2e"
    location "%{prj.name}"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++latest"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "d2ePch.h"
    pchsource "%{prj.name}/src/d2ePch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/",
        "%{prj.name}/Src/",
        "$(SolutionDir)deps/include/",

        "$(SolutionDir)d2eProject/",
        "$(SolutionDir)LogProject/",
        "$(SolutionDir)d2eNetProject/",
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
        "Log",
        "d2eNet",

        "opengl32",
        "freetype",
        --"winmm",
        --"gdi32",
        "flac",
        "vorbis",
        "vorbisfile",
        "enet64.lib",
        "WS2_32.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "ENGINE_DEBUG", "SFML_STATIC" }
        symbols "on"
        links
        {
            "sfml-graphics-s-d.lib",
            "sfml-window-s-d.lib",
            "sfml-system-s-d.lib",
            "sfml-audio-s-d.lib",
        }

    filter "configurations:Release"
        runtime "Release"
        defines "SFML_STATIC"
        optimize "on"
        links
        {
            "sfml-graphics-s.lib",
            "sfml-window-s.lib",
            "sfml-system-s.lib",
            "sfml-audio-s.lib",
        }
