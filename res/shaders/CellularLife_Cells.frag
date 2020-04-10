#version 430 core

//? #define u.pts.size 1

struct Parameters {
    vec2[u.pts.size] pts; // size 15
    float smoothMin; // min 16 max 500 default 116 OR 88
    float strokeSize; // min 0. max 0.01 default 0.002
    float aa; // min 0 max 0.01 default 0.003
    float centerRadius; // min 0 max 0.05 default 0.033
    float centerInnerRadius; // min 0 max 0.05 default 0.011

    float min; // min 0 max 0.1
    float max; // min 0 max 0.1

    float rSample; // min 0 max 5 default 1
    float noiseAmpl; // min 0 max 0.1 default 0.01
};

uniform Parameters u;
in vec2 vTexCoords;

float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}


// Simplex 2D noise
//
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

void main() {
    vec2 uv = vTexCoords;
    float alpha = 0.0;
    vec3 color = vec3(1.);

    float[u.pts.size] rds;
    for (int i = 0; i < u.pts.size; ++i) {
        //float angle = atan(u.pts[i].y - uv.y, u.pts[i].x - uv.y);
        //float noise = u.noiseAmpl*snoise(vec2(u.rSample*cos(angle), u.rSample*sin(angle))+u.rSample);
        rds[i] = length( uv - u.pts[i] ) - 0.1;// + noise;
    }
    float D = length( uv ) - 0.9;
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
        ds[i] -= smin(0., -D-0.05, u.smoothMin);
    }

    int count = 0;
    for (int i = 0; i < u.pts.size; ++i) {
        alpha += 1. - smoothstep(u.strokeSize, u.strokeSize + u.aa, abs(ds[i]));
        alpha += 0.1 * smoothstep(0.001, 0., ds[i]);
        if (u.min < rds[i] && rds[i] < u.max)
            count += 1;
    }
    //if (count > 1) {
    //    color = vec3(1., 0., 0.);
    //    alpha = 1;
    //}

    // Cell Center
    for (int i = 0; i < u.pts.size; ++i) {
        alpha += 1. - smoothstep(u.centerRadius, u.centerRadius + u.aa, abs(rds[i]+0.1));
        //alpha -= 1. - smoothstep(u.centerInnerRadius, u.centerInnerRadius + u.aa, abs(rds[i]+0.1));
    }
    // Container circle
    alpha += 1. - smoothstep(u.strokeSize, u.strokeSize + u.aa, abs(D));

    gl_FragColor = vec4(color, alpha);
}