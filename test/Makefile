UNAMES := $(shell uname)

CXX=clang++
CFXXLAGS=-std=gnu++2a -Wno-macro-redefined -Wall -g
CFXXLAGS += -Wno-deprecated-declarations

ifeq ($(UNAMES), Darwin)
	LDFLAGS=-L$(BREW_PREFIX)/lib -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION
	BREW_PREFIX=$(shell brew --prefix)
	INCLUDES=-I$(BREW_PREFIX)/include -I$(PROJ_DIR)/include
else
	$(error Unsupported platform: $(UNAMES))
endif

PROJ_DIR=$(CURDIR)

TARGET=build/test

MAIN_FILE ?= 
ADDITIONAL_FILES=$(wildcard $(PROJ_DIR)/include/**/*.c $(PROJ_DIR)/include/**/*.cpp)

all: build run

build: $(TARGET)

$(TARGET): $(MAIN_FILE)
	@mkdir -p build
	$(CXX) -o $(TARGET) $(MAIN_FILE) $(ADDITIONAL_FILES) $(INCLUDES) $(LDFLAGS) $(CFXXLAGS)

run:
	@./$(TARGET) $(INPUT) $(OUTPUT)

clean:
	$(RM) -rf build/

rebuild: clean all