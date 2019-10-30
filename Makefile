CC = g++
CFLAGS = -Wall -Werror -std=c++11 -fPIC
LDFLAGS =

INC=-Iinclude -Ithird_party/mpisimulator/include
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
LD = -Lthird_party/mpisimulator/lib -L$(LIB_DIR)
TARGET = demoprog

SRC = $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.cpp=.o))

all: demoprog

demoprog: main.cpp $(LIB_DIR)/libpredictor.so third_party
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $(LD) main.cpp -lpredictor -lmpisimulator

third_party: mpisimulator

mpisimulator:
	make -C third_party/mpisimulator

$(LIB_DIR)/libpredictor.so: $(OBJECTS) third_party
	mkdir -p $(LIB_DIR)
	$(CC) $(LD) -shared -fPIC -o $(LIB_DIR)/libpredictor.so $(OBJECTS) -lmpisimulator

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	make -C third_party/mpisimulator clean
	rm -f $(OBJECTS) $(TARGET) libpredictor.so

.PHONY: demo clean library third_party mpisimulator
