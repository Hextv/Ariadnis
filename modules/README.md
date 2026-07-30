# Ariadnis drop-in modules

Ariadnis discovers compatible modules when it starts. To install one, copy its
`*.ariadnis.dll` file into the `modules` directory beside `Ariadnis.exe`:

```text
bin/Debug/
  Ariadnis.exe
  modules/
    my_exporter.ariadnis.dll
```

Subfolders are supported, so a module can be distributed as a folder containing
its plug-in DLL and private dependency DLLs. Only files ending in
`.ariadnis.dll` are loaded. Restart the editor after installing, removing, or
updating a module: Windows keeps a loaded DLL locked until the editor exits.

The root `modules/` directory is intentionally gitignored except for this
guide. Keep a module in its own repository or package it separately; this
prevents local plug-ins from being included in Ariadnis core contributions.

## SDK contract

The entire public SDK is [`include/ariadnis/module_api.h`](../include/ariadnis/module_api.h).
It is a versioned C ABI, not a C++ base class. A module must:

- export `Ariadnis_GetModuleApi`;
- return a static `AriadnisModuleApi` whose `abi_version` and `struct_size`
  exactly match the installed editor SDK;
- use a globally unique ASCII id such as `com.example.my-exporter`;
- never throw exceptions through an API callback;
- keep metadata and extension strings alive for as long as the DLL is loaded.

Modules receive POD terrain views and callback tables. They must not include
`src/` headers, link against Ariadnis, access the editor's ImGui context, or
free editor-owned memory. Importers pass their completed mesh back to the host
through `AriadnisTerrainSink`, which copies it immediately.

The host validates the ABI, metadata, capability callbacks, duplicate ids, and
file extension lists before activating a module. It calls lifecycle callbacks
in this order:

```text
on_load -> on_enable -> ... -> on_disable -> on_unload
```

Disabled modules do not receive feature calls. Enabling/disabling from the
**Modules** menu performs the corresponding lifecycle transition immediately;
the setting is stored by id next to the executable in `modules_config.json`.

## Creating a module

This minimal standalone CMake project has no dependency on the editor source:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyModule LANGUAGES CXX)

add_library(my_module SHARED my_module.cpp)
target_compile_features(my_module PRIVATE cxx_std_17)
target_include_directories(my_module PRIVATE "C:/AriadnisSDK/include")
set_target_properties(my_module PROPERTIES OUTPUT_NAME "my_module.ariadnis")
```

Point `C:/AriadnisSDK/include` at a copy of Ariadnis's `include` directory (or
the matching installed SDK). Build the DLL, copy it into the editor's `modules`
folder, and launch Ariadnis. The local OBJ exporter and Statistics panel are
reference implementations when present; they are intentionally not part of the
core repository.

The local WoW exporter is deliberately excluded unless
`-DARIADNIS_BUILD_EXPERIMENTAL_MODULES=ON` is set. It demonstrates the ABI only;
it does not write ADT/WDT files and should not be distributed as a real exporter.

## Capability mapping

| Module type | Required callbacks | Editor integration |
| --- | --- | --- |
| Exporter | extensions, `export_terrain` | File > Export opens a host save dialog |
| Importer | extensions, `import_terrain` | File > Import opens a host file dialog |
| Panel | `render_panel` | Host UI callbacks during each ImGui frame |
| Brush | name, `apply_brush` | Tools menu and sculpting loop |
| Renderer | name, `render_terrain` | View > Render Mode |

Modules are native code. Install only modules from sources you trust.
