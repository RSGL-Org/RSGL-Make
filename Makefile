GXX = g++
SOURCE = source/*.cpp
OUTPUT = RSGLMake


all:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cd examples && ../RSGLMAKE.exe