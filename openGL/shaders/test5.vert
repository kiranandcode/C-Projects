#version 130

in vec3 vertex_position;
in vec3 vertex_normal;

uniform mat4 matrix, view, proj;

out vec3 position_eye, normal_eye;

void main() {
  position_eye = vec3(view * matrix * vec4(vertex_position, 1.0));
  normal_eye   = vec3(view * matrix * vec4(vertex_normal, 0.0));
  gl_Position = proj * vec4(position_eye, 1.0);
}
