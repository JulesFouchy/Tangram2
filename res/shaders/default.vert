#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

uniform mat3 u_mat;

void main(void){
    vTexCoords = texCoords;
    gl_Position = vec4((u_mat * vec3(vertexPos, 1.0)).xy, 0.0, 1.0);
}