CROSS_COMPILE = 

CC =  $(CROSS_COMPILE)clang
CXX =  $(CROSS_COMPILE)clang++
AR =  $(CROSS_COMPILE)ar

ARFLAGS = cr

CFLAGS =  -Wall
debug = y

# ifeq ( $(debug), y)
CFLAGS += -g
# else
# CFLAGS += -O2 -s
# endif

DEFS = -std=c++20 -stdlib=libc++
CFLAGS +=  $(DEFS)
LDFLAGS =  $(LIBS)

DIRECTORIES := $(wildcard */)

test :
	${CXX} ${CFLAGS} -o test Test/test.cc
# VPATH := CM:DS:Str:Test
.PHONY : clean
clean :
	rm -f *.o