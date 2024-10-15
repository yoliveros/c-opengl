#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void error_callback(int error, const char *msg) {
  printf("%d %s\n", error, msg);
}

bool check_status(GLuint object_id,
                  PFNGLGETSHADERIVPROC object_property_getter_func,
                  PFNGLGETSHADERINFOLOGPROC get_info_log_func,
                  GLenum status_type) {
  GLint status;
  object_property_getter_func(object_id, status_type, &status);
  if (status != GL_TRUE) {
    GLint info_log_length;
    object_property_getter_func(object_id, GL_INFO_LOG_LENGTH,
                                &info_log_length);
    GLchar *buffer = malloc(info_log_length);

    GLsizei buffer_size;
    get_info_log_func(object_id, info_log_length, &buffer_size, buffer);
    fprintf(stderr, "Vertex Shader %s\n", buffer);

    free(buffer);

    return false;
  }

  return true;
}

bool check_shader_status(GLuint shader_id) {
  return check_status(shader_id, glGetShaderiv, glGetShaderInfoLog,
                      GL_COMPILE_STATUS);
}

bool check_program_status(GLuint program_id) {
  return check_status(program_id, glGetProgramiv, glGetProgramInfoLog,
                      GL_LINK_STATUS);
}

char *read_file(char *file_name) {
  const size_t buffer_size = 1024;

  errno = 0;
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error opening file: %s\n%s\n", file_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
  char *my_file = malloc(buffer_size);

  size_t buffer_read = 0;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (buffer_read >= buffer_size) {
      my_file = (char *)realloc(my_file, buffer_read + 10);
      if (!my_file) {
        fprintf(stderr, "Memory allocation failed for file: %s\n", file_name);
        fclose(file);
        exit(EXIT_FAILURE);
      }
    }

    my_file[buffer_read++] = c;
  }

  my_file[buffer_read] = '\0'; // NULL

  fclose(file);
  return my_file;
}

int main() {
  GLFWwindow *window;

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //   glfwWindowHint();

  glfwSetErrorCallback(error_callback);
  window = glfwCreateWindow(WIDTH, HEIGHT, "Hola World", NULL, NULL);
  printf("GLFW: %s\n", glfwGetVersionString());
  if (!window) {
    fprintf(stderr, "Failed to create a GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize OpenGL context\n");
    exit(EXIT_FAILURE);
  }
  printf("GL: %s\n", glad_glGetString(GL_VERSION));

  glfwSetKeyCallback(window, key_callback);

  GLfloat verts[] = {
      0.0,  0.0,  1.0, 0.0, 0.0,

      -1.0, 1.0,  0.0, 1.0, 0.0,

      1.0,  1.0,  0.0, 0.0, 1.0,

      -1.0, -1.0, 1.0, 0.0, 0.0,

      1.0,  -1.0, 1.0, 0.0, 0.0,
  };

  GLuint vertex_buffer_id;
  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        (char *)(sizeof(float) * 2));

  GLushort indices[] = {
      0, 1, 2, 0, 3, 4,
  };

  GLuint index_buffer_id;
  glGenBuffers(1, &index_buffer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  adapter[0] = read_file("vertex-shader.glsl");
  glShaderSource(vertex_shader_id, 1, adapter, 0);

  adapter[0] = read_file("fragment-shader.glsl");
  glShaderSource(fragment_shader_id, 1, adapter, 0);

  glCompileShader(vertex_shader_id);
  glCompileShader(fragment_shader_id);

  if (!check_shader_status(vertex_shader_id) ||
      !check_shader_status(fragment_shader_id)) {
    exit(EXIT_FAILURE);
  }

  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  if (!check_program_status(program_id)) {
    exit(EXIT_FAILURE);
  }

  glUseProgram(program_id);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, WIDTH, HEIGHT);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
