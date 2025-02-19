gcc -Wall -o main -g src\simulation.c src\main.c src\renderer.c src\shader.c src\camera.c src\model_parser.c src\polygon.c src\debug.c src\timer.c src\input.c src\physics.c -I lib\GLFW\include -I lib\GLEW\include -I lib\cglm\include -I lib\CIMGUI -L lib\GLEW\lib\Release\x64 -L lib\GLFW\lib-mingw-w64 -lglew32s -l glfw3 -lgdi32 -lopengl32 -DCGLM_FORCE_LEFT_HANDED