TARGET = dev

# Compiler options.
CC_DEV = g++
CC_ARM = /usr/bin/arm-linux-gnueabihf-g++

# Header includes.
INCLUDEPATH = -Iranger/cpp_version/src -Iranger/cpp_version/src/utility -Iranger/cpp_version/src/Forest -Iranger/cpp_version/src/Tree

# Flags.
CFLAGS = -Wall -static -O3 -std=c++11 -pthread

# Source directory and files.
SOURCEDIR = ranger/cpp_version/src
HEADERS := $(wildcard $(SOURCEDIR)/*.h $(SOURCEDIR)/**/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/**/*.cpp)

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