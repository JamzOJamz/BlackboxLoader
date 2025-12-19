#include "proxy_exports.h"
#include <stdio.h>
#include <string.h>

#if IS_PROXY_WINHTTP
#include <winhttp.h>
#endif

static HMODULE hOriginalDll = NULL;

#if IS_PROXY_VERSION
// ===== version.dll function pointers =====
typedef DWORD(WINAPI *GetFileVersionInfoSizeW_t)(LPCWSTR, LPDWORD);
typedef BOOL(WINAPI *GetFileVersionInfoW_t)(LPCWSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *VerQueryValueW_t)(LPCVOID, LPCWSTR, LPVOID *, PUINT);
typedef DWORD(WINAPI *GetFileVersionInfoSizeA_t)(LPCSTR, LPDWORD);
typedef BOOL(WINAPI *GetFileVersionInfoA_t)(LPCSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *VerQueryValueA_t)(LPCVOID, LPCSTR, LPVOID *, PUINT);

static GetFileVersionInfoSizeW_t OriginalGetFileVersionInfoSizeW = NULL;
static GetFileVersionInfoW_t OriginalGetFileVersionInfoW = NULL;
static VerQueryValueW_t OriginalVerQueryValueW = NULL;
static GetFileVersionInfoSizeA_t OriginalGetFileVersionInfoSizeA = NULL;
static GetFileVersionInfoA_t OriginalGetFileVersionInfoA = NULL;
static VerQueryValueA_t OriginalVerQueryValueA = NULL;
#endif

#if IS_PROXY_WINHTTP
// ===== winhttp.dll function pointers =====
typedef LPVOID(WINAPI *WinHttpOpen_t)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);
typedef BOOL(WINAPI *WinHttpCloseHandle_t)(LPVOID);
typedef LPVOID(WINAPI *WinHttpConnect_t)(LPVOID, LPCWSTR, INTERNET_PORT, DWORD);
typedef LPVOID(WINAPI *WinHttpOpenRequest_t)(LPVOID, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR *, DWORD);
typedef BOOL(WINAPI *WinHttpSendRequest_t)(LPVOID, LPCWSTR, DWORD, LPVOID, DWORD, DWORD, DWORD_PTR);
typedef BOOL(WINAPI *WinHttpReceiveResponse_t)(LPVOID, LPVOID);
typedef BOOL(WINAPI *WinHttpQueryHeaders_t)(LPVOID, DWORD, LPCWSTR, LPVOID, LPDWORD, LPDWORD);
typedef BOOL(WINAPI *WinHttpQueryDataAvailable_t)(LPVOID, LPDWORD);
typedef BOOL(WINAPI *WinHttpReadData_t)(LPVOID, LPVOID, DWORD, LPDWORD);
typedef BOOL(WINAPI *WinHttpSetOption_t)(LPVOID, DWORD, LPVOID, DWORD);
typedef BOOL(WINAPI *WinHttpQueryOption_t)(LPVOID, DWORD, LPVOID, LPDWORD);

static WinHttpOpen_t OriginalWinHttpOpen = NULL;
static WinHttpCloseHandle_t OriginalWinHttpCloseHandle = NULL;
static WinHttpConnect_t OriginalWinHttpConnect = NULL;
static WinHttpOpenRequest_t OriginalWinHttpOpenRequest = NULL;
static WinHttpSendRequest_t OriginalWinHttpSendRequest = NULL;
static WinHttpReceiveResponse_t OriginalWinHttpReceiveResponse = NULL;
static WinHttpQueryHeaders_t OriginalWinHttpQueryHeaders = NULL;
static WinHttpQueryDataAvailable_t OriginalWinHttpQueryDataAvailable = NULL;
static WinHttpReadData_t OriginalWinHttpReadData = NULL;
static WinHttpSetOption_t OriginalWinHttpSetOption = NULL;
static WinHttpQueryOption_t OriginalWinHttpQueryOption = NULL;
#endif

