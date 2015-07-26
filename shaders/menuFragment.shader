#version 330 core

in vec2 texcoord;
uniform vec4 bgColor;
uniform sampler2D tex;
out vec4 color;
 
void main() {
    color = bgColor;
}
