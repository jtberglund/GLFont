#version 330 core

layout(location = 0) in vec4 uv;
//uniform mat4 mvp;
out vec2 texcoord;

void main() {
	gl_Position.xyz = uv.xyz;
	gl_Position.w = 1.0;
	texcoord = uv.zw;
}