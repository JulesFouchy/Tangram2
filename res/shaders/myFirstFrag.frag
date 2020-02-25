#version 430 core

in vec2 vTexCoords;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    gl_FragColor = vec4(0.0, vTexCoords, 1.0);
}