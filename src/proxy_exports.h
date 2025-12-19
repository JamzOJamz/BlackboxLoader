#ifndef PROXY_EXPORTS_H
#define PROXY_EXPORTS_H

#include <windows.h>
#include "config.h"

/**
 * Initializes the proxy by loading the original DLL and retrieving function pointers.
 * The DLL to proxy is determined at compile time based on PROXY_WINHTTP define.
 * 
 * @return TRUE if successful, FALSE otherwise
 */
BOOL InitializeProxy();

/**
 * Cleans up resources used by the proxy.
 */
void CleanupProxy();

#endif // PROXY_EXPORTS_H
