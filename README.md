# BlackboxLoader

**BlackboxLoader** is a minimal Windows DLL loader based on classic **DLL proxying / hijacking**.  
It replaces a legitimate system DLL and loads unmanaged plugin DLLs from a `Blackbox/` directory.

It can be used for **game modding**, following a similar concept to **MelonLoader** or **BepInEx**, but operating entirely with **unmanaged DLLs** for lower-level control inside the target process.

## Features

- **Modular Architecture**: Refactored codebase with separate modules for plugin loading, proxy exports, and configuration
- **Multiple DLL Proxy Support**: Can be built to proxy either `version.dll` or `winhttp.dll`
- **Automatic Plugin Loading**: All DLLs in the `Blackbox/` directory are loaded automatically
- **No Runtime Dependencies**: No configuration files or external dependencies required

## How It Works

1. A target application loads a common system DLL
2. BlackboxLoader is placed alongside the executable under the same DLL name
3. Required exports are forwarded to the real system DLL
4. All DLLs in `Blackbox/` are loaded automatically

## Building

The DLL to proxy is selected at compile time using CMake options.

### Build as version.dll proxy (default)
```bash
cmake -B build
cmake --build build
```

### Build as winhttp.dll proxy
```bash
cmake -B build -DPROXY_DLL=WINHTTP
cmake --build build
```

## Layout

### When built as version.dll (default)
```
<root directory>/
├── version.dll      # BlackboxLoader
└── Blackbox/
    ├── plugin1.dll
    └── plugin2.dll
```

### When built as winhttp.dll
```
<root directory>/
├── winhttp.dll      # BlackboxLoader
└── Blackbox/
    ├── plugin1.dll
    └── plugin2.dll
```

## Code Structure

The codebase is organized into modular components:

- **`src/dllmain.c`**: Main DLL entry point
- **`src/config.h`**: Compile-time configuration for DLL proxy selection
- **`src/plugin_loader.c/h`**: Plugin loading and management
- **`src/proxy_exports.c/h`**: DLL export proxying implementation
- **`version.def`**: Export definition file for version.dll
- **`winhttp.def`**: Export definition file for winhttp.dll

## Notes

- No configuration files or runtime dependencies
- Plugins are unmanaged DLLs
- Load order is filesystem-dependent
- The DLL to proxy is determined at compile time

## License

MIT

