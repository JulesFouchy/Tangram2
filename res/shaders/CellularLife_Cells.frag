#version 430 core

//? #define u.pts.size 1
//? #define u.rand.size 1

const vec3 green = vec3(51, 255, 195) / 255.;
const vec3 violet = vec3(250, 19, 94) / 255.;

struct Parameters {
    vec2[u.pts.size] pts; // size 72
    vec2[u.rand.size] rand; // size 72
    float transp; // default 0.813 min 0 max 0.813
    float cellRadius; // default 0.1 min 0 max 0.4
    float smoothMin; // min 16 max 500 default 116 OR 88
    float strokeSize; // min 0. max 0.01 default 0.001
    float strokeProp; // min 0 max 1 default 0.4
    float aa; // min 0 max 0.01 default 0.002
    float centerRadius; // min 0 max 0.05 default 0.027
    float centerInnerRadius; // min 0 max 0.05 default 0.011
    float prop; // min 0 max 2 default 0.346

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

float snoise(vec2 v);

void main() {
    vec2 uv = vTexCoords;
    float alpha = 0.0;
    vec3 color = vec3(1.);

    float[u.pts.size] rds;
    float minDist = 100000;
    int cellIndex = 0;
    for (int i = 0; i < u.pts.size; ++i) {
        //float angle = atan(u.pts[i].y - uv.y, u.pts[i].x - uv.y);
        //float noise = u.noiseAmpl*snoise(vec2(u.rSample*cos(angle), u.rSample*sin(angle))+u.rSample);
        rds[i] = length( uv - u.pts[i] ) - u.cellRadius * u.prop;// + noise;
        if (rds[i] < minDist) {
            cellIndex = i;
            minDist = rds[i];
        }
    }
    color = u.rand[cellIndex].x < 0.5 ? violet : green;
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
    
    float isMiddleCell = smoothstep(0., -0.01, ds[cellIndex]) * smoothstep(u.centerRadius * u.prop, u.centerRadius * u.prop + u.aa, (rds[cellIndex]+u.cellRadius * u.prop));
    float isInnerCell = smoothstep(0., -0.01, ds[cellIndex]) * smoothstep(-0.1, 0 + u.aa, (rds[cellIndex]+u.cellRadius * u.prop));
    //color = mix(green, violet, isInnerCell);
    color = mix(color, vec3(0.), u.transp * isMiddleCell);

    int count = 0;
    for (int i = 0; i < u.pts.size; ++i) {
        //alpha += 1. - smoothstep(u.strokeSize, u.strokeSize + u.aa, abs(ds[i]));
        alpha += 1. - smoothstep(u.strokeSize * u.strokeProp * u.prop, u.strokeSize * u.strokeProp * u.prop + u.aa , (ds[i]));
        //alpha += 0.1 * smoothstep(0.001, 0., ds[i]);
        if (u.min < rds[i] && rds[i] < u.max)
            count += 1;
    }
    //if (count > 1) {
    //    color = vec3(1., 0., 0.);
    //    alpha = 1;
    //}

    // Cell Center
    for (int i = 0; i < u.pts.size; ++i) {
        alpha += 1. - smoothstep(u.centerRadius * u.prop, u.centerRadius * u.prop + u.aa, (rds[i]+u.cellRadius * u.prop));
        //alpha -= 1.2 * smoothstep(u.centerInnerRadius + u.aa, u.centerInnerRadius, (rds[i]+u.cellRadius));
    }
    // Container circle
    float isContainer = 1. - smoothstep(u.strokeSize * u.strokeProp * u.prop, u.strokeSize * u.strokeProp * u.prop + u.aa , abs(D));
    alpha += isContainer;
    color = mix(color, green, isContainer);

    gl_FragColor = vec4(color, alpha);
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