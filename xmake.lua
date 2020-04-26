set_languages("c++11")

target("DCE")
    set_kind("static")
    add_files("DCE/source/**.cpp")
    add_includedirs("DCE/include", {public = true})
    add_includedirs("Submodules/PIMPL/PIMPL/include", {public = true})

target("dyld_cache_extract")
    set_kind("binary")
    add_deps("DCE")
    add_files("dyld_cache_extract/*.cpp")
