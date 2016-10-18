# A few common Makefile items

CC=/data/cdl110/llvm/build/bin/clang++
CXX=/data/cdl110/llvm/build/bin/clang++

CPPFLAGS=-std=c++0x -pthread -Wall -fsanitize=thread -g

UNAME = $(shell uname)

INCLUDE = -I../include

# C preprocessor flags
CPPFLAGS += $(INCLUDE) -g

# C++ compiler flags
CXXFLAGS += $(CPPFLAGS)

# C compiler flags
CFLAGS += $(CPPFLAGS)

# Linker flags
LDFLAGS +=
