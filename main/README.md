# Project Source Code

`main.c` is the main project file and is what is actually built and flashed to the ESP32.

Other .c files are standalone demonstration files for various components incorporated in the project. If you want to build one of these instead of the main file, you can modify the `CMakeLists.txt` file in this directory. As an example, if you wanted to build `sht3x_demo.c`, you can change the first line in the CMake file to `idf_component_register(SRCS "sht3x_demo.c"`.
