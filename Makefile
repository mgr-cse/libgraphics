SRC:=$(shell find ./ | grep "\.c$$" | grep -v "^./src/graphics")
HEADERS:=$(shell find ./ | grep "\.h$$")
OBJ:=$(addprefix  ./build/, $(patsubst %.c, %.o, $(SRC)))

SRC_GRAPHLIB:=$(shell find ./src/graphics/ | grep "\.c$$")
OBJ_GRAPHLIB:=$(addprefix  ./build/, $(patsubst %.c, %.o, $(SRC_GRAPHLIB)))

DIRS:=$(addprefix ./build/, $(dir $(SRC)))
DIRS+=$(addprefix ./build/, $(dir $(SRC_GRAPHLIB)))
$(shell [ ! -d ./build ] && mkdir -p $(DIRS))

# check OS
OSTYPE:=
ifeq ($(OS),Windows_NT)
	OSTYPE+=Win32
else 
	OSTYPE+=$(shell uname -s)
endif

CFLAGS=-g
INCLUDE=-I./src -I./src/graphics

LDLIBS:=
LDFLAGS=-L./build/
ifeq ($(OSTYPE),Linux)
	LDLIBS+=-lGL -lGLU -lglut
endif
ifeq ($(OSTYPE),Darwin)
	LDLIBS+=-framework opengl -framework glut
endif
ifeq ($(OSTYPE),Win32)
	INCLUDE+=
	LDLIBS+=-lopengl32 -lfreeglut
	LDFLAGS+=
endif
LDLIBS+=-lm -lpthread

build/a.out: $(OBJ) build/libgraphics.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/libgraphics.a: $(OBJ_GRAPHLIB)	
	$(AR) rcs $@ $^

build/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build

.PHONY: clean