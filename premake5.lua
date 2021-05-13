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

VulkanSDK = os.getenv("VULKAN_SDK")

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
IncludeDir["yaml"] =    "Coco/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] ="Coco/vendor/ImGuizmo"
IncludeDir["assimp"] ="Coco/vendor/assimp/include"
IncludeDir["VulkanSDK"] = "%{VulkanSDK}/Include"

LibraryDir = "%{VulkanSDK}/Lib"
DebugLibraryDir = "vendor/VulkanSDKDebug/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir}/VkLayer_utils.lib"

Library["ShaderC_Debug"] =              "%{DebugLibraryDir}/shaderc_sharedd.lib"
Library["SpirV_Cross_Debug"] =          "%{DebugLibraryDir}/spirv-cross-cored.lib"
Library["SpirV_Cross_GLSL_Debug"] =     "%{DebugLibraryDir}/spirv-cross-glsld.lib"
Library["SpirV_Cross_Reflect_Debug"] =  "%{DebugLibraryDir}/spirv-cross-reflectd.lib"
Library["SpirV_Tools_Debug"] =          "%{DebugLibraryDir}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] =                "%{LibraryDir}/shaderc_shared.lib"
Library["SpirV_Cross_Release"] =            "%{LibraryDir}/spirv-cross-core.lib"
Library["SpirV_Cross_GLSL_Release"] =       "%{LibraryDir}/spirv-cross-glsl.lib"
Library["SpirV_Cross_Reflect_Release"] =    "%{LibraryDir}/spirv-cross-reflect.lib"

group "Dependencies"
    include "premakes/glfw"
    include "premakes/glad"
    include "premakes/imgui"
    include "premakes/yaml-cpp"
    include "premakes/imguizmo"
    include "premakes/assimp"
group ""

project "Coco"
    location "Coco"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "Yaml-cpp",
        "ImGuizmo",
        "Assimp",
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

        links
        {
            "%{Library.ShaderC_Debug}",
            "%{Library.SpirV_Cross_Debug}",
            "%{Library.SpirV_Cross_GLSL_Debug}",
            "%{Library.SpirV_Cross_Reflect_Debug}"
        }
    
    filter "configurations:Release"
        defines "COCO_RELEASE"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.ShaderC_Release}",
            "%{Library.SpirV_Cross_Release}",
            "%{Library.SpirV_Cross_GLSL_Release}",
            "%{Library.SpirV_Cross_Reflect_Release}"
        }

    filter "configurations:Dist"
        defines "COCO_DIST"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.ShaderC_Release}",
            "%{Library.SpirV_Cross_Release}",
            "%{Library.SpirV_Cross_GLSL_Release}",
            "%{Library.SpirV_Cross_Reflect_Release}"
        }

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.assimp}"
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
    staticruntime "off"
        
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.assimp}"
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
