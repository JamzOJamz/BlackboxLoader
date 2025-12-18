#include <windows.h>
#include <stdio.h>

HMODULE hOriginalDll = NULL;
HMODULE* hPluginDlls = NULL;
int pluginCount = 0;

typedef DWORD(WINAPI *GetFileVersionInfoSizeW_t)(LPCWSTR, LPDWORD);
typedef BOOL(WINAPI *GetFileVersionInfoW_t)(LPCWSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *VerQueryValueW_t)(LPCVOID, LPCWSTR, LPVOID *, PUINT);
typedef DWORD(WINAPI *GetFileVersionInfoSizeA_t)(LPCSTR, LPDWORD);
typedef BOOL(WINAPI *GetFileVersionInfoA_t)(LPCSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *VerQueryValueA_t)(LPCVOID, LPCSTR, LPVOID *, PUINT);

GetFileVersionInfoSizeW_t OriginalGetFileVersionInfoSizeW = NULL;
GetFileVersionInfoW_t OriginalGetFileVersionInfoW = NULL;
VerQueryValueW_t OriginalVerQueryValueW = NULL;
GetFileVersionInfoSizeA_t OriginalGetFileVersionInfoSizeA = NULL;
GetFileVersionInfoA_t OriginalGetFileVersionInfoA = NULL;
VerQueryValueA_t OriginalVerQueryValueA = NULL;

// Load all DLLs from Blackbox/
void LoadPlugins()
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    char dllPath[MAX_PATH];
    int capacity = 10;

    hPluginDlls = (HMODULE*)malloc(capacity * sizeof(HMODULE));

    hFind = FindFirstFileA(".\\Blackbox\\*.dll", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        snprintf(dllPath, MAX_PATH, ".\\Blackbox\\%s", findData.cFileName);

        HMODULE hModule = LoadLibraryA(dllPath);
        if (hModule != NULL) {
            if (pluginCount >= capacity) {
                capacity *= 2;
                hPluginDlls = (HMODULE*)realloc(hPluginDlls, capacity * sizeof(HMODULE));
            }
            hPluginDlls[pluginCount++] = hModule;
        }

    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
}

// Load the original DLL and get function addresses
BOOL LoadOriginalDll()
{
    char systemPath[MAX_PATH];

    // Get system directory to load real version.dll
    if (GetSystemDirectoryA(systemPath, MAX_PATH) == 0) {
        return FALSE;
    }

    strcat_s(systemPath, MAX_PATH, "\\version.dll");

    hOriginalDll = LoadLibraryA(systemPath);
    if (hOriginalDll == NULL) {
        return FALSE;
    }

    // Get addresses of original functions
    OriginalGetFileVersionInfoSizeW = (GetFileVersionInfoSizeW_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoSizeW");
    OriginalGetFileVersionInfoW = (GetFileVersionInfoW_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoW");
    OriginalVerQueryValueW = (VerQueryValueW_t)GetProcAddress(hOriginalDll, "VerQueryValueW");
    OriginalGetFileVersionInfoSizeA = (GetFileVersionInfoSizeA_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoSizeA");
    OriginalGetFileVersionInfoA = (GetFileVersionInfoA_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoA");
    OriginalVerQueryValueA = (VerQueryValueA_t)GetProcAddress(hOriginalDll, "VerQueryValueA");

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        if (!LoadOriginalDll()) {
            return FALSE;
        }

        LoadPlugins();
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        for (int i = 0; i < pluginCount; i++) {
            if (hPluginDlls[i] != NULL) {
                FreeLibrary(hPluginDlls[i]);
            }
        }
        free(hPluginDlls);
    }

    return TRUE;
}

DWORD WINAPI GetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle)
{
    if (OriginalGetFileVersionInfoSizeW) {
        return OriginalGetFileVersionInfoSizeW(lptstrFilename, lpdwHandle);
    }
    return 0;
}

BOOL WINAPI GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    if (OriginalGetFileVersionInfoW) {
        return OriginalGetFileVersionInfoW(lptstrFilename, dwHandle, dwLen, lpData);
    }
    return FALSE;
}

BOOL WINAPI VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
    if (OriginalVerQueryValueW) {
        return OriginalVerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);
    }
    return FALSE;
}

DWORD WINAPI GetFileVersionInfoSizeA(LPCSTR lptstrFilename, LPDWORD lpdwHandle)
{
    if (OriginalGetFileVersionInfoSizeA) {
        return OriginalGetFileVersionInfoSizeA(lptstrFilename, lpdwHandle);
    }
    return 0;
}

BOOL WINAPI GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
    if (OriginalGetFileVersionInfoA) {
        return OriginalGetFileVersionInfoA(lptstrFilename, dwHandle, dwLen, lpData);
    }
    return FALSE;
}

BOOL WINAPI VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen)
{
    if (OriginalVerQueryValueA) {
        return OriginalVerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);
    }
    return FALSE;
}