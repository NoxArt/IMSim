NAME = imsim

# Windows
ifeq ($(OS),Windows_NT)
    RM = rm -rf
    PATH_SEP = /
    FILENAME = lib$(NAME).a
    CFLAGS_LIBRARY = $(CFLAGS_RELEASE) -shared -Wl,-enable-auto-import
# Pinguin
else
    RM = rm -rf
    PATH_SEP = /
    FILENAME = lib$(NAME).a
    CFLAGS_LIBRARY = $(CFLAGS_RELEASE) -shared
endif

CC = g++
ARCHIVER = ar -rvos

TEST_FOLDER = .$(PATH_SEP)tests

RELEASE_DIR = .$(PATH_SEP)release
RELEASE_FILEPATH = $(RELEASE_DIR)$(PATH_SEP)$(FILE_NAME)
CFLAGS_RELEASE = -O2 -Wall

DEBUG_DIR = .$(PATH_SEP)debug
DEBUG_FILEPATH = $(DEBUG_DIR)$(PATH_SEP)$(FILE_NAME)
CFLAGS_DEBUG = -g -O0 -Wall -W


INCLUDE = 

GARBAGE = *.o Randomness$(PATH_SEP)*.o Stats$(PATH_SEP)*.o QS$(PATH_SEP)*.o Continuous$(PATH_SEP)*.o

OBJECTS =   Simulation.o Simulator.o Calendar.o Event.o Process.o Randomness$(PATH_SEP)Generator.o\
	    Entity.o Continuous$(PATH_SEP)Condition.o\
	    Continuous$(PATH_SEP)ConditionsContainer.o Action.o Continuous$(PATH_SEP)Block.o\
	    Continuous$(PATH_SEP)Integrator.o Continuous$(PATH_SEP)BlockContainer.o
	
# Continuous$(PATH_SEP)BlockStrategy.o

CS = *.h *.cpp


.PHONY: clean
.SUFFIXES: .cpp


all: $(OBJECTS)
	$(ARCHIVER) $(FILENAME) $(OBJECTS)
	
$(NAME): $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS_RELEASE) -c $^ -o $@

clean:
	$(RM) $(GARBAGE)
	
purge:	
	$(RM) $(GARBAGE) $(FILENAME)
