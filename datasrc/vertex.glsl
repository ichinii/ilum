#version 330 core

attribute vec2 vertexPosition;
attribute vec4 vertexColor;

uniform mat4 mvp;

out vec4 pos;
out vec4 color;

void main() {
	pos = vec4(vertexPosition, 0, 1);
  color = vertexColor;

	gl_Position = mvp * pos;
}
