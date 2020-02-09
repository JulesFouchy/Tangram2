#version 430 core

in vec2 vTexCoords;

uniform sampler2D u_TextureSlot;

void main() {
	vec4 texColor = texture(u_TextureSlot, vTexCoords);
	gl_FragColor = texColor;
}