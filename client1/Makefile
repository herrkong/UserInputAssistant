INC_DIR:= ./
SRC_DIR:= ./
SRCS:=$(wildcard *.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -ljson

CXX:= gcc

clean:
	 rm -rf $(EXE)
	 rm -rf $(OBJS) 

