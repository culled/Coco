project "ImGui"
location "%{wks.location}/Coco/vendor/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Coco/vendor/imgui/imconfig.h",
        "%{wks.location}/Coco/vendor/imgui/imgui.h",
        "%{wks.location}/Coco/vendor/imgui/imgui.cpp",
		"%{wks.location}/Coco/vendor/imgui/imgui_draw.cpp",
		"%{wks.location}/Coco/vendor/imgui/imgui_internal.h",
		"%{wks.location}/Coco/vendor/imgui/imgui_widgets.cpp",
		"%{wks.location}/Coco/vendor/imgui/imstb_rectpack.h",
		"%{wks.location}/Coco/vendor/imgui/imstb_textedit.h",
		"%{wks.location}/Coco/vendor/imgui/imstb_truetype.h",
		"%{wks.location}/Coco/vendor/imgui/imgui_demo.cpp",
        "%{wks.location}/Coco/vendor/imgui/imgui_tables.cpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"