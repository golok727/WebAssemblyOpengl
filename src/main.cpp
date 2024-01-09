#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "shader.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <GLFW/glfw3.h>

GLFWwindow *window;

int wWidth = 1920;
int wHeight = 1080;
Shader *basicShader = nullptr;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  wWidth = width;
  wHeight = height;
}

#ifdef __EMSCRIPTEN__
extern "C" void EMSCRIPTEN_KEEPALIVE canvas_resize(int width, int height)
{
  framebuffer_size_callback(window, width, height);
}
#endif

static void error_callback(int error, const char *description)
{
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void force_exit()
{
#ifdef __EMSCRIPTEN__
  emscripten_force_exit(EXIT_FAILURE);
#else
  exit(EXIT_FAILURE);
#endif
}

static void generate_frame()
{
  glViewport(0, 0, wWidth, wHeight);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main()
{

  system("cd");
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
  {
    fprintf(stderr, "Error: GLFW Initialization failed.");
    force_exit();
  }

  window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
  if (!window)
  {
    fprintf(stderr, "Error: GLFW Window Creation Failed");
    glfwTerminate();
    force_exit();
  }

#ifndef __EMSCRIPTEN__ // Only if not with js
  glfwGetFramebufferSize(window, &wWidth, &wHeight);
  framebuffer_size_callback(window, wWidth, wHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
#endif

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);

// Initialize GLEW after the OpenGL context is created
#ifndef __EMSCRIPTEN__
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Error: GLEW Initialization failed.");
    glfwDestroyWindow(window);
    glfwTerminate();
    force_exit();
  }
#endif

  basicShader = Shader::LoadFromFiles("assets/basic.vert.glsl", "assets/basic.frag.glsl");
  basicShader->detach();
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // Bottom-left
      0.5f, -0.5f, 0.0f,  // Bottom-right
      0.5f, 0.5f, 0.0f,   // Top-right
      -0.5f, 0.5f, 0.0f   // Top-left
  };

  int indices[] = {0, 1, 2, 2, 3, 0};

  GLuint VBO, VAO, IBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  basicShader->use();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(VAO);
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(generate_frame, 0, false);
#else
  while (!glfwWindowShouldClose(window))
  {
    generate_frame();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  delete basicShader;
  exit(EXIT_SUCCESS);
#endif
}
