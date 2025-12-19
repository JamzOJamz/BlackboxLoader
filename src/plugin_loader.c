#include "plugin_loader.h"
#include "config_reader.h"
#include <stdio.h>
#include <stdlib.h>

static HMODULE* hPluginDlls = NULL;
static int pluginCount = 0;

void LoadPlugins()
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    char dllPath[MAX_PATH];
    int capacity = 10;

    if (!ShouldActivateLoader()) {
        return;
    }

    hPluginDlls = (HMODULE*)malloc(capacity * sizeof(HMODULE));
    if (hPluginDlls == NULL) {
        return;
    }

    hFind = FindFirstFileA(".\\Blackbox\\*.dll", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        free(hPluginDlls);
        hPluginDlls = NULL;
        return;
    }

    do {
        // Validate filename length to prevent buffer overflow
        if (strlen(findData.cFileName) > MAX_PATH - 12) { // 12 = length of ".\\Blackbox\\" + null terminator
            continue;
        }
        
        snprintf(dllPath, MAX_PATH, ".\\Blackbox\\%s", findData.cFileName);

        HMODULE hModule = LoadLibraryA(dllPath);
        if (hModule != NULL) {
            if (pluginCount >= capacity) {
                capacity *= 2;
                HMODULE* newPlugins = (HMODULE*)realloc(hPluginDlls, capacity * sizeof(HMODULE));
                if (newPlugins == NULL) {
                    // Allocation failed, continue with current capacity
                    FreeLibrary(hModule);
                    break;
                }
                hPluginDlls = newPlugins;
            }
            hPluginDlls[pluginCount++] = hModule;
        }

    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
}

void UnloadPlugins()
{
    if (hPluginDlls != NULL) {
        for (int i = 0; i < pluginCount; i++) {
            if (hPluginDlls[i] != NULL) {
                FreeLibrary(hPluginDlls[i]);
            }
        }
        free(hPluginDlls);
        hPluginDlls = NULL;
        pluginCount = 0;
    }
}