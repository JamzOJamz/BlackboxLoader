# Blackbox.ini Configuration Testing Guide

This document describes how to test the Blackbox.ini configuration functionality.

## Manual Testing Procedure

### Test 1: No Configuration File
**Setup**: Remove or rename `Blackbox.ini`
**Expected**: Loader should activate and load all plugins from `Blackbox/` directory
**Verification**: Plugins should load regardless of the current process name

### Test 2: Enabled=true, No TargetProcess
**Configuration**:
```ini
[Loader]
Enabled=true
```
**Expected**: Loader should activate for all processes
**Verification**: Plugins should load in any process

### Test 3: Enabled=false
**Configuration**:
```ini
[Loader]
Enabled=false
```
**Expected**: Loader should not activate
**Verification**: No plugins should be loaded

### Test 4: TargetProcess with Match
**Configuration**:
```ini
[Loader]
Enabled=true
TargetProcess=notepad.exe,calc.exe
```
**Expected**: When running in notepad.exe or calc.exe, plugins should load
**Verification**: 
- Place version.dll in notepad.exe directory, run notepad.exe → plugins load
- Place version.dll in calc.exe directory, run calc.exe → plugins load
- Place version.dll in other process directory → plugins don't load

### Test 5: TargetProcess without Match
**Configuration**:
```ini
[Loader]
Enabled=true
TargetProcess=game.exe
```
**Expected**: When running in processes other than game.exe, plugins should NOT load
**Verification**: Place version.dll in notepad.exe directory, run notepad.exe → plugins don't load

### Test 6: Case Insensitive Process Names
**Configuration**:
```ini
[Loader]
Enabled=true
TargetProcess=NOTEPAD.EXE,Calc.Exe
```
**Expected**: Should match notepad.exe and calc.exe regardless of case
**Verification**: Process name comparison should be case-insensitive

### Test 7: Multiple Target Processes with Whitespace
**Configuration**:
```ini
[Loader]
Enabled=true
TargetProcess= notepad.exe , calc.exe , game.exe 
```
**Expected**: Whitespace should be trimmed, all three processes should be valid targets
**Verification**: All three processes should load plugins

### Test 8: Boolean Value Variations
**Configuration variations**:
```ini
Enabled=true   # Should work
Enabled=TRUE   # Should work (case insensitive)
Enabled=yes    # Should work
Enabled=1      # Should work
Enabled=on     # Should work
Enabled=false  # Should disable
Enabled=0      # Should disable
```
**Expected**: All boolean representations should be recognized
**Verification**: Test each variation to ensure proper parsing

### Test 9: Comments and Empty Lines
**Configuration**:
```ini
[Loader]
# This is a comment
Enabled=true

; This is also a comment
TargetProcess=notepad.exe
```
**Expected**: Comments and empty lines should be ignored
**Verification**: Configuration should parse correctly

## Testing on Windows

To test this implementation on Windows:

1. Build the project:
   ```bash
   mkdir build
   cd build
   cmake .. -DPROXY_DLL=VERSION
   cmake --build .
   ```

2. Create a test directory structure:
   ```
   test_dir/
   ├── notepad.exe (copy from System32)
   ├── version.dll (built BlackboxLoader)
   ├── Blackbox.ini (test configuration)
   └── Blackbox/
       └── test_plugin.dll (simple test plugin)
   ```

3. Create a simple test plugin that writes to a log file when loaded

4. Run notepad.exe from test_dir and verify plugin loading behavior based on Blackbox.ini settings

## Automated Testing (Future Enhancement)

Consider implementing:
- Unit tests for INI parsing functions
- Integration tests that mock Windows API calls
- Test fixtures for various configuration scenarios
