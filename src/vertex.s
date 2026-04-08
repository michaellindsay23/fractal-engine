#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvp;
void main() {
  gl_Position = mvp * vec4(aPos.x, aPos.y + 0.5, aPos.z, 1.0);
  gl_PointSize = 10.0;
}
