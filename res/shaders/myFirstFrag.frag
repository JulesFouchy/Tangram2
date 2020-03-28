#version 430 core

struct Parameters {
    float red; // default 0.5 min -1 max 2
    vec2 red2; // default 0.5 1.1 min -3 max 20
    float test; // default 0 min 0 max 1
    vec3 col;
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    gl_FragColor = vec4(u.red, vTexCoords, 1.0);
}