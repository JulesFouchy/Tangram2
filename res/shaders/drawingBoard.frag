#version 430 core

in vec2 vTexCoords;

void main(){
    vec3 color = vec3(0.8, 0.3, 0.7);
    gl_FragColor = vec4(color, 1.0);
}