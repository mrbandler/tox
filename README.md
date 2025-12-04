# Tox

A Lox implementation from [Crafting Interpreters](https://craftinginterpreters.com) with an added type checker.

**Tox = Lox + Types**

## Prerequisites

- A C++23 compatible compiler:
  - **Windows**: MSVC (Visual Studio 2022) or Clang
  - **macOS**: Clang (Xcode Command Line Tools)
  - **Linux**: GCC 13+ or Clang 16+
- [CMake](https://cmake.org/) 3.25 or higher
- [Ninja](https://ninja-build.org/) (recommended for LSP support)

## Building

### Quick Start (Recommended)

Use the Ninja-based preset for best LSP (clangd) support:

**On Windows (from Developer Command Prompt):**

```powershell
# Open "Developer Command Prompt for VS 2022" or "Developer PowerShell for VS 2022"
cmake --preset=default
cmake --build --preset=default

# Run
.\build\default\bin\tox.exe
```

**On Unix (Linux/macOS):**

```bash
cmake --preset=default
cmake --build --preset=default

# Run
./build/default/bin/tox
```

### Windows MSVC (Alternative)

If you don't want to use a Developer Command Prompt, use the Visual Studio generator:

```powershell
cmake --preset=msvc-debug
cmake --build --preset=msvc-debug

# Run
.\build\msvc-debug\bin\Debug\tox.exe
```

> **Note:** The Visual Studio generator does not produce `compile_commands.json`, so clangd/LSP features may be limited.

### Available Presets

| Preset         | Generator       | Description                                      |
|----------------|-----------------|--------------------------------------------------|
| `default`      | Ninja           | Debug build with Ninja (best for development)   |
| `release`      | Ninja           | Release build with Ninja                         |
| `msvc-debug`   | Visual Studio   | Debug build with VS generator (Windows only)    |
| `msvc-release` | Visual Studio   | Release build with VS generator (Windows only)  |

## Development

### Formatting

The project uses `clang-format` with LLVM style (4 spaces, 120 line width).

```bash
# Format a single file
clang-format -i src/main.cpp

# Format all source files (Unix)
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

# Format all source files (PowerShell)
Get-ChildItem -Recurse -Include *.cpp,*.hpp | ForEach-Object { clang-format -i $_.FullName }
```

### Project Structure

```
tox/
├── src/              # Source files (.cpp)
├── include/          # Header files (.hpp)
├── build/            # Build output (gitignored)
├── CMakeLists.txt    # CMake configuration
├── CMakePresets.json # CMake presets for easy building
├── .clang-format     # Code formatting rules
├── .clangd           # LSP configuration
└── .editorconfig     # Editor settings
```

## License

MIT License - see [LICENSE](LICENSE) for details.
