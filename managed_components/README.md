# Managed Components

"Components" within the ESP-IDF build system can be thought of as libraries - they are standalone collections of code that add additional functionality to a project. These are often produced by Espressif but may be developed elsewhere. As mentioned in the main project README, these components are collected within the [IDF Component Registry](https://components.espressif.com/). The IDF Component Manager tool handles dependencies for you when CMake runs. More information can be found [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-component-manager.html). If you were to clone this repo and delete this folder, then build the project, the IDF Component Manager would download all of these components for you according to the `dependencies.lock` file. I have made some of my own changes to some of these files, which is why I have explicitly included them in this repo.

Here is the list of components which I have modified:

* [/maucke__bmp280](maucke__bmp280)

All other components have been utilized without change.
