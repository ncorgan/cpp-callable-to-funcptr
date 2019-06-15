#!/usr/bin/env python3

# Generate the appveyor.yml file used to specify which configurations Appveyor
# tests.

import yaml

MSVC_VERSIONS = [
    {"num": 15, "year": 2017, "stds": ["c++17","c++latest"], "boost": "1_69_0"},
]

def get_msvc_nodes_for_version(version):
    win32_base = {
        "GENERATOR_NAME": "Visual Studio {0} {1}".format(version["num"], version["year"]),
        "BOOST_VERSION": version["boost"]
    }
    win64_base = {
        "GENERATOR_NAME": "Visual Studio {0} {1} Win64".format(version["num"], version["year"]),
        "BOOST_VERSION": version["boost"]
    }

    # Appveyor currently runs their VS2017 builds on a different VM.
    if version["num"] >= 15:
        win32_base["APPVEYOR_BUILD_WORKER_IMAGE"] = "Visual Studio 2017"
        win64_base["APPVEYOR_BUILD_WORKER_IMAGE"] = "Visual Studio 2017"

    ret = []

    if len(version["stds"]) > 0:
        for std in version["stds"]:
            win32 = win32_base.copy()
            win64 = win64_base.copy()

            win32["STD"] = std
            win64["STD"] = std

            ret += [win32, win64]
    else:
        ret = [win32_base, win64_base]

    return ret

if __name__ == "__main__":
    msvc_nodes = [get_msvc_nodes_for_version(version) for version in MSVC_VERSIONS]

    # Flatten
    msvc_nodes = [item for innerlist in msvc_nodes for item in innerlist]

    yml = {
        "platform": "x64",
        "configuration": "Release",
        "install": "git submodule update --init --recursive",
        "environment": {
            "matrix": msvc_nodes
        },
        "build_script": [
            "mkdir build",
            "cd build",
            "cmake -G \"%GENERATOR_NAME%\" -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=C:/Libraries/boost_%BOOST_VERSION% -DMANUALLY_SET_STANDARD_FLAGS=ON ..",
            "cmake --build . --config \"Release\"",
            "ctest --output-on-failure"
        ]
    }

    with open("appveyor.yml", "w") as yaml_file:
        yaml_file.write("# This file is auto-generated by tools/generate-appveyor-yaml.py.\n\n")
        yaml.dump(yml, yaml_file, default_flow_style=False)