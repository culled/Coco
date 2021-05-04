project "ImGui"
location "../../Coco/vendor/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../Coco/vendor/imgui/imconfig.h",
        "../../Coco/vendor/imgui/imgui.h",
        "../../Coco/vendor/imgui/imgui.cpp",
		"../../Coco/vendor/imgui/imgui_draw.cpp",
		"../../Coco/vendor/imgui/imgui_internal.h",
		"../../Coco/vendor/imgui/imgui_widgets.cpp",
		"../../Coco/vendor/imgui/imstb_rectpack.h",
		"../../Coco/vendor/imgui/imstb_textedit.h",
		"../../Coco/vendor/imgui/imstb_truetype.h",
		"../../Coco/vendor/imgui/imgui_demo.cpp",
        "../../Coco/vendor/imgui/imgui_tables.cpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"