#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <functional>

#include "imguistyle.h"
#include "imguielem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <GLFW/glfw3.h>

GLFWwindow *window;

int wWidth = 1920;
int wHeight = 1080;

glm::mat4 mvp;
glm::mat4 projectionMatrix;

glm::vec3 objPosition = glm::vec3(0);
glm::vec3 objScale = glm::vec3(1);

glm::vec4 clearColor = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
glm::vec4 objColor = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);

Shader *basicShader = nullptr;

void updateProjectionMatrix()
{
  projectionMatrix = glm::ortho(0.0f, static_cast<float>(wWidth), 0.0f, static_cast<float>(wHeight), -1.0f, 1.0f);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  wWidth = width;
  wHeight = height;
  updateProjectionMatrix();
}

#ifdef __EMSCRIPTEN__
extern "C" void EMSCRIPTEN_KEEPALIVE canvas_resize(int width, int height)
{
  framebuffer_size_callback(window, width, height);
}

EM_JS(void, save_ini, (const char *str), {
  console.log(UTF8ToString(str));
});

#endif

static bool isResizableWindowOpen = true;
static void drawGui()
{
  ImGui::AppAutoResize("Radha", &isResizableWindowOpen, [&]()
                       {
                         // Begin
                         ImGui::Text("Radhey Shyam!");
                         ImGui::Text("Width: %d, Height: %d", wWidth, wWidth);
                         ImGui::ColorEdit4("Background Color", glm::value_ptr(clearColor));
                         // End
                       });

  ImGui::AppAutoResize("Object Properties", &isResizableWindowOpen, [&]()
                       {
                         // Begin

                         ImGui::Text("Position");
                         ImGui::DragFloat3("Position", glm::value_ptr(objPosition));

                         ImGui::Text("Scale");
                         ImGui::DragFloat3("Scale", glm::value_ptr(objScale), 0.1);

                         ImGui::Text("Color");
                         ImGui::ColorEdit4("Color", glm::value_ptr(objColor));

                         if (ImGui::Button("Save Layout"))
                         {
#ifdef __EMSCRIPTEN__

                           size_t size = 0;
                           const char *settings = ImGui::SaveIniSettingsToMemory(&size);
                           save_ini(settings);
#endif
                         }
                         // End
                       });
};

static void draw()
{
  glm::mat4 translate = glm::translate(glm::mat4(1), objPosition);
  glm::mat4 scale = glm::scale(glm::mat4(1), objScale);
  mvp = projectionMatrix * translate * scale;

  basicShader->setUniformMat4f("u_MVP", mvp);
  basicShader->setUniform4f("u_Color", objColor.r, objColor.g, objColor.b, objColor.a);

  glViewport(0, 0, wWidth, wHeight);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
static void generate_frame()
{
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow(); // Show demo window! :)

  drawGui();

  draw();

  // Im Render
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

static void initImGui()
{
  //  Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("assets/Poppins-Regular.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("assets/Poppins-Thin.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("assets/Poppins-Medium.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("assets/Poppins-ExtraBold.ttf", 18.0f);
  io.Fonts->AddFontDefault();

  ImGui::embraceTheDarkness();
  ImGui::LoadIniSettingsFromDisk("assets/imgui.ini");
}
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

int main()
{
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Window Context
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
  initImGui();

  basicShader = Shader::LoadFromFiles("assets/basic.vert.glsl", "assets/basic.frag.glsl");
  basicShader->detach();
  // float vertices[] = {
  //     -0.5f, -0.5f, 0.0f, // Bottom-left
  //     0.5f, -0.5f, 0.0f,  // Bottom-right
  //     0.5f, 0.5f, 0.0f,   // Top-right
  //     -0.5f, 0.5f, 0.0f   // Top-left
  // };

  float vertices[] = {
      0.0f, 0.0f, 0.0f,     // Bottom-left
      200.0f, 0.0f, 0.0f,   // Bottom-right
      200.0f, 200.0f, 0.0f, // Top-right
      0.0f, 200.0f, 0.0f    // Top-left
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
  updateProjectionMatrix();

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

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  delete basicShader;
  exit(EXIT_SUCCESS);
#endif
}
