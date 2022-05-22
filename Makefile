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
	cp ColleagueRiley.vscode-RSGLMake-0.9 $(VSCPATH)\extensions

localInstall:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	#cp $(OUTPUT) $(LOCALBINPATH)
	cp ColleagueRiley.vscode-RSGLMake-0.9 $(VSCPATH)\extensions
	
example:
	$(GXX) $(SOURCE) -o $(OUTPUT)
	cd examples && ../RSGLMake otherMethod