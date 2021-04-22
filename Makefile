CXX=g++
CX=gcc
CXXFLAGS=-g -std=c++11
CXFLAGS=-g
LDFLAGS=-lpthread -lmpi

CC_SRCS=$(wildcard *.cc)
C_SRCS=$(wildcard *.c)
CC_BINS=$(patsubst %.cc, %, $(CC_SRCS))
C_BINS=$(patsubst %.c, %, $(C_SRCS))


all:$(CC_BINS) $(C_BINS)

$(CC_BINS):%:%.cc
	@$(CXX) $< $(CXXFLAGS) -o $@ $(LDFLAGS)
	@echo -e "\033[32mCXX\033[0m" $@

$(C_BINS):%:%.c
	@$(CX) $< $(CXFLAGS) -o $@ $(LDFLAGS)
	@echo -e "\033[32mCXX\033[0m" $@

clean:
	rm -f $(C_BINS) $(CC_BINS) 

