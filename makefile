SHELL = /bin/sh
CC = g++
LIBS = -lm -lboost_system -lboost_thread -lpthread -std=c++0x -lgsl -lgslcblas 
CPPFLAGS = -g -O2 -Wall

VPATH=%.h ./include
VPATH=%.o ./obj

OBJDIR = ./obj
INCLUDE_DIR = ./include
INCLUDES  := $(addprefix -I,$(INCLUDE_DIR))

objects = $(addprefix $(OBJDIR)/, main.o individual.o population.o worker.o)

MY_APPS = app1

$(MY_APPS) : $(objects)
	${CC} -o app1 ${objects} ${CPPFLAGS} ${LIBS}

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CPPFLAGS) ${LIBS} ${INCLUDES} $< -o $@


$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY : clean
clean:
	rm -f ${MY_APPS}
	rm -f ${objects}
