CC := g++
OBJ_DIR := obj
DIST_DIR := dist
SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))
DIST_FILE := $(DIST_DIR)/async_server
CFLAGS := -g -c -std=c++14
LFLAGS :=

all: $(DIST_FILE)

$(OBJ_DIR)/%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $<

$(DIST_FILE): $(OBJ_FILES)
	${CC} ${LFLAGS} -o $@ $^

$(OBJ_FILES): | $(OBJ_DIR)

$(DIST_FILE): | $(DIST_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

run:
	cd ./$(DIST_DIR) && ./async_server --config ../test-config.json

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DIST_DIR)