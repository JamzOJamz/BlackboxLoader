#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

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

#endif // PLUGIN_LOADER_H
