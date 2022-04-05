GXX = g++
SOURCE = source/*.cpp
OUTPUT = RSGLMake

LOCALBINPATH = ~/.loca/bin
BINPATH = /usr/bin 

build:
	$(GXX) $(SOURCE) -o $(OUTPUT)

install:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	sudo cp $(OUTPUT) $(BINPATH)

localInstall:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cp $(OUTPUT) $(LOCALBINPATH)

example:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cd examples && ../RSGLMake otherMethod
