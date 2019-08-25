CC = g++
CFLAGS = -Wall -Werror -std=c++11 -g
LDFLAGS =

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
TARGET = demoprog

SRC = $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.cpp=.o))

all: demoprog

demoprog: main.cpp $(LIB_DIR)/libmpisimulator.so
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(TARGET) main.cpp -L./$(LIB_DIR) -lmpisimulator

$(LIB_DIR)/libmpisimulator.so: $(OBJECTS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -fPIC -o $(LIB_DIR)/libmpisimulator.so $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) $(LIB_DIR)/libmpisimulator.so

.PHONY: demo clean library
