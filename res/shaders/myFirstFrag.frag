#version 430 core

struct Parameters {
    float red; // default 0.5 min 0 max 1
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    gl_FragColor = vec4(u.red, vTexCoords, 1.0);
}