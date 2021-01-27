srcs := $(wildcard *.cpp)
bins := $(srcs:.cpp=.bin)

all: $(bins)

%.bin: %.cpp Makefile
	g++ -std=c++17 -Wall -O2 -o $@ $<
