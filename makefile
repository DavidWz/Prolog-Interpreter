CC = gcc
CXX = g++
RM = rm -f
DEBUG = -Wall
CPPFLAGS = -g -O3 -c -std=c++14 $(DEBUG)
LDFLAGS = -g $(Debug)

SRCS = Main.cpp Parser.cpp Interpreter.cpp Substitution.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

all: PrologInterpreter

PrologInterpreter: $(OBJS)
	$(CXX) $(LDFLAGS) -o PrologInterpreter $(OBJS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend