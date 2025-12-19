#include "proxy_exports.h"
#include <stdio.h>
#include <string.h>

#if IS_PROXY_WINHTTP
typedef WORD INTERNET_PORT;
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
typedef BOOL(WINAPI *WinHttpAddRequestHeaders_t)(LPVOID, LPCWSTR, DWORD, DWORD);
typedef BOOL(WINAPI *WinHttpCheckPlatform_t)(VOID);
typedef BOOL(WINAPI *WinHttpCrackUrl_t)(LPCWSTR, DWORD, DWORD, LPVOID);
typedef BOOL(WINAPI *WinHttpCreateUrl_t)(LPVOID, DWORD, LPWSTR, LPDWORD);
typedef BOOL(WINAPI *WinHttpDetectAutoProxyConfigUrl_t)(DWORD, LPWSTR *);
typedef BOOL(WINAPI *WinHttpGetDefaultProxyConfiguration_t)(LPVOID);
typedef BOOL(WINAPI *WinHttpGetIEProxyConfigForCurrentUser_t)(LPVOID);
typedef BOOL(WINAPI *WinHttpGetProxyForUrl_t)(LPVOID, LPCWSTR, LPVOID, LPVOID);
typedef LPVOID(WINAPI *WinHttpSetStatusCallback_t)(LPVOID, LPVOID, DWORD, DWORD_PTR);
typedef BOOL(WINAPI *WinHttpSetTimeouts_t)(LPVOID, int, int, int, int);
typedef BOOL(WINAPI *WinHttpTimeFromSystemTime_t)(CONST SYSTEMTIME *, LPWSTR);
typedef BOOL(WINAPI *WinHttpTimeToSystemTime_t)(LPCWSTR, SYSTEMTIME *);
typedef BOOL(WINAPI *WinHttpWriteData_t)(LPVOID, LPCVOID, DWORD, LPDWORD);
typedef BOOL(WINAPI *WinHttpQueryAuthSchemes_t)(LPVOID, LPDWORD, LPDWORD, LPDWORD);
typedef BOOL(WINAPI *WinHttpSetCredentials_t)(LPVOID, DWORD, DWORD, LPCWSTR, LPCWSTR, LPVOID);
typedef DWORD(WINAPI *WinHttpSetDefaultProxyConfiguration_t)(LPVOID);
typedef BOOL(WINAPI *WinHttpWebSocketClose_t)(LPVOID, USHORT, LPCVOID, DWORD);
typedef BOOL(WINAPI *WinHttpWebSocketCompleteUpgrade_t)(LPVOID, DWORD_PTR);
typedef LPVOID(WINAPI *WinHttpWebSocketQueryCloseStatus_t)(LPVOID, USHORT *, LPVOID, DWORD, LPDWORD);
typedef BOOL(WINAPI *WinHttpWebSocketReceive_t)(LPVOID, LPVOID, DWORD, LPDWORD, LPVOID);
typedef BOOL(WINAPI *WinHttpWebSocketSend_t)(LPVOID, DWORD, LPVOID, DWORD);
typedef BOOL(WINAPI *WinHttpWebSocketShutdown_t)(LPVOID, USHORT, LPCVOID, DWORD);
typedef DWORD(WINAPI *WinHttpGetProxyResult_t)(LPVOID, LPVOID);
typedef DWORD(WINAPI *WinHttpFreeProxyResult_t)(LPVOID);
typedef DWORD(WINAPI *WinHttpGetProxyResultEx_t)(LPVOID, LPVOID);
typedef DWORD(WINAPI *WinHttpResetAutoProxy_t)(LPVOID, DWORD);
typedef DWORD(WINAPI *WinHttpCreateProxyResolver_t)(LPVOID, LPVOID *);
typedef DWORD(WINAPI *WinHttpGetProxyForUrlEx_t)(LPVOID, LPCWSTR, LPVOID, DWORD_PTR);
typedef DWORD(WINAPI *WinHttpGetProxyForUrlEx2_t)(LPVOID, LPCWSTR, LPVOID, DWORD_PTR);
typedef DWORD(WINAPI *WinHttpFreeProxyResultEx_t)(LPVOID);
typedef DWORD(WINAPI *WinHttpFreeProxySettings_t)(LPVOID);
typedef DWORD(WINAPI *WinHttpReadDataEx_t)(LPVOID, LPVOID, DWORD, LPDWORD, ULONGLONG, DWORD, DWORD);
typedef DWORD(WINAPI *WinHttpWriteDataEx_t)(LPVOID, LPCVOID, DWORD, LPDWORD, ULONGLONG, DWORD, DWORD);
typedef DWORD(WINAPI *WinHttpQueryHeadersEx_t)(LPVOID, DWORD, ULONGLONG, ULONGLONG, ULONGLONG, LPVOID, LPDWORD, LPVOID *, LPVOID *);
typedef DWORD(WINAPI *WinHttpRegisterProxyChangeNotification_t)(DWORD, LPVOID, LPVOID, LPVOID);
typedef DWORD(WINAPI *WinHttpUnregisterProxyChangeNotification_t)(LPVOID);

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
static WinHttpAddRequestHeaders_t OriginalWinHttpAddRequestHeaders = NULL;
static WinHttpCheckPlatform_t OriginalWinHttpCheckPlatform = NULL;
static WinHttpCrackUrl_t OriginalWinHttpCrackUrl = NULL;
static WinHttpCreateUrl_t OriginalWinHttpCreateUrl = NULL;
static WinHttpDetectAutoProxyConfigUrl_t OriginalWinHttpDetectAutoProxyConfigUrl = NULL;
static WinHttpGetDefaultProxyConfiguration_t OriginalWinHttpGetDefaultProxyConfiguration = NULL;
static WinHttpGetIEProxyConfigForCurrentUser_t OriginalWinHttpGetIEProxyConfigForCurrentUser = NULL;
static WinHttpGetProxyForUrl_t OriginalWinHttpGetProxyForUrl = NULL;
static WinHttpSetStatusCallback_t OriginalWinHttpSetStatusCallback = NULL;
static WinHttpSetTimeouts_t OriginalWinHttpSetTimeouts = NULL;
static WinHttpTimeFromSystemTime_t OriginalWinHttpTimeFromSystemTime = NULL;
static WinHttpTimeToSystemTime_t OriginalWinHttpTimeToSystemTime = NULL;
static WinHttpWriteData_t OriginalWinHttpWriteData = NULL;
static WinHttpQueryAuthSchemes_t OriginalWinHttpQueryAuthSchemes = NULL;
static WinHttpSetCredentials_t OriginalWinHttpSetCredentials = NULL;
static WinHttpSetDefaultProxyConfiguration_t OriginalWinHttpSetDefaultProxyConfiguration = NULL;
static WinHttpWebSocketClose_t OriginalWinHttpWebSocketClose = NULL;
static WinHttpWebSocketCompleteUpgrade_t OriginalWinHttpWebSocketCompleteUpgrade = NULL;
static WinHttpWebSocketQueryCloseStatus_t OriginalWinHttpWebSocketQueryCloseStatus = NULL;
static WinHttpWebSocketReceive_t OriginalWinHttpWebSocketReceive = NULL;
static WinHttpWebSocketSend_t OriginalWinHttpWebSocketSend = NULL;
static WinHttpWebSocketShutdown_t OriginalWinHttpWebSocketShutdown = NULL;
static WinHttpGetProxyResult_t OriginalWinHttpGetProxyResult = NULL;
static WinHttpFreeProxyResult_t OriginalWinHttpFreeProxyResult = NULL;
static WinHttpGetProxyResultEx_t OriginalWinHttpGetProxyResultEx = NULL;
static WinHttpResetAutoProxy_t OriginalWinHttpResetAutoProxy = NULL;
static WinHttpCreateProxyResolver_t OriginalWinHttpCreateProxyResolver = NULL;
static WinHttpGetProxyForUrlEx_t OriginalWinHttpGetProxyForUrlEx = NULL;
static WinHttpGetProxyForUrlEx2_t OriginalWinHttpGetProxyForUrlEx2 = NULL;
static WinHttpFreeProxyResultEx_t OriginalWinHttpFreeProxyResultEx = NULL;
static WinHttpFreeProxySettings_t OriginalWinHttpFreeProxySettings = NULL;
static WinHttpReadDataEx_t OriginalWinHttpReadDataEx = NULL;
static WinHttpWriteDataEx_t OriginalWinHttpWriteDataEx = NULL;
static WinHttpQueryHeadersEx_t OriginalWinHttpQueryHeadersEx = NULL;
static WinHttpRegisterProxyChangeNotification_t OriginalWinHttpRegisterProxyChangeNotification = NULL;
static WinHttpUnregisterProxyChangeNotification_t OriginalWinHttpUnregisterProxyChangeNotification = NULL;
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
    OriginalWinHttpAddRequestHeaders = (WinHttpAddRequestHeaders_t)GetProcAddress(hOriginalDll, "WinHttpAddRequestHeaders");
    OriginalWinHttpCheckPlatform = (WinHttpCheckPlatform_t)GetProcAddress(hOriginalDll, "WinHttpCheckPlatform");
    OriginalWinHttpCrackUrl = (WinHttpCrackUrl_t)GetProcAddress(hOriginalDll, "WinHttpCrackUrl");
    OriginalWinHttpCreateUrl = (WinHttpCreateUrl_t)GetProcAddress(hOriginalDll, "WinHttpCreateUrl");
    OriginalWinHttpDetectAutoProxyConfigUrl = (WinHttpDetectAutoProxyConfigUrl_t)GetProcAddress(hOriginalDll, "WinHttpDetectAutoProxyConfigUrl");
    OriginalWinHttpGetDefaultProxyConfiguration = (WinHttpGetDefaultProxyConfiguration_t)GetProcAddress(hOriginalDll, "WinHttpGetDefaultProxyConfiguration");
    OriginalWinHttpGetIEProxyConfigForCurrentUser = (WinHttpGetIEProxyConfigForCurrentUser_t)GetProcAddress(hOriginalDll, "WinHttpGetIEProxyConfigForCurrentUser");
    OriginalWinHttpGetProxyForUrl = (WinHttpGetProxyForUrl_t)GetProcAddress(hOriginalDll, "WinHttpGetProxyForUrl");
    OriginalWinHttpSetStatusCallback = (WinHttpSetStatusCallback_t)GetProcAddress(hOriginalDll, "WinHttpSetStatusCallback");
    OriginalWinHttpSetTimeouts = (WinHttpSetTimeouts_t)GetProcAddress(hOriginalDll, "WinHttpSetTimeouts");
    OriginalWinHttpTimeFromSystemTime = (WinHttpTimeFromSystemTime_t)GetProcAddress(hOriginalDll, "WinHttpTimeFromSystemTime");
    OriginalWinHttpTimeToSystemTime = (WinHttpTimeToSystemTime_t)GetProcAddress(hOriginalDll, "WinHttpTimeToSystemTime");
    OriginalWinHttpWriteData = (WinHttpWriteData_t)GetProcAddress(hOriginalDll, "WinHttpWriteData");
    OriginalWinHttpQueryAuthSchemes = (WinHttpQueryAuthSchemes_t)GetProcAddress(hOriginalDll, "WinHttpQueryAuthSchemes");
    OriginalWinHttpSetCredentials = (WinHttpSetCredentials_t)GetProcAddress(hOriginalDll, "WinHttpSetCredentials");
    OriginalWinHttpSetDefaultProxyConfiguration = (WinHttpSetDefaultProxyConfiguration_t)GetProcAddress(hOriginalDll, "WinHttpSetDefaultProxyConfiguration");
    OriginalWinHttpWebSocketClose = (WinHttpWebSocketClose_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketClose");
    OriginalWinHttpWebSocketCompleteUpgrade = (WinHttpWebSocketCompleteUpgrade_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketCompleteUpgrade");
    OriginalWinHttpWebSocketQueryCloseStatus = (WinHttpWebSocketQueryCloseStatus_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketQueryCloseStatus");
    OriginalWinHttpWebSocketReceive = (WinHttpWebSocketReceive_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketReceive");
    OriginalWinHttpWebSocketSend = (WinHttpWebSocketSend_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketSend");
    OriginalWinHttpWebSocketShutdown = (WinHttpWebSocketShutdown_t)GetProcAddress(hOriginalDll, "WinHttpWebSocketShutdown");
    OriginalWinHttpGetProxyResult = (WinHttpGetProxyResult_t)GetProcAddress(hOriginalDll, "WinHttpGetProxyResult");
    OriginalWinHttpFreeProxyResult = (WinHttpFreeProxyResult_t)GetProcAddress(hOriginalDll, "WinHttpFreeProxyResult");
    OriginalWinHttpGetProxyResultEx = (WinHttpGetProxyResultEx_t)GetProcAddress(hOriginalDll, "WinHttpGetProxyResultEx");
    OriginalWinHttpResetAutoProxy = (WinHttpResetAutoProxy_t)GetProcAddress(hOriginalDll, "WinHttpResetAutoProxy");
    OriginalWinHttpCreateProxyResolver = (WinHttpCreateProxyResolver_t)GetProcAddress(hOriginalDll, "WinHttpCreateProxyResolver");
    OriginalWinHttpGetProxyForUrlEx = (WinHttpGetProxyForUrlEx_t)GetProcAddress(hOriginalDll, "WinHttpGetProxyForUrlEx");
    OriginalWinHttpGetProxyForUrlEx2 = (WinHttpGetProxyForUrlEx2_t)GetProcAddress(hOriginalDll, "WinHttpGetProxyForUrlEx2");
    OriginalWinHttpFreeProxyResultEx = (WinHttpFreeProxyResultEx_t)GetProcAddress(hOriginalDll, "WinHttpFreeProxyResultEx");
    OriginalWinHttpFreeProxySettings = (WinHttpFreeProxySettings_t)GetProcAddress(hOriginalDll, "WinHttpFreeProxySettings");
    OriginalWinHttpReadDataEx = (WinHttpReadDataEx_t)GetProcAddress(hOriginalDll, "WinHttpReadDataEx");
    OriginalWinHttpWriteDataEx = (WinHttpWriteDataEx_t)GetProcAddress(hOriginalDll, "WinHttpWriteDataEx");
    OriginalWinHttpQueryHeadersEx = (WinHttpQueryHeadersEx_t)GetProcAddress(hOriginalDll, "WinHttpQueryHeadersEx");
    OriginalWinHttpRegisterProxyChangeNotification = (WinHttpRegisterProxyChangeNotification_t)GetProcAddress(hOriginalDll, "WinHttpRegisterProxyChangeNotification");
    OriginalWinHttpUnregisterProxyChangeNotification = (WinHttpUnregisterProxyChangeNotification_t)GetProcAddress(hOriginalDll, "WinHttpUnregisterProxyChangeNotification");
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

BOOL WINAPI WinHttpAddRequestHeaders(LPVOID hRequest, LPCWSTR pwszHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
    if (OriginalWinHttpAddRequestHeaders) {
        return OriginalWinHttpAddRequestHeaders(hRequest, pwszHeaders, dwHeadersLength, dwModifiers);
    }
    return FALSE;
}

BOOL WINAPI WinHttpCheckPlatform(VOID)
{
    if (OriginalWinHttpCheckPlatform) {
        return OriginalWinHttpCheckPlatform();
    }
    return FALSE;
}

BOOL WINAPI WinHttpCrackUrl(LPCWSTR pwszUrl, DWORD dwUrlLength, DWORD dwFlags, LPVOID lpUrlComponents)
{
    if (OriginalWinHttpCrackUrl) {
        return OriginalWinHttpCrackUrl(pwszUrl, dwUrlLength, dwFlags, lpUrlComponents);
    }
    return FALSE;
}

BOOL WINAPI WinHttpCreateUrl(LPVOID lpUrlComponents, DWORD dwFlags, LPWSTR pwszUrl, LPDWORD lpdwUrlLength)
{
    if (OriginalWinHttpCreateUrl) {
        return OriginalWinHttpCreateUrl(lpUrlComponents, dwFlags, pwszUrl, lpdwUrlLength);
    }
    return FALSE;
}

BOOL WINAPI WinHttpDetectAutoProxyConfigUrl(DWORD dwAutoDetectFlags, LPWSTR *ppwszAutoConfigUrl)
{
    if (OriginalWinHttpDetectAutoProxyConfigUrl) {
        return OriginalWinHttpDetectAutoProxyConfigUrl(dwAutoDetectFlags, ppwszAutoConfigUrl);
    }
    return FALSE;
}

BOOL WINAPI WinHttpGetDefaultProxyConfiguration(LPVOID pProxyInfo)
{
    if (OriginalWinHttpGetDefaultProxyConfiguration) {
        return OriginalWinHttpGetDefaultProxyConfiguration(pProxyInfo);
    }
    return FALSE;
}

BOOL WINAPI WinHttpGetIEProxyConfigForCurrentUser(LPVOID pProxyConfig)
{
    if (OriginalWinHttpGetIEProxyConfigForCurrentUser) {
        return OriginalWinHttpGetIEProxyConfigForCurrentUser(pProxyConfig);
    }
    return FALSE;
}

BOOL WINAPI WinHttpGetProxyForUrl(LPVOID hSession, LPCWSTR lpcwszUrl, LPVOID pAutoProxyOptions, LPVOID pProxyInfo)
{
    if (OriginalWinHttpGetProxyForUrl) {
        return OriginalWinHttpGetProxyForUrl(hSession, lpcwszUrl, pAutoProxyOptions, pProxyInfo);
    }
    return FALSE;
}

LPVOID WINAPI WinHttpSetStatusCallback(LPVOID hInternet, LPVOID lpfnInternetCallback, DWORD dwNotificationFlags, DWORD_PTR dwReserved)
{
    if (OriginalWinHttpSetStatusCallback) {
        return OriginalWinHttpSetStatusCallback(hInternet, lpfnInternetCallback, dwNotificationFlags, dwReserved);
    }
    return NULL;
}

BOOL WINAPI WinHttpSetTimeouts(LPVOID hInternet, int nResolveTimeout, int nConnectTimeout, int nSendTimeout, int nReceiveTimeout)
{
    if (OriginalWinHttpSetTimeouts) {
        return OriginalWinHttpSetTimeouts(hInternet, nResolveTimeout, nConnectTimeout, nSendTimeout, nReceiveTimeout);
    }
    return FALSE;
}

BOOL WINAPI WinHttpTimeFromSystemTime(CONST SYSTEMTIME *pst, LPWSTR pwszTime)
{
    if (OriginalWinHttpTimeFromSystemTime) {
        return OriginalWinHttpTimeFromSystemTime(pst, pwszTime);
    }
    return FALSE;
}

BOOL WINAPI WinHttpTimeToSystemTime(LPCWSTR pwszTime, SYSTEMTIME *pst)
{
    if (OriginalWinHttpTimeToSystemTime) {
        return OriginalWinHttpTimeToSystemTime(pwszTime, pst);
    }
    return FALSE;
}

BOOL WINAPI WinHttpWriteData(LPVOID hRequest, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten)
{
    if (OriginalWinHttpWriteData) {
        return OriginalWinHttpWriteData(hRequest, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
    }
    return FALSE;
}

BOOL WINAPI WinHttpQueryAuthSchemes(LPVOID hRequest, LPDWORD lpdwSupportedSchemes, LPDWORD lpdwFirstScheme, LPDWORD lpdwAuthTarget)
{
    if (OriginalWinHttpQueryAuthSchemes) {
        return OriginalWinHttpQueryAuthSchemes(hRequest, lpdwSupportedSchemes, lpdwFirstScheme, lpdwAuthTarget);
    }
    return FALSE;
}

BOOL WINAPI WinHttpSetCredentials(LPVOID hRequest, DWORD AuthTargets, DWORD AuthScheme, LPCWSTR pwszUserName, LPCWSTR pwszPassword, LPVOID pAuthParams)
{
    if (OriginalWinHttpSetCredentials) {
        return OriginalWinHttpSetCredentials(hRequest, AuthTargets, AuthScheme, pwszUserName, pwszPassword, pAuthParams);
    }
    return FALSE;
}

DWORD WINAPI WinHttpSetDefaultProxyConfiguration(LPVOID pProxyInfo)
{
    if (OriginalWinHttpSetDefaultProxyConfiguration) {
        return OriginalWinHttpSetDefaultProxyConfiguration(pProxyInfo);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

BOOL WINAPI WinHttpWebSocketClose(LPVOID hWebSocket, USHORT usStatus, LPCVOID pvReason, DWORD dwReasonLength)
{
    if (OriginalWinHttpWebSocketClose) {
        return OriginalWinHttpWebSocketClose(hWebSocket, usStatus, pvReason, dwReasonLength);
    }
    return FALSE;
}

BOOL WINAPI WinHttpWebSocketCompleteUpgrade(LPVOID hRequest, DWORD_PTR pContext)
{
    if (OriginalWinHttpWebSocketCompleteUpgrade) {
        return OriginalWinHttpWebSocketCompleteUpgrade(hRequest, pContext);
    }
    return FALSE;
}

LPVOID WINAPI WinHttpWebSocketQueryCloseStatus(LPVOID hWebSocket, USHORT *pusStatus, LPVOID pvReason, DWORD dwReasonLength, LPDWORD pdwReasonLengthConsumed)
{
    if (OriginalWinHttpWebSocketQueryCloseStatus) {
        return OriginalWinHttpWebSocketQueryCloseStatus(hWebSocket, pusStatus, pvReason, dwReasonLength, pdwReasonLengthConsumed);
    }
    return NULL;
}

BOOL WINAPI WinHttpWebSocketReceive(LPVOID hWebSocket, LPVOID pvBuffer, DWORD dwBufferLength, LPDWORD pdwBytesRead, LPVOID peBufferType)
{
    if (OriginalWinHttpWebSocketReceive) {
        return OriginalWinHttpWebSocketReceive(hWebSocket, pvBuffer, dwBufferLength, pdwBytesRead, peBufferType);
    }
    return FALSE;
}

BOOL WINAPI WinHttpWebSocketSend(LPVOID hWebSocket, DWORD eBufferType, LPVOID pvBuffer, DWORD dwBufferLength)
{
    if (OriginalWinHttpWebSocketSend) {
        return OriginalWinHttpWebSocketSend(hWebSocket, eBufferType, pvBuffer, dwBufferLength);
    }
    return FALSE;
}

BOOL WINAPI WinHttpWebSocketShutdown(LPVOID hWebSocket, USHORT usStatus, LPCVOID pvReason, DWORD dwReasonLength)
{
    if (OriginalWinHttpWebSocketShutdown) {
        return OriginalWinHttpWebSocketShutdown(hWebSocket, usStatus, pvReason, dwReasonLength);
    }
    return FALSE;
}

DWORD WINAPI WinHttpGetProxyResult(LPVOID hResolver, LPVOID pProxyResult)
{
    if (OriginalWinHttpGetProxyResult) {
        return OriginalWinHttpGetProxyResult(hResolver, pProxyResult);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpFreeProxyResult(LPVOID pProxyResult)
{
    if (OriginalWinHttpFreeProxyResult) {
        return OriginalWinHttpFreeProxyResult(pProxyResult);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpGetProxyResultEx(LPVOID hResolver, LPVOID pProxyResultEx)
{
    if (OriginalWinHttpGetProxyResultEx) {
        return OriginalWinHttpGetProxyResultEx(hResolver, pProxyResultEx);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpResetAutoProxy(LPVOID hSession, DWORD dwFlags)
{
    if (OriginalWinHttpResetAutoProxy) {
        return OriginalWinHttpResetAutoProxy(hSession, dwFlags);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpCreateProxyResolver(LPVOID hSession, LPVOID *phResolver)
{
    if (OriginalWinHttpCreateProxyResolver) {
        return OriginalWinHttpCreateProxyResolver(hSession, phResolver);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpGetProxyForUrlEx(LPVOID hResolver, LPCWSTR pcwszUrl, LPVOID pAutoProxyOptions, DWORD_PTR pContext)
{
    if (OriginalWinHttpGetProxyForUrlEx) {
        return OriginalWinHttpGetProxyForUrlEx(hResolver, pcwszUrl, pAutoProxyOptions, pContext);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpGetProxyForUrlEx2(LPVOID hResolver, LPCWSTR pcwszUrl, LPVOID pAutoProxyOptions, DWORD_PTR pContext)
{
    if (OriginalWinHttpGetProxyForUrlEx2) {
        return OriginalWinHttpGetProxyForUrlEx2(hResolver, pcwszUrl, pAutoProxyOptions, pContext);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpFreeProxyResultEx(LPVOID pProxyResultEx)
{
    if (OriginalWinHttpFreeProxyResultEx) {
        return OriginalWinHttpFreeProxyResultEx(pProxyResultEx);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpFreeProxySettings(LPVOID pProxySettings)
{
    if (OriginalWinHttpFreeProxySettings) {
        return OriginalWinHttpFreeProxySettings(pProxySettings);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpReadDataEx(LPVOID hRequest, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead, ULONGLONG ullFlags, DWORD cbProperty, DWORD cbBuffers)
{
    if (OriginalWinHttpReadDataEx) {
        return OriginalWinHttpReadDataEx(hRequest, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead, ullFlags, cbProperty, cbBuffers);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpWriteDataEx(LPVOID hRequest, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten, ULONGLONG ullFlags, DWORD cbProperty, DWORD cbBuffers)
{
    if (OriginalWinHttpWriteDataEx) {
        return OriginalWinHttpWriteDataEx(hRequest, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten, ullFlags, cbProperty, cbBuffers);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpQueryHeadersEx(LPVOID hRequest, DWORD dwInfoLevel, ULONGLONG ullFlags, ULONGLONG uiCodePage, ULONGLONG puiIndex, LPVOID pBuffer, LPDWORD pdwBufferLength, LPVOID *ppvExtraInfo, LPVOID *ppReserved)
{
    if (OriginalWinHttpQueryHeadersEx) {
        return OriginalWinHttpQueryHeadersEx(hRequest, dwInfoLevel, ullFlags, uiCodePage, puiIndex, pBuffer, pdwBufferLength, ppvExtraInfo, ppReserved);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpRegisterProxyChangeNotification(DWORD dwFlags, LPVOID hEvent, LPVOID pvContext, LPVOID phNotification)
{
    if (OriginalWinHttpRegisterProxyChangeNotification) {
        return OriginalWinHttpRegisterProxyChangeNotification(dwFlags, hEvent, pvContext, phNotification);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI WinHttpUnregisterProxyChangeNotification(LPVOID hNotification)
{
    if (OriginalWinHttpUnregisterProxyChangeNotification) {
        return OriginalWinHttpUnregisterProxyChangeNotification(hNotification);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}
#endif // IS_PROXY_WINHTTP