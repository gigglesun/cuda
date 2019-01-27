LDFLAGS += -L/usr/local/cuda/lib64 -L/home/qsun/explore/glew-2.1.0/lib

LDFLAGS += -lglut -lGL -lGLU -lGLEW

CUDA_PATH ?= /usr/local/cuda-10.0
HOST_COMPILER ?= g++
#NVCC := $(CUDA_PATH)/bin/nvcc -ccbin $(HOST_COMPILER)
NVCC := /usr/local/cuda-10.0/bin/nvcc

NVCC_FLAGS = -g -G -Xcompiler "-Wall -Wno-deprecated-declarations"
INCLUDES  := -I../../common/inc

all: flashlight
build: flashlight

flashlight: main.o kernel.o
	$(NVCC) $^ -o $@ $(LDFLAGS)

main.o: main.cpp kernel.h interactions.h
	$(NVCC) $(INCLUDES) $(NVCC_FLAGS) -c $< -o $@

kernel.o: kernel.cu kernel.h
	$(NVCC) $(INCLUDES) $(NVCC_FLAGS) -c $< -o $@