BOOL InitializeProxy()
{
    char systemPath[MAX_PATH];

    // Get system directory to load the real DLL
    if (GetSystemDirectoryA(systemPath, MAX_PATH) == 0) {
        return FALSE;
    }

    // Build the full path to the system DLL
    int pathLen = (int)strlen(systemPath);
    snprintf(systemPath + pathLen, MAX_PATH - pathLen, "\\%s", PROXY_DLL_NAME);

    hOriginalDll = LoadLibraryA(systemPath);
    if (hOriginalDll == NULL) {
        return FALSE;
    }

    // Load function pointers based on compile-time configuration
#if IS_PROXY_VERSION
    OriginalGetFileVersionInfoSizeW = (GetFileVersionInfoSizeW_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoSizeW");
    OriginalGetFileVersionInfoW = (GetFileVersionInfoW_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoW");
    OriginalVerQueryValueW = (VerQueryValueW_t)GetProcAddress(hOriginalDll, "VerQueryValueW");
    OriginalGetFileVersionInfoSizeA = (GetFileVersionInfoSizeA_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoSizeA");
    OriginalGetFileVersionInfoA = (GetFileVersionInfoA_t)GetProcAddress(hOriginalDll, "GetFileVersionInfoA");
    OriginalVerQueryValueA = (VerQueryValueA_t)GetProcAddress(hOriginalDll, "VerQueryValueA");
#endif

#if IS_PROXY_WINHTTP
    OriginalWinHttpOpen = (WinHttpOpen_t)GetProcAddress(hOriginalDll, "WinHttpOpen");
    OriginalWinHttpCloseHandle = (WinHttpCloseHandle_t)GetProcAddress(hOriginalDll, "WinHttpCloseHandle");
    OriginalWinHttpConnect = (WinHttpConnect_t)GetProcAddress(hOriginalDll, "WinHttpConnect");
    OriginalWinHttpOpenRequest = (WinHttpOpenRequest_t)GetProcAddress(hOriginalDll, "WinHttpOpenRequest");
    OriginalWinHttpSendRequest = (WinHttpSendRequest_t)GetProcAddress(hOriginalDll, "WinHttpSendRequest");
    OriginalWinHttpReceiveResponse = (WinHttpReceiveResponse_t)GetProcAddress(hOriginalDll, "WinHttpReceiveResponse");
    OriginalWinHttpQueryHeaders = (WinHttpQueryHeaders_t)GetProcAddress(hOriginalDll, "WinHttpQueryHeaders");
    OriginalWinHttpQueryDataAvailable = (WinHttpQueryDataAvailable_t)GetProcAddress(hOriginalDll, "WinHttpQueryDataAvailable");
    OriginalWinHttpReadData = (WinHttpReadData_t)GetProcAddress(hOriginalDll, "WinHttpReadData");
    OriginalWinHttpSetOption = (WinHttpSetOption_t)GetProcAddress(hOriginalDll, "WinHttpSetOption");
    OriginalWinHttpQueryOption = (WinHttpQueryOption_t)GetProcAddress(hOriginalDll, "WinHttpQueryOption");
#endif

    return TRUE;
}

void CleanupProxy()
{
    if (hOriginalDll != NULL) {
        FreeLibrary(hOriginalDll);
        hOriginalDll = NULL;
    }
}

#if IS_PROXY_VERSION
// ===== version.dll exported functions =====

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
#endif // IS_PROXY_VERSION

#if IS_PROXY_WINHTTP
// ===== winhttp.dll exported functions =====

LPVOID WINAPI WinHttpOpen(LPCWSTR pwszUserAgent, DWORD dwAccessType, LPCWSTR pwszProxyName, LPCWSTR pwszProxyBypass, DWORD dwFlags)
{
    if (OriginalWinHttpOpen) {
        return OriginalWinHttpOpen(pwszUserAgent, dwAccessType, pwszProxyName, pwszProxyBypass, dwFlags);
    }
    return NULL;
}

BOOL WINAPI WinHttpCloseHandle(LPVOID hInternet)
{
    if (OriginalWinHttpCloseHandle) {
        return OriginalWinHttpCloseHandle(hInternet);
    }
    return FALSE;
}

LPVOID WINAPI WinHttpConnect(LPVOID hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved)
{
    if (OriginalWinHttpConnect) {
        return OriginalWinHttpConnect(hSession, pswzServerName, nServerPort, dwReserved);
    }
    return NULL;
}

LPVOID WINAPI WinHttpOpenRequest(LPVOID hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR *ppwszAcceptTypes, DWORD dwFlags)
{
    if (OriginalWinHttpOpenRequest) {
        return OriginalWinHttpOpenRequest(hConnect, pwszVerb, pwszObjectName, pwszVersion, pwszReferrer, ppwszAcceptTypes, dwFlags);
    }
    return NULL;
}

BOOL WINAPI WinHttpSendRequest(LPVOID hRequest, LPCWSTR pwszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext)
{
    if (OriginalWinHttpSendRequest) {
        return OriginalWinHttpSendRequest(hRequest, pwszHeaders, dwHeadersLength, lpOptional, dwOptionalLength, dwTotalLength, dwContext);
    }
    return FALSE;
}

BOOL WINAPI WinHttpReceiveResponse(LPVOID hRequest, LPVOID lpReserved)
{
    if (OriginalWinHttpReceiveResponse) {
        return OriginalWinHttpReceiveResponse(hRequest, lpReserved);
    }
    return FALSE;
}

BOOL WINAPI WinHttpQueryHeaders(LPVOID hRequest, DWORD dwInfoLevel, LPCWSTR pwszName, LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
{
    if (OriginalWinHttpQueryHeaders) {
        return OriginalWinHttpQueryHeaders(hRequest, dwInfoLevel, pwszName, lpBuffer, lpdwBufferLength, lpdwIndex);
    }
    return FALSE;
}

BOOL WINAPI WinHttpQueryDataAvailable(LPVOID hRequest, LPDWORD lpdwNumberOfBytesAvailable)
{
    if (OriginalWinHttpQueryDataAvailable) {
        return OriginalWinHttpQueryDataAvailable(hRequest, lpdwNumberOfBytesAvailable);
    }
    return FALSE;
}

BOOL WINAPI WinHttpReadData(LPVOID hRequest, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead)
{
    if (OriginalWinHttpReadData) {
        return OriginalWinHttpReadData(hRequest, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
    }
    return FALSE;
}

BOOL WINAPI WinHttpSetOption(LPVOID hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
{
    if (OriginalWinHttpSetOption) {
        return OriginalWinHttpSetOption(hInternet, dwOption, lpBuffer, dwBufferLength);
    }
    return FALSE;
}

BOOL WINAPI WinHttpQueryOption(LPVOID hInternet, DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength)
{
    if (OriginalWinHttpQueryOption) {
        return OriginalWinHttpQueryOption(hInternet, dwOption, lpBuffer, lpdwBufferLength);
    }
    return FALSE;
}
#endif // IS_PROXY_WINHTTP
