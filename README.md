# BlackboxLoader (Blackbox Loader)

**BlackboxLoader** is a minimal Windows DLL loader based on classic **DLL proxying / hijacking**.  
It replaces a legitimate system DLL (`version.dll`), and loads unmanaged plugin DLLs from a `Blackbox/` directory.

It can be used for **game modding**, following a similar concept to **MelonLoader** or **BepInEx**, but operating entirely with **unmanaged DLLs** for lower-level control inside the target process.

## How It Works

1. A target application loads a common system DLL
2. BlackboxLoader is placed alongside the executable under the same DLL name
3. Required exports are forwarded to the real system DLL
4. All DLLs in `Blackbox/` are loaded automatically

## Layout

```
<root directory>/
├── version.dll      # BlackboxLoader
└── Blackbox/
    ├── plugin1.dll
    └── plugin2.dll
```

## Notes

- No configuration or dependencies
- Plugins are unmanaged DLLs
- Load order is filesystem-dependent

## License

MIT
