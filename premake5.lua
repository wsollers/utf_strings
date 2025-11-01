workspace "utf_strings"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    startproject "utf_strings-tests"

    filter "action:vs*"
        toolset "v143"
    filter {}

    language "C++"
    cppdialect "C++23"
    staticruntime "Off"
    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        symbols "On"
        defines { "UTF_STRINGS_DEBUG" }
    filter "configurations:Release"
        optimize "Speed"
        defines { "NDEBUG" }
    filter {}

    pcall(function() include("premake5-system.lua") end)
    pcall(function() include("conandeps.premake5.lua") end)

    includedirs { "include" }

    project "utf_strings-static"
        kind "StaticLib"
        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
        objdir   ("%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}")
        files { "include/**.hpp", "src/**.cpp" }

    project "utf_strings-shared"
        kind "SharedLib"
        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
        objdir   ("%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}")
        files { "include/**.hpp", "src/**.cpp" }
        defines { "UTF_STRINGS_DLL" }

    project "utf_strings-tests"
        kind "ConsoleApp"
        files { "tests/**.cpp" }
        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
        objdir   ("%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}")
        links { "utf_strings-static" }
        externalincludedirs { conandeps_includedirs }
        libdirs { conandeps_libdirs }
        links (conandeps_libs)
        dependson { "utf_strings-static" }

    project "utf_strings-bench"
        kind "ConsoleApp"
        files { "benchmarks/**.cpp" }
        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
        objdir   ("%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}")
        links { "utf_strings-static" }
        externalincludedirs { conandeps_includedirs }
        libdirs { conandeps_libdirs }
        links (conandeps_libs)
        dependson { "utf_strings-static" }
