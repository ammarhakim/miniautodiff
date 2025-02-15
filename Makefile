# -*- makefile-gmake -*-

CFLAGS ?= -O3 -g -ffast-math -fPIC -MMD -MP -DGIT_COMMIT_ID=\"$(GIT_TIP)\" -DGKYL_BUILD_DATE="${BUILD_DATE}" -DGKYL_GIT_CHANGESET="${GIT_TIP}"
CXXFLAGS ?= -O3 -g -ffast-math -fPIC -MMD -MP -Wall -std=c++17

INCLUDES = -I.

all: newton quad

newton: Examples/newton.cxx
	$(MKDIR_P) build/Examples
	$(CXX) $(INCLUDES) $(CXXFLAGS) Examples/newton.cxx -o build/Examples/newton

quad: Examples/quad.cxx
	$(MKDIR_P) build/Examples
	$(CXX) $(INCLUDES) $(CXXFLAGS) Examples/quad.cxx -o build/Examples/quad

.PHONY:
clean:
	rm -rf build

# command to make dir
MKDIR_P ?= mkdir -p
