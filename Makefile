SRC_DIR     = src
INC_DIR     = include

CC          = g++
CPPFLAGS    =  -I$(INC_DIR) -fPIC
CXXFLAGS    = -g -std=c++11 -Wall -fdiagnostics-color=always -Wno-reorder -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -MMD -MP `root-config --libs` -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -pthread -m64
CPPFLAGS   += `root-config --cflags`
LDFLAGS     = `root-config --libs` -lMinuit
OUTPUT_OPTION = -o $@

SRC         = $(wildcard $(SRC_DIR)/*.cpp)
OBJ         = $(SRC:%.cpp=%.o)
HDR         = $(wildcard $(INC_DIR)/*.hh)
HDR_NAMES   = $(patsubst $(INC_DIR)/%,%,$(HDR))

EXECUTABLE=headertest
LIBRARY=$(SRC_DIR)/libFindMrdTracks.so

all: clean $(LIBRARY)

.PHONY: clean all

$(EXECUTABLE) : $(OBJ) $(SRC_DIR)/FindMrdTracks_RootDict.o $(SRC_DIR)/$(EXECUTABLE).o
	@echo "making $@"
	g++ $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(OUTPUT_OPTION)
 
$(SRC_DIR)/libFindMrdTracks.so:  $(OBJ) $(SRC_DIR)/FindMrdTracks_RootDict.o
	g++ $(CXXFLAGS) $(CPPFLAGS) -shared $(LDFLAGS) $^ $(OUTPUT_OPTION)

clean:
	@-rm -f $(OBJ) \
	        $(SRC_DIR)/FindMrdTracks_RootDict.o \
	        $(SRC_DIR)/FindMrdTracks_RootDict.c \
	        $(SRC_DIR)/FindMrdTracks_RootDict_rdict.pcm \
	        $(INC_DIR)/FindMrdTracks_Linkdef.h \
	        $(EXECUTABLE) \
	        $(LIBRARY) \
	        $(SRC_DIR)/FindMrdTracks_RootDict.cpp

$(SRC_DIR)/FindMrdTracks_RootDict.cpp: $(INC_DIR)/FindMrdTracks_Linkdef.h
	@echo "making $@"
	rootcint -f $@ -c -p $(CPPFLAGS) $(HDR_NAMES) $^

$(INC_DIR)/FindMrdTracks_Linkdef.h:
	@echo "making $@"
	@cat $(INC_DIR)/linkdefpreamble.txt > $@
	@for file in $(INC_DIR)/*.hh;\
	  do echo -n $$(cat $(INC_DIR)/linkdefincludeheader.txt) >> $@;\
	  echo " \"$${file}\";" >> $@;\
	done
	@cat $(INC_DIR)/linkdefpostamble.txt >> $@

#-include $(SRC:%.cpp=%.d)

## IMPORTANT NOTES:
# we do not need to write our own rule for %.o : %.cpp etc, if we use make's implicit rules. 
# These will compile .o files (with g++) for the '.cpp' or '.cc' extension, but NOT for the '.cxx' extension
# We can pass flags to the implicit rules by using the standard flag sets: 
#   CPPFLAGS for pre-processor flags (e.g. 'include' paths)
#   CXXFLAGS for other c++ compiler flags (including -shared)
#   LDFLAGS for library paths
#   LDLIBS for libraries (-lMyLib)
#   OUTPUT_OPTION for defining target naming rules. (Perhaps this could somehow be used to put the objects in a build dir, but then the implicit rules do not get triggered for $(BUILD_DIR)/%.o files...)
# compiling and linking against ROOT dictionary generated with rootcint is necessary for classes that depend on ROOT, otherwise you'll be left with undefined references to Streamer, Class, IsA... etc
# rootcint calls '$CC', which unless not otherwised defined will resolve to 'gcc' NOT 'g++' and will not recognise the -cstd=c++1y flag (and some CXX flags) and will fall over.
# rootcint also requires the include directory to be specified WITHOUT A SPACE AFTER THE -I, which would normally be ok. To allow the same CPPFLAGS to be used for both, ensure CPPFLAGS uses no space!


#######################################################
# simple template with automatic dependency inclusion #
# SRC = $(wildcard *.cpp)                             #
# all: main                                           #
#                                                     #
# main: $(SRC:%.cpp=%.o)                              #
#	g++ -std=c++1y -MD -MP -o $@ $^               #
#                                                     #
# -include $(SRC:%.cpp=%.d)                           #
#######################################################
