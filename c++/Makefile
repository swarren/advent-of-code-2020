srcs := $(wildcard *.cpp)
bins := $(srcs:.cpp=.bin)

all: $(bins)

%.bin: %.cpp Makefile
	clang++-11 -ggdb -std=c++2a -Wall -Werror -O2 -o $@ $<
