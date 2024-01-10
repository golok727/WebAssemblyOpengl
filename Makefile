CXX = em++
OUTPUT = ./js/krsna.mjs
SRCDIR = ./src
IMGUI_DIR:=$(SRCDIR)/vendor/imgui
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)

INCLUDES = ./includes
LIBS = -lGL
FLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1
USE_WASM = -s WASM=1
PAC_FILES = assets
# PAC_TYPE = --embed-file
PAC_TYPE = --preload-file
FN_EXPORTS = -s EXPORTED_FUNCTIONS=_canvas_resize
RUNTIME_METHODS = -s EXPORTED_RUNTIME_METHODS=ccall,cwrap

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CXX) $(SOURCES) -o $(OUTPUT) -I $(INCLUDES) -I ./src/vendor $(FLAGS) $(USE_WASM) $(PAC_TYPE) $(PAC_FILES) $(FN_EXPORTS) $(RUNTIME_METHODS) $(LIBS)

clean:
	del .\js\krsna.mjs
