CC = gcc
IDIRS = -I/home/gopiandcode/Documents/C-Projects/openGL
CFLAGS = -Wall -Werror $(IDIRS)
LDLIBS = -lm -lGL -lglut -lglfw -lGLEW 
PROG = main.exe
OBJS = main.o shader.o shaderloader.o logger.o errorchk.o list.o shadermanager.o
VPATH = util:error:graphics

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LDLIBS) $(CFLAGS) $(OBJS) -o $@ 


main.o : shader.h shadermanager.h logger.h errorchk.h
shader.o : shader.h
shaderloader.o : shaderloader.h
shadermanager.o : shadermanager.h list.h errorchk.h shaderloader.h
errorchk.o : errorchk.h logger.h
logger.o : logger.h
list.o : list.h
matrix.o : matrix.h



