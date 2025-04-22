CXX=clang++
CFXXLAGS=-std=gnu++2a -Wno-macro-redefined -Wall -g
LDFLAGS=-L$(BREW_PREFIX)/lib -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION

BREW_PREFIX=$(shell brew --prefix)
PROJ_DIR=$(CURDIR)

TARGET=build/main

MAIN_FILE ?= src/main.cpp

INCLUDES=-I$(PROJ_DIR)/include -I$(BREW_PREFIX)/include

all: build run

build: $(TARGET)

$(TARGET): $(MAIN_FILE)
	@mkdir -p build
	$(CXX) -o $(TARGET) $(MAIN_FILE) $(INCLUDES) $(LDFLAGS) $(CFXXLAGS)

run:
	@./$(TARGET) $(INPUT) $(OUTPUT)

clean:
	$(RM) -rf build/

rebuild: clean all