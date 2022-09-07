

# Linux Makefile
# Socket class
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
LDFLAGS := -lssl -lcrypto
DEPS := Socket.h
TARGETS := ipv4-test.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o Socket.o
	$(info $$^ = $^)
	$(CXX) -g $^ Socket.o -o $^

ipv4-test.out: ipv4-test.o Socket.o
	$(CXX) -g ipv4-test.o Socket.o -o $@

ipv6-test.out: ipv6-test.o Socket.o
	$(CXX) -g ipv6-test.o Socket.o -o $@

ssl-ipv4-test.out: ssl-ipv4-test.o Socket.o
	$(CXX) -g ssl-ipv4-test.o Socket.o -o $@