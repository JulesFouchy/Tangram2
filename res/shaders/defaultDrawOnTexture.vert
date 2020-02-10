#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

uniform mat3 u_localTransformMat;

void main(void){
    vTexCoords = (u_localTransformMat  * vec3(texCoords, 1.0)).xy;
    gl_Position = vec4(vertexPos, 0.0, 1.0);
}