#version 430 core

struct Parameters {
    vec3 col;
    float radius; // default 0.3 min 0 max 1
    float margin; // default 0.001 min -0.01 max 0.01
};

in vec2 vTexCoords;

uniform Parameters u;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    gl_FragColor = vec4(u.col, smoothstep(u.radius+u.margin, u.radius-u.margin, d));
}