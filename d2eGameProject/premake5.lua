-- ---------------------------- d2e Game Project.
project "d2eGame"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"
    cppdialect "C++latest"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/",
        "$(SolutionDir)deps/include/",

        "$(SolutionDir)LogProject/",
        "$(SolutionDir)d2eProject/",
        "$(SolutionDir)d2eNetProject/",
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
        "d2e",
        "d2eNet",
        "opengl32",
        "freetype",
        "winmm",
        "gdi32",
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
        defines { "DEV_CONFIGURATION", "GAME_DEBUG", "SFML_STATIC" }
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
