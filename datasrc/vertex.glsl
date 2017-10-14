#version 330 core

attribute vec2 vertexPosition;

out vec4 pos;

void main() {
	pos = vec4(vertexPosition, 0, 1);
	gl_Position = pos;
}
