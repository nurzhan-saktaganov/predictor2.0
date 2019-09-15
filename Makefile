CC = g++
CFLAGS = -Wall -Werror -std=c++11
LDFLAGS =

INC_DIR = third_party/mpisimulator/include
SRC_DIR = src
OBJ_DIR = obj
TARGET = demoprog

SRC = $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.cpp=.o))

all: demo

demo: libpredictor.so
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(TARGET) main.cpp -L./ -lpredictor -lmpisimulator

libpredictor.so: $(OBJECTS)
	$(CC) -shared -fPIC -o libpredictor.so $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) libpredictor.so

.PHONY: demo clean library
