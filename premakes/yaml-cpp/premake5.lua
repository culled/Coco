project "Yaml-cpp"
    location "../../Coco/vendor/yaml-cpp"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../Coco/vendor/yaml-cpp/include/**.h",
        "../../Coco/vendor/yaml-cpp/src/**.cpp",
        "../../Coco/vendor/yaml-cpp/src/**.h"
    }

    includedirs
    {
        "../../Coco/vendor/yaml-cpp/include"
    }

    filter "system:windows"
        systemversion "latest"
        pic "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"