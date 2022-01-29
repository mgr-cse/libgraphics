SRC:=$(shell find ./ | grep "\.c$$")
HEADERS:=$(shell find ./ | grep "\.h$$")
OBJ:=$(addprefix ./build/, $(notdir $(patsubst %.c,%.o, $(SRC))))
TARGETS:=$(addprefix ./build/, a.out)

CFLAGS=-g
INCLUDE=-I./src
LDLIBS=-lGL -lGLU -lglut -lm

$(shell [ ! -d ./build ] && mkdir build)

all: $(TARGETS)

$(TARGETS): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build


