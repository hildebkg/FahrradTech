# FarrhadTech

Senior design project files for FarrhadTech: an affordable, feature-rich bike computer with integrated safety accessories.

## Navigation

* [/main](/main): project source code
* [/references](/references/): datasheets, etc
* [/schematics](/schematics/): hardware design files
* [/sim](/sim): hardware simulation files

## Getting Started

This project utilizes the official Espressif Systems IoT Development Framework (ESP-IDF). You can view Espressif's official getting started docs [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html). I did a manual installation on Windows 10 (more info on that specifically [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html)).

* Download the proper installer for your machine (I used “Universal Online Installer 2.3.4” for Windows 10) and run it.
  * I mostly ran with default settings, but I did check the box to install the IDE in case I ever wanted to use it.
* After installation is complete, open a command prompt and run the following commands:
  * `cd C:\Espressif\frameworks\esp-idf-v5.4` (or whatever your install directory is)
  * `install.bat`
    * This installs the ESP-IDF Python virtual environment.
  * `export.bat`
    * This adds ESP-IDF Tools to PATH (not required).
* Keep things simple and get a demo project from [here](https://github.com/espressif/esp-idf/tree/v5.2.5/examples/get-started) or from your install directory (in \examples\get-started). Copy the entire folder to your desired workspace.
* Download the required serial drivers for your board.
  * My board contains a CP2102 USB to UART bridge, but others may differ. If yours has a CP2102, you can download the drivers from Silicon Labs [here](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads).
* Connect your ESP32 device to your machine and take note of the port to which it is connected. You will need this for the next step. On Windows, this can be checked via the Device Manager.
* To test your chosen example project, fire up either ESP-IDF Cmd or ESP-IDF PowerShell and run the following commands:
  * `cd <dir_containing_example_project>`
  * `idf.py add-dependency esp32`
    * This adds the Board Support Package (BSP) for ESP32 boards. If you have something specific, you can look for it in the [IDF Component Registry](https://components.espressif.com/).
  * `idf.py build`
    * This builds the project. This will take a bit the first time!
  * `idf.py – p COM# flash`
    * This flashes your device with the most recent build.
    * Make sure to replace "COM#" with the port your device is currently using.
  * `idf.py -p COM# monitor`
    * This relays serial data to and from your target device's serial port. This allows you to see output from your board after it has been flashed.
    * More information [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-monitor.html).
* To add extra components to your project (from within ESP-IDF CMD or PowerShell):
  * `cd <dir_containing_example_project>`
  * `idf.py add-dependency "component_name"`
    * You should replace "component_name" with the name contained in the [IDF Component Registry](https://components.espressif.com/).
  * `idf.py reconfigure`
    * Processes the `idf_component.yml` file in your `main` folder for each component in your project and resolves dependencies.
    * Creates/updates the `dependencies.lock` file in your  project directory with the full dependency list.
    * Downloads all newly-added depedencies to the `managed_components` folder in your project directory.
