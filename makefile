#===================================================================
#
#Makefile for MANGO
#
#===================================================================

# Compilers
GCC=g++ -DCOMP='"GNU C++ Compiler"'
LLVM=g++ -DCOMP='"LLVM C++ Compiler"'

export LANG=C
export LC_ALL=C

# LIBS
LIBS=-lstdc++ -lGL -lGLU -lSDL2 -lGLEW
LIBS_Mac =-lstdc++ -framework OpenGL -framework SDL2 -lGLEW #-lGLU -lSDL2 -lGLEW

# Debug Flags
LLVM_CFLAGS= -O3 -mtune=native -funroll-loops -I./hdr -std=c++11
LLVM_LDFLAGS= -lstdc++ -I./hdr -I./src/qvoronoi

GCC_CFLAGS=-O3 -mtune=native -funroll-all-loops -fexpensive-optimizations -funroll-loops -I./include/ -std=c++11 
GCC_LDFLAGS= -lstdc++ -I./hdr -I./src/qvoronoi -std=c++11

# Objects
OBJECTS= \
obj/Arguments.o \
obj/Controls.o \
obj/DataInput.o \
obj/DrawGroup.o \
obj/Main.o \
obj/Output.o \
obj/Shader.o \
obj/System.o \
obj/Text.o \
obj/Window.o

EXECUTABLE=mango

all: $(OBJECTS) gcc

# Serial Targets
gcc: $(OBJECTS)
	$(GCC) $(OBJECTS) $(GCC_LDFLAGS) $(LIBS) -o $(EXECUTABLE)

$(OBJECTS): obj/%.o: Source/%.cpp
	$(GCC) -c -o $@ $(GCC_CFLAGS) $<


clean:
	@rm -f obj/*.o
	@rm -f obj/*/*.o

purge:
	@rm -f obj/*.o
	@rm -f obj/*/*.o
	@rm -f $(EXECUTABLE)
