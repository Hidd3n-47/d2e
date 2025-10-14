-- ---------------------------- d2eNet Project.
project "d2eNet"
    location "%{prj.name}"
    kind "StaticLib"
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
        "$(SolutionDir)deps/include/",

        "$(SolutionDir)d2eNetProject/",
    }

    libdirs
    {
        "$(SolutionDir)deps/lib/",
    }

    links
    {
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
