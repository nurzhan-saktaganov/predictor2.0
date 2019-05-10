CC = g++
CFLAGS = -Wall -Werror -std=c++11
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
TARGET = demoprog

SRC = DFormat.cpp Distribute.cpp Entity.cpp Predictor.cpp Range.cpp PGrid.cpp
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.cpp=.o))

all: demo

demo: libpredictor.so
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp -L./ -lpredictor

libpredictor.so: $(OBJECTS)
	$(CC) -shared -fPIC -o libpredictor.so $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) libpredictor.so

.PHONY: demo clean library
