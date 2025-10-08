-- ---------------------------- d2eGameServer Project.
project "d2eGameServer"
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

        "$(SolutionDir)LogProject/",
        "$(SolutionDir)d2eNetProject/",

        "$(SolutionDir)deps/include/"
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
        "Log",
        "d2eNet",
        "enet64.lib",
        "winmm.lib",
        "WS2_32.lib",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "DEV_CONFIGURATION"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
