#pragma once

#include <stdbool.h>

/**
 * Configuration reader for Blackbox.ini
 * Manages loader configuration including enabled state and target processes
 */

/**
 * Check if the loader should be enabled based on Blackbox.ini configuration
 * 
 * @return true if loader should be enabled, false otherwise
 */
bool IsLoaderEnabled();

/**
 * Check if the current process is a target process based on Blackbox.ini configuration
 * 
 * @return true if current process matches a target process or no target is specified,
 *         false if targets are specified but current process doesn't match
 */
bool IsTargetProcess();

/**
 * Check if the loader should activate based on all configuration rules
 * This combines the Enabled flag and TargetProcess checks
 * 
 * @return true if loader should activate, false otherwise
 */
bool ShouldActivateLoader();
