#version 450

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertex_color;

out vec3 the_color;

void main() {
    gl_Position = vec4(position, 1.0);
    the_color = vertex_color;
}
