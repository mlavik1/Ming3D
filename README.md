# Ming3D
Game engine.

This is a playground for me to learn different things.

# How to clone and build
- Download and install DirectX SDK: https://www.microsoft.com/en-us/download/confirmation.aspx?id=6812
- Download CMake
- Clone (recursively, since it contains modules)
- If you have Python 2: Run DownloadLibs.py (or DownloadLibs.bat if on Windows)
- (Alternatively: Download these files and put them in a folder called "ExternalLibraries": https://drive.google.com/file/d/1p4GA1lcoKZZcPsGSPOZ6ZWb85yyvmm2b/view?usp=sharing)
- Open CMake
- Set source code directory ("Where is the source code: " in CMake GUI) to the repository root directory
- Configure and Generate
- For the test project, modify the "TestType" variable to select the type of test you want to use.

# Renderer

Has support for:
- Rendering with OpenGL or DirectX11
- Cross platform shaders, that are automatically converted to GLSL or HLSL (still missing some features)
- Abstractions for: Textures, Vertex Buffers, Shaders, Uniforms / Constant Buffers
