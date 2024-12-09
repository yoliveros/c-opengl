#include "shader.h"
#include "src/utils.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  VERTEX,
  FRAGMENT,
  LINK,
} SHADER_TYPE;

unsigned int ID;

static void get_gl_error(int shader_id, SHADER_TYPE shader_type) {
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

void shader_init(const char *vertex_path, const char *fragment_path) {
  unsigned int vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
  char *vertex_shader_source = read_file(vertex_path);
  glad_glShaderSource(vertex_shader, 1,
                      (char const *const *)&vertex_shader_source, NULL);
  glad_glCompileShader(vertex_shader);
  get_gl_error(vertex_shader, VERTEX);

  unsigned int fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
  char *fragmen_shader_source = read_file(fragment_path);
  glad_glShaderSource(fragment_shader, 1,
                      (char const *const *)&fragmen_shader_source, NULL);
  glad_glCompileShader(fragment_shader);
  get_gl_error(fragment_shader, FRAGMENT);

  ID = glad_glCreateProgram();
  glad_glAttachShader(ID, vertex_shader);
  glad_glAttachShader(ID, fragment_shader);
  glad_glLinkProgram(ID);
  get_gl_error(ID, LINK);

  glad_glDeleteShader(vertex_shader);
  glad_glDeleteShader(fragment_shader);

  free(vertex_shader_source);
  free(fragmen_shader_source);
}

void shader_use() { glad_glUseProgram(ID); }

void shader_delete() { glad_glDeleteProgram(ID); }

void shader_set_bool(const char *name, bool value) {
  glad_glUniform1i(glad_glGetUniformLocation(ID, name), value);
}

void shader_set_int(const char *name, int value) {
  glad_glUniform1i(glad_glGetUniformLocation(ID, name), value);
}

void shader_set_float(const char *name, float value) {
  glad_glUniform1f(glad_glGetUniformLocation(ID, name), value);
}
