# Copyright 2018-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
SPL

Library that enables developers to easily exploit all the functions of the STM8
microcontrollers to address a wide range of applications.

https://www.st.com/en/embedded-software/stsw-stm8069.html
"""

import sys
from os.path import basename, isdir, join

from SCons.Script import DefaultEnvironment

from platformio.util import exec_command

env = DefaultEnvironment()
platform = env.PioPlatform()
board_config = env.BoardConfig()

FRAMEWORK_DIR = platform.get_package_dir("framework-ststm8spl")
assert isdir(FRAMEWORK_DIR)


def get_core_files():
    command = [
        env.subst("$CC"), "-m%s" % board_config.get("build.cpu"),
        "-D%s" % board_config.get("build.mcu")[0:8].upper(),
        "-I.", "-I", "%s" % env.subst("$PROJECTSRC_DIR"),
        "-Wp-MM", "-E", "stm8s.h"
    ]

    result = exec_command(
        command,
        cwd=join(FRAMEWORK_DIR, "Libraries", "STM8S_StdPeriph_Driver", "inc")
    )

    if result['returncode'] != 0:
        sys.stderr.write(
            "Error: Could not parse library files for the target.\n")
        sys.stderr.write(result['err'])
        env.Exit(1)

    src_files = []
    includes = result['out']
    for inc in includes.split(" "):
        if "_" not in inc or ".h" not in inc or "conf" in inc:
            continue
        src_files.append(basename(inc).replace(".h", ".c").strip())

    return src_files

env.Append(
    CFLAGS=["--opt-code-size"],

    CPPDEFINES=[
        "USE_STDPERIPH_DRIVER",
        "USE_STDINT"
    ],

    CPPPATH=[
        join(FRAMEWORK_DIR, "Libraries", "STM8S_StdPeriph_Driver", "inc"),
        "$PROJECTSRC_DIR",
    ]
)


#
# Target: Build Core Library
#

env.BuildSources(
    join("$BUILD_DIR", "SPL"),
    join(FRAMEWORK_DIR, "Libraries", "STM8S_StdPeriph_Driver", "src"),
    src_filter=["-<*>"] + [" +<%s>" % f for f in get_core_files()]
)
