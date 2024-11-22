#version 460

out vec4 da_color;
in vec3 the_color;

void main() {
    da_color = vec4(the_color, 1.0);
}
