#===================================================================
#
#								Makefile for MANGO
#
#===================================================================

# Compilers
GCC=g++ -DCOMP='"GNU C++ Compiler"'
LLVM=g++ -DCOMP='"LLVM C++ Compiler"'

export LANG=C
export LC_ALL=C

# LIBS
LIBS=-lstdc++

# Debug Flags
LLVM_CFLAGS= -O3 -mtune=native -funroll-loops -I./hdr -I./src/qvoronoi
LLVM_LDFLAGS= -lstdc++ -I./hdr -I./src/qvoronoi

GCC_CFLAGS=-O3 -mtune=native -funroll-all-loops -fexpensive-optimizations -funroll-loops -I./hdr -I./src/qvoronoi
GCC_LDFLAGS= -lstdc++ -I./hdr -I./src/qvoronoi

# Objects
OBJECTS= \
Source/Arguments.o \

Source/Controls.o \
Source/DataInput.o \
Source/DrawGroup.o \
Source/Main.o \
Source/Output.o \
Source/Shader.o \
Source/System.o \
Source/Text.o \
Source/Window.o

EXECUTABLE=mango

all: $(OBJECTS) gcc

# Serial Targets
gcc: $(OBJECTS)
	$(GCC) $(GCC_LDFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJECTS): obj/%.o: src/%.cpp
	$(GCC) -c -o $@ $(GCC_CFLAGS) $<

llvm: $(OBJECTS)
	$(LLVM) $(LLVM_LDFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJECTS): obj/%.o: src/%.cpp
	$(LLVM) -c -o $@ $(LLVM_CFLAGS) $<

clean:
	@rm -f obj/*.o
	@rm -f obj/*/*.o

purge:
	@rm -f obj/*.o
	@rm -f obj/*/*.o
	@rm -f $(EXECUTABLE)
