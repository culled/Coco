project "Glad"
    location "%{wks.location}/Coco/vendor/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Coco/vendor/glad/include/glad/glad.h",
        "%{wks.location}/Coco/vendor/glad/include/KHR/khrplatform.h",
        "%{wks.location}/Coco/vendor/glad/src/glad.c"
    }

    includedirs
    {
        "%{wks.location}/Coco/vendor/glad/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"