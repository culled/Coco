workspace "Coco"
    architecture "x64"
    startproject "Cocobox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solution folder
IncludeDir = {}
IncludeDir["src"] =     "Coco/src"
IncludeDir["spdlog"] =  "Coco/vendor/spdlog/include"
IncludeDir["GLFW"] =    "Coco/vendor/glfw/include"
IncludeDir["Glad"] =    "Coco/vendor/glad/include"
IncludeDir["ImGui"] =   "Coco/vendor/imgui"
IncludeDir["glm"] =     "Coco/vendor/glm"
IncludeDir["stb"] =     "Coco/vendor/stb"
IncludeDir["entt"] =    "Coco/vendor/entt/single_include/entt"

group "Dependencies"
    include "premakes/glfw"
    include "premakes/glad"
    include "premakes/imgui"
group ""

project "Coco"
    location "Coco"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ccpch.h"
    pchsource "Coco/src/ccpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/entt/single_include/entt/entt.hpp",
        "%{prj.name}/vendor/stb/stb_image.h"
    }

    includedirs
    {
        "%{IncludeDir.src}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "COCO_IMGUI=1",
        "COCO_ASSERTS=1"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GLFW_INCLUDE_NONE",
            "COCO_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "COCO_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "COCO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "COCO_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "%{IncludeDir.src}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "Coco"
    }

    defines
    {
        "COCO_ASSERTS=1"
    }

    filter "system:windows" 
        systemversion "latest"

        defines
        {
            "COCO_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "COCO_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "COCO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "COCO_DIST"
        runtime "Release"
        optimize "on"
    
project "Cocobox"
    location "Cocobox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
        
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
        
    includedirs
    {
        "%{IncludeDir.src}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}"
    }
    
    links
    {
        "Coco"
    }
    
    defines
    {
        "COCO_IMGUI=1",
        "COCO_ASSERTS=1"
    }
    
    filter "system:windows" 
        systemversion "latest"

        defines
        {
            "COCO_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "COCO_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "COCO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "COCO_DIST"
        runtime "Release"
        optimize "on"
