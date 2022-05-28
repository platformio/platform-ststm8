# ST STM8: development platform for [PlatformIO](https://platformio.org)

[![Build Status](https://github.com/platformio/platform-ststm8/workflows/Examples/badge.svg)](https://github.com/platformio/platform-ststm8/actions)

The STM8 is an 8-bit microcontroller family by STMicroelectronics an extended variant of the ST7 microcontroller architecture. STM8 microcontrollers are particularly low cost for a full-featured 8-bit microcontroller.

* [Home](https://registry.platformio.org/platforms/platformio/ststm8) (home page in the PlatformIO Registry)
* [Documentation](https://docs.platformio.org/page/platforms/ststm8.html) (advanced usage, packages, boards, frameworks, etc.)

# Usage

1. [Install PlatformIO](https://platformio.org)
2. Create PlatformIO project and configure a platform option in [platformio.ini](https://docs.platformio.org/page/projectconf.html) file:

## Stable version

```ini
[env:stable]
platform = ststm8
board = ...
...
```

## Development version

```ini
[env:development]
platform = https://github.com/platformio/platform-ststm8.git
board = ...
...
```

# Configuration

Please navigate to [documentation](https://docs.platformio.org/page/platforms/ststm8.html).
