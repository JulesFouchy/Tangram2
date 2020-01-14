#version 430 core

in vec2 vTexCoords;

void main(){
    gl_FragColor = vec4(vTexCoords, 0.0, 1.0);
}