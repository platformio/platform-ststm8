..  Copyright 2021-present PlatformIO <contact@platformio.org>
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-ststm8/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-ststm8/examples/native-blink

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e stm8sblue

# Upload firmware for the specific environment
$ pio run -e stm8sblue --target upload

# Clean build files
$ pio run --target clean
```

Project description
===================

This is a baremetal project targeting three STM8S example chips / boards :
* STM8S103F3 breakout board
* Nucleo-8S207K8
* Nucleo-8S208RB

The pinmapping is such that the built-in LED of those boards is automatically used.

The project does not any framework like Arduino or SPL for compilation, hence no `framework = ..` line in the `platformio.ini`. Only one `.c` file and the right `.h` device header file is used.

The project uses the FOSS header files from https://github.com/gicking/STM8_headers, which are placed under the MIT license.

If you wish to adapt this example for more chips and boards, add a new environment for your chip, find the appropriate header file from the referenced repository and include that header.
