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
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
        "d2e",
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
        defines "DEV_CONFIGURATION"
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
        optimize "on"
        links
        {
            "sfml-graphics-s.lib",
            "sfml-window-s.lib",
            "sfml-system-s.lib",
            "sfml-audio-s.lib",
        }
