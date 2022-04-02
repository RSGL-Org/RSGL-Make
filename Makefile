GXX = g++
SOURCE = source/*.cpp
OUTPUT = RSGLMake


build:
	$(GXX) $(SOURCE) -o $(OUTPUT)

install:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	sudo cp $(OUTPUT) /usr/bin

localInstall:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cp $(OUTPUT) ~/.local/bin

example:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cd examples && ../RSGLMake otherMethod