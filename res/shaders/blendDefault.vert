#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

void main(void){
    vTexCoords = texCoords;
    gl_Position = vec4(vertexPos, 0.0, 1.0);
}