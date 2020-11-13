CXX=g++
CXXFLAGS=-g -std=c++11
LDFLAGS=-lpthread

CC_SRCS=$(wildcard *.cc)
C_SRCS=$(wildcard *.c)
CC_BINS=$(patsubst %.cc, %, $(CC_SRCS))
C_BINS=$(patsubst %.c, %, $(C_SRCS))


all:$(CC_BINS) $(C_BINS)

$(CC_BINS):%:%.cc
	@$(CXX) $< $(CXXFLAGS) -o $@ $(LDFLAGS)
	@echo "CXX" $@

$(C_BINS):%:%.c
	@$(CXX) $< $(CXXFLAGS) -o $@ $(LDFLAGS)
	@echo "CXX" $@

clean:
	rm -f $(C_BINS) $(CC_BINS) 

