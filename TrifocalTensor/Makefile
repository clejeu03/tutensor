CXX			= g++
CXXFLAGS 	= -Wall -g -I include -I libs/eigen
LDFLAGS 	= -lSDL -lSDL_image -lSDL_gfx

SRC_PATH 	= src
BIN_PATH 	= bin
EXEC 		= trifocal

SRC_FILES 	= $(shell find $(SRC_PATH) -type f -name '*.cpp')
OBJ_FILES 	= $(patsubst $(SRC_PATH)/%.cpp, $(SRC_PATH)/%.o, $(SRC_FILES))

DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all: $(BIN_PATH)/$(EXEC)

$(BIN_PATH)/$(EXEC): $(OBJ_FILES) 
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: $(BIN_PATH)/$(EXEC) &"
	@echo "--------------------------------------------------------------"

$(SRC_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $^ 

clean:
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	rm -f *~ $(OBJ_FILES) $(BIN_PATH)/$(EXEC)

bigclean :
	@echo "**************************"
	@echo "BIG CLEAN"
	@echo "**************************"
	find . -name '*~' -exec rm -fv {} \;
	rm -f *~ $(OBJ_FILES) $(BIN_PATH)/$(EXEC) output/*

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)
