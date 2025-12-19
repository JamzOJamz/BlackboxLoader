#include "ini_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

// Helper function to trim whitespace from both ends of a string
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

    // If string is all whitespace
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

bool IniReadString(const char* filename, const char* section, const char* key,
                   const char* defaultValue, char* buffer, int bufferSize)
{
    if (filename == NULL || section == NULL || key == NULL || buffer == NULL || bufferSize <= 0) {
        return false;
    }

    // Set default value initially
    if (defaultValue != NULL) {
        strncpy_s(buffer, bufferSize, defaultValue, _TRUNCATE);
    } else {
        buffer[0] = '\0';
    }

    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == NULL) {
        return false;
    }

    char line[512];
    char currentSection[128] = "";
    bool inTargetSection = false;
    bool found = false;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            if (len > 1 && line[len - 2] == '\r') {
                line[len - 2] = '\0';
            }
        }

        TrimWhitespace(line);

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == ';' || line[0] == '#') {
            continue;
        }

        // Check for section header
        if (line[0] == '[') {
            char* end = strchr(line, ']');
            if (end != NULL) {
                *end = '\0';
                strncpy_s(currentSection, sizeof(currentSection), line + 1, _TRUNCATE);
                TrimWhitespace(currentSection);
                inTargetSection = (StrCmpI(currentSection, section) == 0);
            }
            continue;
        }

        // If we're in the target section, look for the key
        if (inTargetSection) {
            char* equals = strchr(line, '=');
            if (equals != NULL) {
                *equals = '\0';
                char* lineKey = line;
                char* value = equals + 1;

                TrimWhitespace(lineKey);
                TrimWhitespace(value);

                if (StrCmpI(lineKey, key) == 0) {
                    strncpy_s(buffer, bufferSize, value, _TRUNCATE);
                    found = true;
                    break;
                }
            }
        }
    }

    fclose(file);
    return found;
}

bool IniReadBool(const char* filename, const char* section, const char* key, bool defaultValue)
{
    char buffer[32];
    bool found = IniReadString(filename, section, key, defaultValue ? "true" : "false", buffer, sizeof(buffer));
    
    if (!found) {
        return defaultValue;
    }

    TrimWhitespace(buffer);

    // Check for various true values (case insensitive)
    if (StrCmpI(buffer, "true") == 0 || 
        StrCmpI(buffer, "yes") == 0 || 
        StrCmpI(buffer, "1") == 0 ||
        StrCmpI(buffer, "on") == 0) {
        return true;
    }

    // Check for various false values (case insensitive)
    if (StrCmpI(buffer, "false") == 0 || 
        StrCmpI(buffer, "no") == 0 || 
        StrCmpI(buffer, "0") == 0 ||
        StrCmpI(buffer, "off") == 0) {
        return false;
    }

    // If value is unrecognized, return default
    return defaultValue;
}