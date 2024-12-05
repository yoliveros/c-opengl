#pragma once

#include <glad/glad.h>
#include <stdbool.h>

void shader_init(const char *vertex_path, const char *fragment_path);

void shader_use();

void shader_delete();

void shader_set_bool(const char *name, bool value);
void shader_set_int(const char *name, int value);
void shader_set_float(const char *name, float value);
