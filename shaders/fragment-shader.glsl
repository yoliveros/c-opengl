#version 330 core
out vec4 frag_color;

in vec3 our_color;
in vec2 tex_cord;

uniform sampler2D our_texture;

void main(){
  //frag_color = texture(our_texture, tex_cord) * vec4(our_color, 1.0);
  frag_color = texture(our_texture, tex_cord);
}

