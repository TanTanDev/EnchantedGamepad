solution "EnchantedGamepad"
    configurations { "Debug", "Final" }
    platforms { "Win32", "Win64" }

project "EnchantedGamepad"
    filter { 'system:windows' }
        files { '**.rc', '**.ico' }
    filter {}

    kind "WindowedApp"
    entrypoint "mainCRTStartup"
    language "C++"
    targetdir "../bin/%{cfg.architecture}"
    files { "code/**.h", "code/**.hpp", "code/**.cpp", "code/**.c", "code/**.frag" }
    includedirs { "code" }
    links { "mscoree" }
    --flags { "Unicode" }
	toolset "v140"
	debugdir "../bin"

    -- Set up platforms.
    filter { "platforms:Win32" }
        architecture "x32"

    filter { "platforms:Win64" }
        architecture "x64"

    filter { "platforms:Win*" }
        buildoptions { "/wd4127 /wd4510 /wd4512 /wd4458" }

    -- Using an older XInput version to support all windows platforms
    filter{}
        links {"Xinput9_1_0"}

    -- Set intermediate directory.
    filter { "configurations:Debug" }
        objdir "../build/debug"
    filter { "configurations:Final" }
        objdir "../build/final"

    -- Debug configuration.
    filter { "configurations:Debug" }
        symbols "On" 
        defines { "DEBUG", "_DEBUG" }

    -- Final configuration.
    filter { "configurations:not Debug" }
        defines { "NDEBUG", "FINAL" }
        optimize "On"

	-- LUA
	filter{}
		includedirs{ "../extlibs/include/LuaJIT"}
		libdirs{ "../extlibs/lib/%{cfg.architecture}/LuaJIT"}
        links{"lua51"}


            -- SFML
    filter{}
        includedirs{ "../extlibs/include"}
        libdirs{ "../extlibs/lib/%{cfg.architecture}/SFML"}
        defines{"SFML_STATIC"}
        --links{"opengl32"}
        --links{"glfw3"}

    filter{"configurations:Debug"}
        links{"sfml-graphics-s-d"}
        links{"sfml-window-s-d"}
        links{"sfml-system-s-d"}
    filter{"configurations:not Debug"}
        links{"sfml-graphics-s"}
        links{"sfml-window-s"}
        links{"sfml-system-s"}
    filter{}
        links{"opengl32"}
        links{"freetype"}
        links{"jpeg"}
        links{"winmm"}
        links{"gdi32"}

    filter{}
        includedirs{ "../extlibs/include/rapidjson"}
    ---- OpenGL
    --filter{}
    --    includedirs{ "../extlibs/include"}
    --   -- libdirs{ "../extlibs/lib/%{cfg.architecture}/GLFW"}
    --    libdirs{ "../extlibs/lib/%{cfg.architecture}/GL"}
    --    links{"opengl32"}
    --    --links{"glfw3"}