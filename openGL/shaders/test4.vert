#version 130

in vec3 vertex_position;
in vec3 vertex_colour;

uniform mat4 matrix, view, proj;

out vec3 colour;
void main() {
  colour = vertex_colour;
  gl_Position = proj * view * matrix * vec4 (vertex_position, 1.0);
}
