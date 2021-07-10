# Comet
Simple 2D game engine. Based on the Cherno's Game Engine YouTube series.

## Build Instructions

Comet requires the shaderc library. However, before building shaderc we need to build the libraries it depends on, namely SPIRV-Tools and glslang.

### Building SPIRV-Tools

- Recursively clone my SPIRV-Tools forked repo: [https://github.com/ej20002015/SPIRV-Tools]():
  ```bat
  git clone --recursive https://github.com/ej20002015/SPIRV-Tools.git
  ```
- 

- Clone the shaderc library [https://github.com/ej20002015/shaderc]()
- Run the following to build the library:
  - ```bat
    cd shaderc
    mkdir build
    cd build 
    cmake ../ -DSHADERC_SKIP_TESTS=ON
    ```

# WIP!!
  
