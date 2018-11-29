TODO
- Câmera WASD
- Câmera mouse
- Luz Sol
- Luz Teto da sala
- Luz Observador
- Textura
- Objeto transparente
- Travar redimensionamento

### Tech
* [gcc]
* [MSYS2]
* [CMake]
* [Ninja]

### Installation (Windows)
* Install MSYS2
* No shell do MYSYS2 MinGW 64-Bit, digite:

    ```sh
    pacman -Syu
    pacman -Su
    pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gcc
    ```

### Compilation
```sh
cd <project_directory>
mkdir build
cd build
cmake .. -G Ninja
ninja
```

### Exec

```sh
cd <project_directory>
./script.sh
```

[MSYS2]: <http://www.msys2.org>
[Ninja]: <https://ninja-build.org>
[CMake]: <https://cmake.org>
[gcc]: <https://gcc.gnu.org>
