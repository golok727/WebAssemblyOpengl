#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif
