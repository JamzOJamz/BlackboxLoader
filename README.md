# BlackboxLoader

**BlackboxLoader** is a minimal Windows DLL loader based on classic **DLL proxying / hijacking**.  
It replaces a legitimate system DLL and loads unmanaged plugin DLLs from a `Blackbox/` directory.

It can be used for **game modding**, following a similar concept to **MelonLoader** or **BepInEx**, but operating entirely with **unmanaged DLLs** for lower-level control inside the target process.

## How It Works

1. Target application attempts to load a common system DLL
2. BlackboxLoader (renamed to match) is loaded instead
3. Required exports are forwarded to the real system DLL
4. All DLLs in `Blackbox/` are loaded automatically on process attach

## Features

- DLL proxying via `version.dll` or `winhttp.dll` (build-time selectable)
- Automatic plugin loading from `Blackbox/` directory
- Configuration file support via `Blackbox.ini`
- Conditional loading based on process name and enabled state
- Pure C implementation with minimal footprint
- Early-process execution for low-level access

## Build Configuration

Select the proxied DLL at compile time:

```bash
cmake -DPROXY_DLL=VERSION ..  # builds version.dll (default)
cmake -DPROXY_DLL=WINHTTP ..  # builds winhttp.dll
```

Place the output DLL next to the target executable.

## Configuration

BlackboxLoader supports optional configuration via a `Blackbox.ini` file placed next to the loader DLL.

### Blackbox.ini Format

```ini
[Loader]
Enabled=true
TargetProcess=notepad.exe,calc.exe
```

### Configuration Options

#### `Enabled`
- **Type**: Boolean (`true`/`false`, `yes`/`no`, `1`/`0`)
- **Default**: `true`
- **Description**: Controls whether the loader is active. Set to `false` to disable plugin loading.

#### `TargetProcess`
- **Type**: Comma-separated list of process names
- **Default**: All processes (if not specified)
- **Description**: Restricts loader activation to specific processes. If specified, plugins will only load when the current process name matches one of the listed names. Process names are case-insensitive.

### Configuration Examples

**Load plugins for all processes:**
```ini
[Loader]
Enabled=true
```

**Load plugins only for specific processes:**
```ini
[Loader]
Enabled=true
TargetProcess=game.exe,launcher.exe
```

**Disable the loader entirely:**
```ini
[Loader]
Enabled=false
```

**No configuration file:**
If `Blackbox.ini` is not present, the loader behaves as if `Enabled=true` with no process restrictions.

## Plugins

- Plugins must be unmanaged DLLs
- All DLLs in the `Blackbox/` directory are loaded automatically
- Load order is filesystem-dependent
- No plugin API is imposed; plugins execute their own `DllMain`

## Example Directory Layout

```
<application root>/
├── version.dll      # BlackboxLoader
├── Blackbox.ini     # Optional configuration file
└── Blackbox/
    ├── plugin1.dll
    └── plugin2.dll
```

## License

MIT
