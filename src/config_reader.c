#include "config_reader.h"
#include "ini_parser.h"
#include <windows.h>
#include <psapi.h>
#include <string.h>
#include <ctype.h>

#define CONFIG_FILENAME ".\\Blackbox.ini"
#define MAX_PROCESS_NAME 260

// Helper function to get the current process name
static bool GetCurrentProcessName(char* processName, int bufferSize)
{
    if (processName == NULL || bufferSize <= 0) {
        return false;
    }

    char fullPath[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, fullPath, MAX_PATH);
    
    if (length == 0 || length >= MAX_PATH) {
        return false;
    }

    // Extract just the filename from the full path
    char* lastSlash = strrchr(fullPath, '\\');
    const char* filename = (lastSlash != NULL) ? (lastSlash + 1) : fullPath;
    
    strncpy_s(processName, bufferSize, filename, _TRUNCATE);
    return true;
}

// Helper function to compare strings case-insensitively
static int StrCmpI(const char* str1, const char* str2)
{
    while (*str1 && *str2) {
        int diff = tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        if (diff != 0) {
            return diff;
        }
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

// Helper function to trim whitespace
static void TrimWhitespace(char* str)
{
    if (str == NULL || *str == '\0') {
        return;
    }

    // Trim leading whitespace
    char* start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // Trim trailing whitespace
    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    // Move trimmed string to beginning if needed
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

bool IsLoaderEnabled()
{
    // If config file doesn't exist, default to enabled
    DWORD fileAttrib = GetFileAttributesA(CONFIG_FILENAME);
    if (fileAttrib == INVALID_FILE_ATTRIBUTES) {
        return true;
    }

    // Read the Enabled setting from the config
    return IniReadBool(CONFIG_FILENAME, "Loader", "Enabled", true);
}

bool IsTargetProcess()
{
    // Get the current process name
    char currentProcessName[MAX_PROCESS_NAME];
    if (!GetCurrentProcessName(currentProcessName, sizeof(currentProcessName))) {
        // If we can't get the process name, default to allowing
        return true;
    }

    // If config file doesn't exist, allow all processes
    DWORD fileAttrib = GetFileAttributesA(CONFIG_FILENAME);
    if (fileAttrib == INVALID_FILE_ATTRIBUTES) {
        return true;
    }

    // Read the TargetProcess setting
    char targetProcesses[512];
    bool found = IniReadString(CONFIG_FILENAME, "Loader", "TargetProcess", "", 
                               targetProcesses, sizeof(targetProcesses));

    // If TargetProcess is not specified or empty, allow all processes
    if (!found || targetProcesses[0] == '\0') {
        return true;
    }

    // Parse comma-separated process names and check for a match
    char* context = NULL;
    char* token = strtok_s(targetProcesses, ",", &context);
    
    while (token != NULL) {
        TrimWhitespace(token);
        
        // Case-insensitive comparison
        if (StrCmpI(token, currentProcessName) == 0) {
            return true;
        }
        
        token = strtok_s(NULL, ",", &context);
    }

    // No match found
    return false;
}

bool ShouldActivateLoader()
{
    // Check if loader is enabled
    if (!IsLoaderEnabled()) {
        return false;
    }

    // Check if current process is a target
    if (!IsTargetProcess()) {
        return false;
    }

    return true;
}