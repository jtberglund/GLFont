#version 330 core

layout(location = 0) in vec4 uv;
uniform mat4 mvp;
out vec2 texcoord;

void main() {
	gl_Position = mvp * vec4 (uv.xy, 0, 1);
	texcoord = uv.zw;
}