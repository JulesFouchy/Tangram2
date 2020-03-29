#version 430 core

struct Parameters {
    float gray; // default 0.5 min 0 max 1
    float radius; // default 0.3 min 0 max 1
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    gl_FragColor = vec4(vec3(u.gray), smoothstep(u.radius+0.001, u.radius-0.001, d));
}