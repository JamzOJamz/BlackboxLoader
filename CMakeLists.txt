cmake_minimum_required(VERSION 3.15)
project(BlackboxLoader C)

# Option to select which DLL to proxy
# Valid values: VERSION (default), WINHTTP
set(PROXY_DLL "VERSION" CACHE STRING "DLL to proxy (VERSION or WINHTTP)")
set_property(CACHE PROXY_DLL PROPERTY STRINGS VERSION WINHTTP)

# Source files common to all configurations
set(COMMON_SOURCES
    src/dllmain.c
    src/plugin_loader.c
    src/proxy_exports.c
    src/ini_parser.c
    src/config_reader.c
)

# Configure based on proxy type
if(PROXY_DLL STREQUAL "WINHTTP")
    # Build as winhttp.dll proxy
    add_library(BlackboxLoader SHARED 
        ${COMMON_SOURCES}
        winhttp.def
    )
    
    target_compile_definitions(BlackboxLoader PRIVATE PROXY_WINHTTP)
    
    set_target_properties(BlackboxLoader PROPERTIES
        OUTPUT_NAME "winhttp"
        PREFIX ""
    )
else()
    # Default: Build as version.dll proxy (backward compatible)
    add_library(BlackboxLoader SHARED 
        ${COMMON_SOURCES}
        version.def
    )
    
    set_target_properties(BlackboxLoader PROPERTIES
        OUTPUT_NAME "version"
        PREFIX ""
    )
endif()

# Common includes
target_include_directories(BlackboxLoader PRIVATE src)