# BlackboxLoader (Blackbox Loader)

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
- No configuration files or runtime dependencies
- Pure C implementation with minimal footprint
- Early-process execution for low-level access

## Build Configuration

Select the proxied DLL at compile time:

```cmake
cmake -DPROXY_DLL=VERSION ..  # builds version.dll (default)
cmake -DPROXY_DLL=WINHTTP ..  # builds winhttp.dll
```

Place the output DLL next to the target executable.

## Plugins

- Plugins must be unmanaged DLLs
- All DLLs in the `Blackbox/` directory are loaded automatically
- Load order is filesystem-dependent
- No plugin API is imposed; plugins execute their own `DllMain`

## Directory Layout

```
<application root>/
├── version.dll      # BlackboxLoader
└── Blackbox/
    ├── plugin1.dll
    └── plugin2.dll
```

## License

MIT
