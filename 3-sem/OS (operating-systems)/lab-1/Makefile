CXX			:= g++
CXX_FLAGS	:= -std=c++20

BIN			:= bin
SRC			:= src
RUN			:= run_files
PROCESSES 	:= processes
CALLS 		:= calls
EXECUTABLE	:= run_parent
CHILD 		:= run_child

build: $(BIN)/$(EXECUTABLE)
	@echo "Build successfully"

run:
	@echo "Running"
	$(BIN)/$(EXECUTABLE)

$(BIN):
	mkdir $@

$(BIN)/$(EXECUTABLE): $(BIN) | $(BIN)/$(CHILD)
	$(CXX) $(CXX_FLAGS) -o $@ $(RUN)/$(EXECUTABLE).cpp $(SRC)/$(CALLS)/calls.cpp $(SRC)/$(PROCESSES)/parent_process.cpp

$(BIN)/$(CHILD): $(BIN)
	$(CXX) $(CXX_FLAGS) -o $@ $(RUN)/$(CHILD).cpp $(SRC)/$(CALLS)/calls.cpp $(SRC)/$(PROCESSES)/child_process.cpp

clear:
	@echo "Clearing"
	-rm -r $(BIN)