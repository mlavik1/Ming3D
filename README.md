# Ming3D
Game engine, for Windows and Linux.

Supports rendering with OpenGL and DirectX11.

![alt text](GitHubMedia/Ming3D.jpg)

# How to clone and build

__WINDOWS__
- Requirements: Visual studio 2017 or newer. C++17 or newer. CMake 3.3 or newer (ideally 3.10).
- Clone (recursively, since it contains modules): git clone --recursive https://github.com/mlavik1/Ming3D
- If you have Python 2: Run DownloadLibs.py (or DownloadLibs.bat if on Windows)
- (Alternatively: Download these files and put them in a folder called "ExternalLibraries": https://drive.google.com/file/d/1p4GA1lcoKZZcPsGSPOZ6ZWb85yyvmm2b/view?usp=sharing)
- Open CMake, and set source code directory ("Where is the source code: " in CMake GUI) to the repository root directory
- Configure and Generate
- Optionally: For the sample projects, modify the "SampleProject" variable to select the type of sample you want to use.

__LINUX__
- Clone the repository (recursively!): git clone --recursive https://github.com/mlavik1/Ming3D
- Run aptget.sh (or manually do apt-get on the dependencies). Note: if you get "permission denied", execute "chmod +x aptget.sh" in the terminal first.
- Run CMake: cmake .
- Build a project (such as Runtime): make Runtime
- Run the executables (in the "build" folder). Make sure the "Resources" folder is in the same as the executable (just copy it over).

(NOTE: You need at least OpenGL 4.0)

# Renderer

Has support for:
- Rendering with OpenGL or DirectX11
- Cross platform shaders, that are automatically converted to GLSL or HLSL (still missing some features)
- Abstractions for: Textures, Vertex Buffers, Shaders, Uniforms / Constant Buffers
