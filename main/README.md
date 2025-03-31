# Project Source Code

`main.c` is the main project file and is what is typically built and flashed to the ESP32.

Other .c files in this folder are standalone demonstration files for various components considered for/incorporated in the project. If you want to build one of these instead of the main file, you can modify the `CMakeLists.txt` file in this directory. For example, if you wanted to build `sht3x_demo.c`, you can change the `SRCS` parameter given to `idf_component_register` in the CMake file from `main.c` to `sht3x_demo.c`.

**Note!** I tried a few different parts before I settled on the current design, so some of the demo files here are not applicable to the most recent designs. The following is a list of components that are no longer being used. If you would like to try any of these out, just be sure to add the corresponding component to your project (see root README for more info on this). Doing so will download the necessary files into your managed_components directory.

* [SHT3x temp/humidity sensor](https://components.espressif.com/components/espressif/sht3x/versions/0.2.0)
* [BMP280 pressure sensor](https://components.espressif.com/components/maucke/bmp280/versions/1.0.1)
  * Special note on this one: I had to make several changes to this component library to make it work. Please see my previous commit [here](https://github.com/hildebkg/FarrhadTech/commit/3e463adc17b4526ff7c5bdaee2d7144cd09fb9ec) with the changes if needed.
