#version 430 core

struct Parameters {
    vec2 center; // default 0 0
    float red; // default 0.5 min 0 max 1
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - u.center);
    gl_FragColor = vec4(vTexCoords, u.red, 1.0);
    gl_FragColor.a = smoothstep(0.3, 0.3001, d);
}