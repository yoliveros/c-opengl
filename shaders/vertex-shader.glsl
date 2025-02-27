#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_tex_cord;

out vec3 our_color;
out vec2 tex_cord;

void main(){
  gl_Position = vec4(a_pos, 1.0);
  our_color = a_color;
  tex_cord = a_tex_cord;
}
