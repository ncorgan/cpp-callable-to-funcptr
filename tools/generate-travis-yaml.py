#!/usr/bin/env python3

# Generate the .travis.yml file used to specify which configurations Travis
# tests.

import yaml

# Note: earlier GCC versions support C++17, but due to a bug, attempting
# to use this library fails.
GCC_VERSIONS = [
    {"num": 8, "stds": ["c++17","c++2a"]},
    {"num": 9, "stds": ["c++17","c++2a"]},
]
CLANG_VERSIONS = [
    {"num": 6, "stds": ["c++17","c++2a"]},
    {"num": 7, "stds": ["c++17","c++2a"]},
    {"num": 8, "stds": ["c++17","c++2a"]},
]

def get_osx_node():
    return { "os": "osx" }

def get_gcc_node_for_version(version):
    return [{
        "compiler": "gcc",
        "os": "linux",
        "env": "CC=gcc-{0} CXX=g++-{0} std={1}".format(version["num"], std),
        "addons": {
            "apt": {
                "sources": [
                    "ubuntu-toolchain-r-test"
                ],
                "packages": [
                    "g++-{0}".format(version["num"])
                ]
            }
        }
    } for std in version["stds"]]

def get_clang_node_for_version(version):
    return [{
        "compiler": "clang",
        "os": "linux",
        "env": "CC=clang-{0}.0 CXX=clang++-{0}.0 std={1}".format(version["num"], std),
        "addons": {
            "apt": {
                "sources": [
                    "llvm-toolchain-trusty-{0}.0".format(version["num"])
                ],
                "packages": [
                    "clang-{0}.0".format(version["num"])
                ]
            }
        }
    } for std in version["stds"]]


if __name__ == "__main__":
    gcc_nodes = [get_gcc_node_for_version(version) for version in GCC_VERSIONS]
    clang_nodes = [get_clang_node_for_version(version) for version in CLANG_VERSIONS]

    # Flatten
    gcc_nodes = [item for innerlist in gcc_nodes for item in innerlist]
    clang_nodes = [item for innerlist in clang_nodes for item in innerlist]

    yml = {
        "language": "generic",
        "matrix": {
            "include" :
                gcc_nodes + clang_nodes
        },
        "dist": "trusty",
        "sudo": "required",
        "before_install": [
          "if [ \"$TRAVIS_OS_NAME\" = \"linux\" ]; then sudo apt-get -y install libboost-dev; fi",
          "git submodule update --init --recursive"
        ],
        "script": [
            "mkdir build",
            "cd build",
            "STD=${std} cmake ..",
            "make",
            "ctest --output-on-failure"
        ]
    }

    with open(".travis.yml", "w") as yaml_file:
        yaml_file.write("# This file is auto-generated by tools/generate-travis-yaml.py.\n\n")
        yaml.dump(yml, yaml_file, default_flow_style=False)
