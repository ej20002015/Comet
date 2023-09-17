# Comet
A simple 2D game engine based on the Cherno's Game Engine YouTube series.

Currently Comet only supports Windows.

[Demo Video](https://drive.google.com/file/d/18dbum_bTOtStv8dWs7WkQj6i0_jE5GgV/view?usp=sharing)

## Tools required to build

- Visual Studio 2015 or newer
- CMake
- git
- python3

## Build Instructions

Comet uses Premake5 to generate the project files which can then be used to build. The only target toolchain that has been validated so far is Visual Studio. Perform the following steps to build Comet:

1. Clone the Comet repository recursively:
   ``` bat
   git clone --recursive https://github.com/ej20002015/Comet.git
   cd Comet
   ```
2. The first thing to do is to generate Visual Studio project files for Comet using Premake5:
    1. run the Scripts/GenerateWindowsFiles.bat batch script - this script calls Premake:
    ```bat
    call scripts/GenerateWindowsFiles.bat
    ```
    2. In the root directory you will now see a Comet.sln Visual Studio solution file
3. Before opening the Visual Studio solution and building Comet, a couple of libraries need to be present. Build these libraries in a separate folder to the Comet directory. To build the shaderc library follow these steps:
    1. Clone my fork of the shaderc repository recursively; it contains a load of submodules that are shaderc dependencies:
    ```bat
    git clone --recursive https://github.com/ej20002015/shaderc.git
    ```
    2. Run these commands to create a build folder and generate Visual Studio project files:
    ``` bat
    cd shaderc
    mkdir build
    cd build
    cmake .. -DSHADERC_SKIP_TESTS=ON
    ```
    3. Now open the Visual Studio .sln file that has been generated and build the "shaderc" project in both Release and Debug configurations.
    4. Several .lib files will be built. Some of these need to be copied into the appropriate folder in the Comet project. The Debug versions of the libraries need to be copied into the "[Comet/Comet/Vendor/shaderc/bin/Debug](Comet/Vendor/shaderc/bin/Debug)" directory, and the Release versions need to be copied into the "[Comet/Comet/Vendor/shaderc/bin/Release](Comet/Vendor/shaderc/bin/Release)" directory. Consult the README.md files in each of these directories for instuctions on which .lib files need to be present and where they can be copied from in the shaderc folder structure.
4. Now shaderc library files have been placed in the correct place, we need to build the other library Comet is dependent on, SPIRV-Cross.
    1. Clone my fork of the SPIRV-Cross repository:
    ```bat
    git clone https://github.com/ej20002015/SPIRV-Cross.git
    ```
    2. Run these commands to create a build folder and generate Visual Studio project files:
    ``` bat
    cd SPIRV-Cross
    mkdir build
    cd build
    cmake ..
    ```
    3. Now open the Visual Studio .sln file that has been generated. Before building the projects a couple of configuration changes need to be made so the compiled .lib files can be used in Comet. For the following projects, modify the runtime library for Debug configurations to be "Multi-threaded Debug (/MTd)", and for Release configurations to be "Multi-threaded (/MT)". Instructions on how to modify the runtime library of Visual Studio projects can be found [here](https://stackoverflow.com/questions/4307429/visual-studio-how-to-specify-different-runtime-libraries-for-the-linker-mtd):
        - spirv-cross-c
        - spirv-cross-core
        - spirv-cross-cpp
        - spirv-cross-glsl
        - spirv-cross-hlsl
        - spirv-cross-msl
        - spirv-cross-reflect
        - spirv-cross-util
    4. Build the "ALL_BUILD" project in both Release and Debug configurations
    5. Several .lib files will be built. All of these need to be copied into the appropriate folder in the Comet project. The Debug versions of the libraries need to be copied into the "[Comet/Comet/Vendor/SPIRV-Cross/bin/Debug](Comet/Vendor/SPIRV-Cross/bin/Debug)" directory, and the Release versions need to be copied into the "[Comet/Comet/Vendor/SPIRV-Cross/bin/Release](Comet/Vendor/SPIRV-Cross/bin/Release)" directory. Consult the README.md files in each of these directories for instuctions on which .lib files need to be present and where they can be copied from in the SPIRV-Cross folder structure.
5. Now the Comet solution can be opened and built.
