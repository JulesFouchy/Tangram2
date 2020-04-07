#version 430 core

//? #define u.pts.size 1

struct Parameters {
    vec2[u.pts.size] pts; // size 3
    float smoothMin; // min 16 max 500 default 116 OR 88
    float strokeSize; // min 0. max 0.01 default 0.002
    float aa; // min 0 max 0.01 default 0.003
};

uniform Parameters u;
in vec2 vTexCoords;

float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

void main() {
    vec2 uv = vTexCoords;
    vec3 color = vec3(0.0);

    float[u.pts.size] rds;
    for (int i = 0; i < u.pts.size; ++i) {
        rds[i] = length( uv - u.pts[i] ) - 0.1;
    }

    float[u.pts.size] ds;
    for (int i = 0; i < u.pts.size; ++i) {
        ds[i] = rds[i];
        float otherDists = 0;
        for (int j = 0; j < u.pts.size; ++j) {
            if (i != j) {
                otherDists = smin(otherDists, rds[j], u.smoothMin);
            }
        }
        ds[i] -= otherDists;
    }

    //float rd1 = length(uv-u.pt1) - 0.4;
    //float rd2 = length(uv-u.pt2) - 0.1;
    //float rd3 = length(uv-u.pt3) - 0.1;
    //
    //float d1 = rd1 - smin(smin(rd2, 0, u.smoothMin), smin(rd3, 0, u.smoothMin), u.smoothMin);
    //float d2 = rd2 - smin(smin(rd1, 0, u.smoothMin), smin(rd3, 0, u.smoothMin), u.smoothMin);
    //float d3 = rd3 - smin(smin(rd1, 0, u.smoothMin), smin(rd2, 0, u.smoothMin), u.smoothMin);

    //color.b += pow(0.5 + 0.5 * sin(ds[0]*150.), 50.);
    //color.g += pow(0.5 + 0.5 * sin(ds[1]*150.), 50.);
    //color.r += pow(0.5 + 0.5 * sin(ds[2]*150.), 50.);

    for (int i = 0; i < u.pts.size; ++i) {
        color += 1. - smoothstep(u.strokeSize, u.strokeSize + u.aa, abs(ds[i]));
    }
    
    gl_FragColor = vec4(color, 1.0);
}