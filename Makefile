
# Linux (default)
EXE = MANGO
LDFLAGS = -lGL -lGLU -lSDL2

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE = MANGO.exe
    LDFLAGS = -lopengl32 -lglu32 -lSDL2
endif

# OS X
ifeq "$(OSTYPE)" "darwin"
    LDFLAGS = -framework OpenGL
endif

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

OBJECTS = \
Source/Arguments.o \
Source/Controls.o \
Source/DataInput.o \
Source/DrawGroup.o \
Source/Main.o \
Source/Output.o \
Source/Shader.o \
Source/System.o \
Source/Text.o \
Source/Window.o \



MANGO: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) $(LDFLAGS)



IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)



Source\Arguments.o \
Source\Controls.o \
Source\DataInput.o \
Source\DrawGroup.o \
Source\Main.o \
Source\Output.o \
Source\Shader.o \
Source\System.o \
Source\Text.o \
Source\Window.o \