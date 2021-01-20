TARGET = dev

# Compiler options.
CC_DEV = g++
CC_ARM = /usr/bin/arm-linux-gnueabihf-g++

# Header includes.
INCLUDEPATH = -IMochiMochi -Ieigen

# Flags.
CFLAGS = -Wall -static -O3 -std=c++14 

# Dependency.
LDFLAGS = -lboost_serialization

# Source directory and files.
SOURCEDIR = src
HEADERS := $(wildcard $(SOURCEDIR)/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)

# Target output.
BUILDTARGET = OrbitAI_Mochi

# Target compiler environment.
ifeq ($(TARGET),arm)
	CC = $(CC_ARM)
else
	CC = $(CC_DEV)
endif

all:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(HEADERS) $(SOURCES) -o $(BUILDTARGET) $(LDFLAGS)

clean:
	rm -f $(SOURCEDIR)/*.o
	rm -f $(BUILDTARGET)