include "lzma"

project "gframe"
    kind "WindowedApp"

    files { "**.cpp", "**.cc", "**.c", "**.h" }
    excludes "lzma/**"
    includedirs { "../ocgcore" }
    links { "ocgcore", "clzma", "irrlicht", "freetype", "sqlite3", "lua" }

    configuration "windows"
        links { "opengl32", "wsock32", "winmm", "gdi32", "kernel32", "user32" }
    configuration {"windows", "not vs*"}
        includedirs { "/mingw/include/irrlicht", "/mingw/include/freetype2" }
    configuration "not vs*"
        buildoptions { "-std=gnu++0x" }
    configuration "not windows"
        links { "gl", "dl" }
