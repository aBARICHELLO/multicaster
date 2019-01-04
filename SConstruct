import os
import sys

env = Environment()

# Link libraries
LINUX_LIBS = ["stdc++", "sfml-graphics", "sfml-window", "sfml-system"]
WIN_LIBS = ["sfml-graphics", "sfml-window", "sfml-system"]

# Compiler/Linker flags
LINUX_FLAGS = "-Wl,-rpath,.'$ORIGIN'/lib"
WIN_FLAGS = "/O2 /std:c++17 /W2 /EHsc"

FILENAME = "bin/caster"
SOURCES = Glob("src/*.cpp")
SOURCES.extend(Glob("src/**/*.cpp"))
BINARIES = "./bin"

def pre_build():
    platform = sys.platform
    print("Building for " + platform)
    try:
        os.mkdir(BINARIES)
    except Exception:
        pass

    if platform.startswith("win"):
        build_windows()
    elif platform.startswith("linux"):
        build_linux()
    else:
        print("No builds for your current OS: " + platform)
        sys.exit(-1)

def build_linux():
    Program(
        FILENAME,
        SOURCES,
        LINKFLAGS = LINUX_FLAGS,
        LIBS=LINUX_LIBS,
    )

def build_windows():
    Program(
        FILENAME,
        SOURCES,
        CXXFLAGS = WIN_FLAGS,
        LIBS=WIN_LIBS,
        CPPPATH = "./include",
        LIBPATH = "./lib",
    )

def post_build():
    # Make the resource folder acessible by the generated binary
    try:
        os.system("cp -r ./resources ./bin")
    except Exception:
        pass

pre_build()
post_build()
