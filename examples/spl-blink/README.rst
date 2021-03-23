..  Copyright 2018-present PlatformIO <contact@platformio.org>
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

1. `Install PlatformIO Core <http://docs.platformio.org/page/core.html>`_
2. Download `development platform with examples <https://github.com/platformio/platform-ststm8/archive/develop.zip>`_
3. Extract ZIP archive
4. Run these commands:

.. code-block:: bash

    # Change directory to example
    > cd platform-ststm8/examples/spl-blink

    # Build project
    > platformio run

    # Upload firmware
    > platformio run --target upload

    # Build specific environment
    > platformio run -e stm8sdisco

    # Upload firmware for the specific environment
    > platformio run -e stm8sdisco --target upload

    # Clean build files
    > platformio run --target clean

Notes regarding SPL setup
=========================

Please see the `src/stm8s_conf.h` file for activating more SPL modules, if you wish to expand the functionality of this example. Only modules (like ADC, UART, etc.) that are activated in the configuration file are compiled in. In this example, only the GPIO module is active. Activating unused modules will result in a higher flash usage that will make even compilation even impossible for smaller chips, to care must be taken.
