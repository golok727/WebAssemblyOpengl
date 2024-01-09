CXX = em++

OUTPUT=./js/krsna.mjs

SOURCES = ./src/main.cpp ./src/shader.cpp 

LIBS = -lGL
FLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1
USE_WASM = -s WASM=1
PAC_FILES = assets
PAC_TYPE = --embed-file
FN_EXPORTS = -s EXPORTED_FUNCTIONS=_canvas_resize
RUNTIME_METHODS =-s EXPORTED_RUNTIME_METHODS=ccall,cwrap


all: $(SOURCES) $(OUTPUT) 

$(OUTPUT): $(SOURCES) 
	$(CXX) $(SOURCES) -o $(OUTPUT) -I ./includes $(FLAGS) $(USE_WASM) $(PAC_TYPE) $(PAC_FILES) $(FN_EXPORTS) $(RUNTIME_METHODS) 
clean: 
	del $(OUTPUT)