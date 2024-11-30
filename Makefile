#
# Makefile for Six Degrees
#
# Project 2 (fall 2020)
#
# STUDENT TODO: Add any testing files before running the provide target
# 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 
INCLUDES = $(shell echo *.h)

SixDegrees: main.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} -o $@ $^

%.o: %.cpp ${INCLUDES}
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf SixDegrees *.o *.dSYM

##############################################
# PLEASE DO NOT FORGET TO ADD ANY ADDITIONAL #
# FILES YOU MADE, SUCH AS TESTING FILES.     #
##############################################

providephase1:
	provide comp15 proj2phase1 SixDegrees.cpp SixDegrees.h \
			main.cpp README Makefile # ADD TESTING FILES HERE

providephase2:
	provide comp15 proj2phase2 SixDegrees.cpp SixDegrees.h \
			main.cpp README Makefile # ADD TESTING FILES HERE
