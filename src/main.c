#include "glad/glad.h"
#include "src/utils.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glad_glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

typedef enum {
  VERTEX,
  FRAGMENT,
  LINK,
} SHADER_TYPE;

void get_gl_error(int shader_id, SHADER_TYPE shader_type) {
  int success;
  char info_log[512];
  if (shader_type == LINK) {
    glad_glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
  } else {
    glad_glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  }

  if (!success) {
    if (shader_type == VERTEX) {
      glad_glGetShaderInfoLog(shader_id, 512, NULL, info_log);
      fprintf(stderr, "Vertex Shader compilation error: %s\n", info_log);
    } else if (shader_type == FRAGMENT) {
      glad_glGetShaderInfoLog(shader_id, 512, NULL, info_log);
      fprintf(stderr, "Fragment Shader compilation error: %s\n", info_log);
    } else {
      glad_glGetProgramInfoLog(shader_id, 512, NULL, info_log);
      fprintf(stderr, "Shader Program linking error: %s\n", info_log);
    }
  }
}

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Open tuto", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create a GLFW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
  const char *vertex_shader_source = read_file("shaders/vertex-shader.glsl");
  glad_glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glad_glCompileShader(vertex_shader);
  get_gl_error(vertex_shader, VERTEX);

  unsigned int fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmen_shader_source = read_file("shaders/fragment-shader.glsl");
  glad_glShaderSource(fragment_shader, 1, &fragmen_shader_source, NULL);
  glad_glCompileShader(fragment_shader);
  get_gl_error(fragment_shader, FRAGMENT);

  unsigned int shader_program = glad_glCreateProgram();
  glad_glAttachShader(shader_program, vertex_shader);
  glad_glAttachShader(shader_program, fragment_shader);
  glad_glLinkProgram(shader_program);
  get_gl_error(shader_program, LINK);

  glad_glDeleteShader(vertex_shader);
  glad_glDeleteShader(fragment_shader);

  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };

  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glad_glGenVertexArrays(1, &VAO);
  glad_glGenBuffers(1, &VBO);
  glad_glGenBuffers(1, &EBO);
  glad_glBindVertexArray(VAO);

  glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                    GL_STATIC_DRAW);

  glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                    GL_STATIC_DRAW);

  glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glad_glEnableVertexAttribArray(0);

  glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
  glad_glBindVertexArray(0);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glad_glUseProgram(shader_program);
    glad_glBindVertexArray(VAO);
    // glad_glDrawArrays(GL_TRIANGLES, 0, 3);
    glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glad_glDeleteVertexArrays(1, &VAO);
  glad_glDeleteBuffers(1, &VBO);
  glad_glDeleteBuffers(1, &EBO);
  glad_glDeleteProgram(shader_program);

  glfwTerminate();

  return EXIT_SUCCESS;
}
