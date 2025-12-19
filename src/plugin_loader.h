#pragma once

#include <windows.h>

/**
 * Loads all DLL plugins from the Blackbox/ directory.
 * Plugins are loaded automatically during DLL_PROCESS_ATTACH.
 */
void LoadPlugins();

/**
 * Unloads all previously loaded plugins.
 * Called during DLL_PROCESS_DETACH.
 */
void UnloadPlugins();