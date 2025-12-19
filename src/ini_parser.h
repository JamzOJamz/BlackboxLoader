#pragma once

#include <stdbool.h>

/**
 * Simple INI parser for Blackbox configuration
 * Supports basic INI format with [Section] and Key=Value pairs
 */

/**
 * Read a string value from an INI file
 * 
 * @param filename Path to the INI file
 * @param section Section name (e.g., "Loader")
 * @param key Key name (e.g., "Enabled")
 * @param defaultValue Default value if key not found
 * @param buffer Output buffer for the value
 * @param bufferSize Size of the output buffer
 * @return true if value was found, false otherwise
 */
bool IniReadString(const char* filename, const char* section, const char* key, 
                   const char* defaultValue, char* buffer, int bufferSize);

/**
 * Read a boolean value from an INI file
 * Accepts: true/false, yes/no, 1/0 (case insensitive)
 * 
 * @param filename Path to the INI file
 * @param section Section name
 * @param key Key name
 * @param defaultValue Default value if key not found
 * @return Boolean value
 */
bool IniReadBool(const char* filename, const char* section, const char* key, bool defaultValue);
