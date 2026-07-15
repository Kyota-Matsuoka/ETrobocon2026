mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

SRCLANG = c++

APPL_CXXOBJS +=\
	Walker.o \
	LineTracer.o \
	ScenarioTracer.o \
	LineMonitor.o \
	Starter.o \
	SimpleTimer.o \
	Test.o \
	PIDControl.o\
	DistanceMeter.o\

APPL_LIBS += -lm

APPL_DIRS +=  $(mkfile_path)app $(mkfile_path)unit

INCLUDES += \
	-I$(mkfile_path)app\
	-I$(mkfile_path)unit
