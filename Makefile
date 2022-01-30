SRC:=$(shell find ./ | grep "\.c$$" | grep -v "^./src/graphics")
HEADERS:=$(shell find ./ | grep "\.h$$")
OBJ:=$(addprefix  ./build/, $(patsubst %.c, %.o, $(SRC)))

SRC_GRAPHLIB:=$(shell find ./src/graphics/ | grep "\.c$$")
OBJ_GRAPHLIB:=$(addprefix  ./build/, $(patsubst %.c, %.o, $(SRC_GRAPHLIB)))

DIRS:=$(addprefix ./build/, $(dir $(SRC)))
DIRS+=$(addprefix ./build/, $(dir $(SRC_GRAPHLIB)))
$(shell [ ! -d ./build ] && mkdir -p $(DIRS))

CFLAGS=-g
INCLUDE=-I./src
LDLIBS=-lGL -lGLU -lglut -lm -lpthread
LDFLAGS=-L./build/

build/a.out: $(OBJ) build/libgraphics.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/libgraphics.a: $(OBJ_GRAPHLIB)	
	ar rcs $@ $^

build/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build


