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

import sys
from os.path import join
from platform import system

from SCons.Script import (AlwaysBuild, COMMAND_LINE_TARGETS, Default,
                          DefaultEnvironment)


env = DefaultEnvironment()
board_config = env.BoardConfig()

env.Replace(
    AR="sdar",
    AS="sdasstm8",
    CC="sdcc",
    GDB="stm8-gdb",
    LD="sdldstm8",
    RANLIB="sdranlib",
    OBJCOPY="stm8-objcopy",
    SIZETOOL="stm8-size",
    OBJSUFFIX=".rel",
    LIBSUFFIX=".lib",

    ARFLAGS=["rcs"],

    ASFLAGS=["-x", "assembler-with-cpp", "-flto"],

    CFLAGS=[
        "-m%s" % board_config.get("build.cpu")
    ],

    CPPDEFINES=[
        "F_CPU=$BOARD_F_CPU"
    ],

    LINKFLAGS=[
        "-m%s" % board_config.get("build.cpu"),
        "--nostdlib",
        "--code-size", board_config.get("upload.maximum_size"),
        "--iram-size", board_config.get("upload.maximum_ram_size"),
        "--out-fmt-elf"
    ],

    LIBPATH=[
        join(env.PioPlatform().get_package_dir("toolchain-sdcc"),
             "%s" % "lib" if system() == "Windows" else join("share", "sdcc", "lib"),
             board_config.get("build.cpu"))
    ],

    LIBS=["stm8"],

    SIZEPROGREGEXP=r"^(?:HOME|GSINIT|GSFINAL|CODE|INITIALIZER)\s+([0-9]+).*",
    SIZEDATAREGEXP=r"^(?:DATA|INITIALIZED)\s+([0-9]+).*",
    SIZEEEPROMREGEXP=r"^(?:EEPROM)\s+([0-9]+).*",
    SIZECHECKCMD="$SIZETOOL -A $SOURCES",
    SIZEPRINTCMD='$SIZETOOL -d $SOURCES',

    PROGNAME="firmware",
    PROGSUFFIX=".elf"
)


def _ldflags_for_hex(env, ldflags):
    ldflags = ["--out-fmt-ihx" if f == "--out-fmt-elf" else f for f in ldflags]
    return ldflags


env.Append(
    ASFLAGS=env.get("CFLAGS", [])[:],
    __ldflags_for_hex=_ldflags_for_hex,
    ldflags_for_hex="${__ldflags_for_hex(__env__, LINKFLAGS)}"
)

# Allow user to override via pre:script
if env.get("PROGNAME", "program") == "program":
    env.Replace(PROGNAME="firmware")

#
# Target: Build executable and linkable firmware
#

target_elf = None
if "nobuild" in COMMAND_LINE_TARGETS:
    target_elf = join("$BUILD_DIR", "${PROGNAME}.elf")
    target_firm = join("$BUILD_DIR", "${PROGNAME}.ihx")
else:
    target_elf = env.BuildProgram()
    # convert elf to hex not via sdcc but via stm8-objcopy.
    # otherwise the resulting ihex file has the debug sections (if --debug --out-fmt-elf is enabled)
    # in it and it fails to flash
    target_firm = env.Command(
        join("$BUILD_DIR", "${PROGNAME}.ihx"),
        join("$BUILD_DIR", "${PROGNAME}.elf"), 
        " ".join(
            ["$OBJCOPY",
            "-O", 
            "ihex", 
            "$SOURCES", 
            "--remove-section=\".debug*\"",
            "--remove-section=SSEG",
            "$TARGET"])
    )
    env.Depends(target_firm, target_elf)

AlwaysBuild(env.Alias("nobuild", target_firm))
target_buildprog = env.Alias("buildprog", target_firm, target_firm)

#
# Target: Print binary size
#

target_size = env.Alias(
    "size", target_elf,
    env.VerboseAction("$SIZEPRINTCMD", "Calculating size $SOURCE"))
AlwaysBuild(target_size)

#
# Target: Upload firmware
#

upload_protocol = env.subst("$UPLOAD_PROTOCOL")
upload_actions = []


if upload_protocol == "serial":
    env.Replace(
        UPLOADER="stm8gal",
        UPLOADERFLAGS=[
            "-p", "$UPLOAD_PORT",
            "-R", 1, "-u", 2,
            "-V", 2, "-v", "-B", "-w"
        ],
        UPLOADCMD='"$UPLOADER" $UPLOADERFLAGS $SOURCE'
    )

    if env.subst("$UPLOAD_SPEED"):
        env.Prepend(UPLOADERFLAGS=["-b", "$UPLOAD_SPEED"])

    upload_actions = [
        env.VerboseAction(env.AutodetectUploadPort,
                          "Looking for upload port..."),
        env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")
    ]

elif "stlink" in upload_protocol:
    mcu = board_config.get("build.mcu")
    # either derive value for the "part" switch from MCU name, or use the value 
    # in the board manifest, in cases in which the derivation would be wrong.
    flash_target = board_config.get("upload.stm8flash_target", mcu[:8] + "?" + mcu[9])
    env.Replace(
        UPLOADER="stm8flash",
        UPLOADERFLAGS=[
            "-c", "$UPLOAD_PROTOCOL",
            "-p", "%s" % flash_target,
            "-s", "flash", "-w"
        ],
        UPLOADCMD='"$UPLOADER" $UPLOADERFLAGS $SOURCE'
    )

    upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

# custom upload tool
elif upload_protocol == "custom":
    upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

else:
    sys.stderr.write("Warning! Unknown upload protocol %s\n" % upload_protocol)

AlwaysBuild(env.Alias("upload", target_firm, upload_actions))

#
# Setup default targets
#

Default([target_buildprog, target_size])
