#pragma once

/**
 * Proxy DLL configuration
 * 
 * The DLL to proxy is determined at compile time using CMake options.
 * Use -DPROXY_DLL=VERSION or -DPROXY_DLL=WINHTTP when building.
 * 
 * This file defines which DLL is being proxied based on preprocessor definitions.
 */

// Check which DLL we're proxying (set by CMake)
#if defined(PROXY_WINHTTP)
    #define PROXY_DLL_NAME "winhttp.dll"
    #define IS_PROXY_WINHTTP 1
    #define IS_PROXY_VERSION 0
#else
    // Default to version.dll for backward compatibility
    #define PROXY_DLL_NAME "version.dll"
    #define IS_PROXY_WINHTTP 0
    #define IS_PROXY_VERSION 1
#endif