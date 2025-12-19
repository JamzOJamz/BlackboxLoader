/**
 * BlackboxLoader - DLL Main Entry Point
 * 
 * This is the main entry point for the BlackboxLoader DLL proxy.
 * The DLL to proxy is determined at compile time using CMake options.
 * 
 * Modular structure:
 * - config.h: Defines which DLL to proxy (version.dll or winhttp.dll)
 * - plugin_loader.c/h: Handles loading of plugins from Blackbox/ directory
 * - proxy_exports.c/h: Implements proxy functions for the target DLL
 * - dllmain.c: Main DLL entry point (this file)
 */

#include <windows.h>
#include "config.h"
#include "plugin_loader.h"
#include "proxy_exports.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        // Initialize the proxy to the configured DLL
        if (!InitializeProxy()) {
            return FALSE;
        }

        // Load all plugins from Blackbox/ directory
        LoadPlugins();
    } 
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // Clean up plugins
        UnloadPlugins();
        
        // Clean up proxy resources
        CleanupProxy();
    }

    return TRUE;
}