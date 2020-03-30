#version 430 core

struct Parameters {
    vec2 center1; // default 0 0
    vec2 center2; // default 0 0.3
    float k; //default 10 min 2 max 30
};

in vec2 vTexCoords;

uniform Parameters u;

float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

void main(){
    float d1 = length(vTexCoords - u.center1);
    float d2 = length(vTexCoords - u.center2);
    float d = smin(d1, d2, u.k);
    gl_FragColor = vec4(vTexCoords, 0.5, 1.0);
    gl_FragColor.a = smoothstep(0.3001, 0.3, d);
}