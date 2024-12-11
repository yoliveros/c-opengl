#include "glad/glad.h"
#include "src/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
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

  shader_init("shaders/vertex-shader.glsl", "shaders/fragment-shader.glsl");

  float vertices[] = {
      // positions        // colors         // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
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

  glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
  glad_glEnableVertexAttribArray(0);

  glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                             (void *)(3 * sizeof(float)));
  glad_glEnableVertexAttribArray(1);

  glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                             (void *)(6 * sizeof(float)));
  glad_glEnableVertexAttribArray(2);

  unsigned int texture;
  glad_glGenTextures(1, &texture);
  glad_glBindTexture(GL_TEXTURE_2D, texture);

  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                       GL_LINEAR_MIPMAP_LINEAR);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, data);
    glad_glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    fprintf(stderr, "Failed to load textures\n");
  }

  stbi_image_free(data);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // int nrAttributes;
  // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  // printf("Max vertex %d\n", nrAttributes);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0, 1, .5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // float time_value = glfwGetTime();
    // float green_value = (sin(time_value) / 2.0) + 0.5;
    // shader_set_float("our_color", 1.0);
    //  glad_glUniform4f(vertex_color_location, 0.0, green_value, 0.0, 1.0);

    shader_use();
    glad_glBindTexture(GL_TEXTURE_2D, texture);
    glad_glBindVertexArray(VAO);
    // glad_glDrawArrays(GL_TRIANGLES, 0, 3);
    glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glad_glDeleteVertexArrays(1, &VAO);
  // glad_glDeleteBuffers(1, &VBO);
  // glad_glDeleteBuffers(1, &EBO);
  // shader_delete();

  glfwTerminate();

  return EXIT_SUCCESS;
}
