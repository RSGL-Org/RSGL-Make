GXX = g++
SOURCE = source/*.cpp
OUTPUT = RSGLMake

LOCALBINPATH = ~/.local/bin
BINPATH = /usr/bin 
VSCPATH = ~/.vscode/

build:
	$(GXX) $(SOURCE) -o $(OUTPUT)

install:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	sudo cp $(OUTPUT) $(BINPATH)

localInstall:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	#cp $(OUTPUT) $(LOCALBINPATH)
	
vscode:
	cp -rrsglmake-vscode $(VSCPATH)\extensions
	
example:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cd examples && ../RSGLMake otherMethod