UNAMES := $(shell uname)


CFXXLAGS=-std=gnu++2a -Wno-macro-redefined -Wall -g
CFXXLAGS += -Wno-deprecated-declarations

PROJ_DIR=$(CURDIR)

ifeq ($(UNAMES), Darwin)
	CXX=clang++
	LDFLAGS=-L$(BREW_PREFIX)/lib -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION
	BREW_PREFIX=$(shell brew --prefix)
	INCLUDES=-I$(BREW_PREFIX)/include -I$(PROJ_DIR)/include
else ifeq ($(UNAMES), Linux)
	CXX=g++
	LDFLAGS := -lglfw -DGL_SILENCE_DEPRECATION
	INCLUDES := -I/usr/include -I$(PROJ_DIR)/include
else
	$(error Unsupported platform: $(UNAMES))
endif

TARGET=build/main

MAIN_FILE ?= src/main.cpp
ADDITIONAL_FILES=$(wildcard $(PROJ_DIR)/include/**/*.c $(PROJ_DIR)/include/**/*.cpp)

all: build run

build: $(TARGET)

$(TARGET): $(MAIN_FILE)
	@mkdir -p build
	echo "Debugging $(PROJ_DIR)"
	$(CXX) -o $(TARGET) $(MAIN_FILE) $(ADDITIONAL_FILES) $(INCLUDES) $(LDFLAGS) $(CFXXLAGS)

run:
	@./$(TARGET) $(INPUT) $(OUTPUT)

clean:
	$(RM) -rf build/
	$(RM) imgui.ini

rebuild: clean all