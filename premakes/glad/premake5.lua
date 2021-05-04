project "Glad"
    location "../../CocoEngine/vendor/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../CocoEngine/vendor/glad/include/glad/glad.h",
        "../../CocoEngine/vendor/glad/include/KHR/khrplatform.h",
        "../../CocoEngine/vendor/glad/src/glad.c"
    }

    includedirs
    {
        "../../CocoEngine/vendor/glad/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"