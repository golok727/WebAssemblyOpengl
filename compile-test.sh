g++ .\src\main.cpp -I .\includes\ -o ./bin/main.exe -DGLEW_STATIC -L .\lib\ -lglew32 -lglfw3 -lopengl32 -lGdi32 


em++ ./src/main.cpp -o ./js/index.html -I ./includes/ -DGLEW_STATIC -s USE_GLFW=3 -s USE_WEBGL2=1  -s WASM=1 --preload-file assets