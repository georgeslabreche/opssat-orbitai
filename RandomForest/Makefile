TARGET = dev

# Compiler options.
CC_DEV = g++
CC_ARM = /usr/bin/arm-linux-gnueabihf-g++

# Header includes.
INCLUDEPATH = -Icsv2/include -Iranger/cpp_version/src -Iranger/cpp_version/src/utility -Iranger/cpp_version/src/Forest -Iranger/cpp_version/src/Tree

# Flags.
CFLAGS = -Wall -O3 -std=c++11 -pthread

# Source directory for ranger.
SOURCEDIR_RANGER = ranger/cpp_version/src

# Srouce directory for OrbitAI_ranger.
SOURCEDIR = src

# Collect all header and source files.
HEADERS := $(wildcard $(SOURCEDIR_RANGER)/*.h $(SOURCEDIR_RANGER)/**/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR_RANGER)/**/*.cpp)

# Target output.
BUILDTARGET = OrbitAI_ranger

# Target compiler environment.
ifeq ($(TARGET),arm)
	CC = $(CC_ARM)
else
	CC = $(CC_DEV)
endif

all:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(HEADERS) $(SOURCES) -o $(BUILDTARGET)

clean:
	rm -f $(BUILDTARGET)