#version 440 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_textureSlot;

void main() {
	vec4 texColor = texture(u_textureSlot, v_texCoord);
	color = texColor;
}