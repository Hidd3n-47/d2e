-- ---------------------------- d2e Project.
project "d2e"
    location "%{prj.name}"
    kind "ConsoleApp"
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

        "$(SolutionDir)LogProject/",
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
        "Log",

        "opengl32",
        "freetype",
        "winmm",
        "gdi32",
        "flac",
        "vorbis",
        "vorbisfile"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "SFML_STATIC" }
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
