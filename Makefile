CC = g++
CFLAGS = -Wall -Werror -std=c++11
LDFLAGS =

INC_DIR = third_party/mpisimulator/include
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
LD_DIR = third_party/mpisimulator/lib
TARGET = demoprog

SRC = $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.cpp=.o))

all: demoprog

demoprog: main.cpp $(LIB_DIR)/libpredictor.so third_party
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(TARGET) main.cpp -L$(LIB_DIR) -L$(LD_DIR) -lpredictor -lmpisimulator

third_party: mpisimulator

mpisimulator:
	make -C third_party/mpisimulator

$(LIB_DIR)/libpredictor.so: $(OBJECTS)
	$(CC) -shared -fPIC -o $(LIB_DIR)/libpredictor.so $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) libpredictor.so

.PHONY: demo clean library third_party mpisimulator
