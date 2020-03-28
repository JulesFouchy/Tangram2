#version 430 core

struct Parameters {
    float red; // default 0.5 min -1 max 2
    vec2 red2; // default 0.5 1.1 min -1 max 1
    vec3 red3; // default 0.5 1.1 0.7 min -3 max 20
    vec4 red4; // default 0.5 1.1 0 1 min -3 max 20
    float test; // default 0 min 0 max 1
    vec3 col;
    vec4 col4;
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - vec2(0.5) + u.red2);
    gl_FragColor =u.col4;// vec4(u.red, vTexCoords+ u.red2, 1.0) + vec4(u.red3, 0.0);
}