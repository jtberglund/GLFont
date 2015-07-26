#version 330 core

in vec2 texcoord;
uniform vec4 textColor;
uniform sampler2D tex;
out vec4 color;

void main() {
	color = vec4(textColor.rgb, texture(tex, texcoord).r);
}