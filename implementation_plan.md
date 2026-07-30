# Ariadnis Module System

## Implemented runtime architecture

Ariadnis uses independently compiled, drop-in DLL modules. The editor discovers
files named `*.ariadnis.dll` below the `modules` directory next to its
executable. A module can be copied there without adding source files to the
editor or changing its CMake project.

The public SDK is [`include/ariadnis/module_api.h`](include/ariadnis/module_api.h).
It defines ABI version 1 as a C-compatible function table, avoiding C++ class,
allocator, STL, and ImGui binary-compatibility dependencies. Each DLL exports
`Ariadnis_GetModuleApi` and identifies itself with a stable id.

The registry validates the ABI, all required callbacks, id uniqueness, and
extension lists. It owns the DLL handle and performs ordered cleanup before
ImGui is destroyed:

```text
load DLL -> on_load -> on_enable -> feature callbacks -> on_disable -> on_unload -> FreeLibrary
```

Enablement is stored by module id in `modules_config.json` beside the editor.
The registry uses an atomic replace when saving that file.

## Supported capabilities

| Capability | Host integration |
| --- | --- |
| Export | File > Export opens the native save dialog and supplies a read-only terrain view. |
| Import | File > Import opens the native file dialog; the plug-in returns a validated mesh through a host sink. |
| Panel | Modules render through small host UI callbacks, with no linked ImGui copy. |
| Brush | Tools menu selects a module brush; the editor supplies a mutable terrain view during sculpting. |
| Renderer | View > Render Mode selects a module renderer; it draws through a limited host render context. |

Local OBJ Export and Statistics Panel modules are reference implementations.
When present, they are built as independent DLLs and copied to the executable's
`modules` directory automatically. The root `modules/` directory is ignored by
Git so local plug-ins are not included in core contributions. The unfinished WoW
example is intentionally excluded unless `ARIADNIS_BUILD_EXPERIMENTAL_MODULES=ON`
is requested.

## Packaging

CMake writes the editor and bundled DLLs to `build/bin/<configuration>/` and
copies plug-ins into `build/bin/<configuration>/modules/`. `cmake --install`
places the editor at the install root and bundled modules under `modules/`.

See [modules/README.md](modules/README.md) for plug-in authoring, lifecycle,
deployment, and security guidance.
