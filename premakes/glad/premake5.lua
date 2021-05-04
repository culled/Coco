project "Glad"
    location "../../Coco/vendor/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../Coco/vendor/glad/include/glad/glad.h",
        "../../Coco/vendor/glad/include/KHR/khrplatform.h",
        "../../Coco/vendor/glad/src/glad.c"
    }

    includedirs
    {
        "../../Coco/vendor/glad/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"