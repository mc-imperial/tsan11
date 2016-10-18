# A few common Makefile items

CC=false
CXX=clang++

CXXFLAGS=-std=c++0x -pthread -Wall -fsanitize=thread -g -I../include

UNAME = $(shell uname)


