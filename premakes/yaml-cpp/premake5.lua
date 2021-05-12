project "Yaml-cpp"
    location "%{wks.location}/Coco/vendor/yaml-cpp"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Coco/vendor/yaml-cpp/include/**.h",
        "%{wks.location}/Coco/vendor/yaml-cpp/src/**.cpp",
        "%{wks.location}/Coco/vendor/yaml-cpp/src/**.h"
    }

    includedirs
    {
        "%{wks.location}/Coco/vendor/yaml-cpp/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"