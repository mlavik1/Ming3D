# Ming3D
Game engine.

This is a playground for me to learn different things.

# How to clone and build
- Download CMake
- Clone (recursively, since it contains modules)
- Run DownloadLibs.py (or DownloadLibs.bat if on Windows)
- Open CMake
- Set source code directory ("Where is the source code: " in CMake GUI) to the repository root directory
- Configure and Generate
- For the test project, modify the "TestType" variable to select the type of test you want to use.

# Renderer

Has support for:
- Rendering with OpenGL or DirectX11
- Cross platform shaders, that are automatically converted to GLSL or HLSL (still missing some features)
- Abstractions for: Textures, Vertex Buffers, Shaders, Uniforms / Constant Buffers
