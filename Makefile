SRC:=$(shell find ./ | grep "\.c$$")
HEADERS:=$(shell find ./ | grep "\.h$$")
OBJ:=$(addprefix  ./build/, $(patsubst %.c, %.o, $(SRC)))
DIRS:=$(addprefix ./build/, $(dir $(SRC)))


CFLAGS=-g
INCLUDE=-I./src
LDLIBS=-lGL -lGLU -lglut -lm -lpthread

$(shell [ ! -d ./build ] && mkdir -p $(DIRS))

build/a.out: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build


